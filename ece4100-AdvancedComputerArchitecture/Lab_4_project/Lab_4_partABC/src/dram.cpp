#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "dram.h"

extern Flag DRAM_PAGE_POLICY;
extern MODE   SIM_MODE;
extern uns64  CACHE_LINESIZE;

////////////////////////////////////////////////////////////////////
// ------------- TRY NOT TO MODIFY THE PRINT STATS FUNCTION -------
////////////////////////////////////////////////////////////////////

void    dram_print_stats(DRAM *dram){
  double rddelay_avg=0;
  double wrdelay_avg=0;
  char header[256];
  sprintf(header, "DRAM");

  if(dram->stat_read_access){
    rddelay_avg=(double)(dram->stat_read_delay)/(double)(dram->stat_read_access);
  }

  if(dram->stat_write_access){
    wrdelay_avg=(double)(dram->stat_write_delay)/(double)(dram->stat_write_access);
  }

  printf("\n%s_READ_ACCESS\t\t : %10llu", header, dram->stat_read_access);
  printf("\n%s_WRITE_ACCESS\t\t : %10llu", header, dram->stat_write_access);
  printf("\n%s_READ_DELAY_AVG\t\t : %10.3f", header, rddelay_avg);
  printf("\n%s_WRITE_DELAY_AVG\t\t : %10.3f", header, wrdelay_avg);
}

/* Helper function for log2() */
uns64 log2func(uns64 num) {
  /* If zero, return 0 (should be error) */
  if (num == 0) {
    return -1;
  }

  /* Mask to get log */
  uns64 mask = 0x1;

  /* Log value */
  uns64 log_val = 0;

  while ((num & mask) == 0) {
    /* Shift num right (equal to divide by 2) */
    num = num >> 1;

    /* Increment log_val */
    log_val++;
  }

  return log_val;
}

DRAM *dram_new() {
  DRAM * dram = (DRAM *) malloc(sizeof(DRAM));
  dram->rowbuf_array = (Rowbuf_Entry *) calloc(MAX_ROW_BUF, sizeof(Rowbuf_Entry));
  dram->stat_write_access = 0;
  dram->stat_read_access = 0;
  dram->stat_write_delay = 0;
  dram->stat_read_delay = 0;
  return dram;
}

uns64 dram_access(DRAM *dram, Addr lineaddr, Flag is_dram_write) {

  /* Set to default latency for Part B */
  uns64 delay = DEFAULT_LATENCY;

  /* Update stats */
  if (is_dram_write) {
    dram->stat_write_access++;
    dram->stat_write_delay += delay;
  } else {
    dram->stat_read_access++;
    dram->stat_read_delay += delay;
  }
  return delay;
}

uns64   dram_access_mode_CDE(DRAM *dram, Addr lineaddr, Flag is_dram_write) {

  uns64 delay;

  /* If close page, delay is constant */
  if (DRAM_PAGE_POLICY == DRAM_POLICY_CLOSE_PAGE) {
    delay = ACT_LATENCY + CAS_LATENCY + BUS_LATENCY; /* No precharge needed */
  } else { /* Else, open page needs delay calculation */

    /* Calculate index into banks */
    uns64 index = (lineaddr >> log2func(ROW_BUF_SIZE / CACHE_LINESIZE)) & ~(0xFFFFFFFFFFFFFFFF << log2func(NUM_BANKS));
    uns64 row_id = lineaddr >> ((log2func(ROW_BUF_SIZE / CACHE_LINESIZE) + log2func(NUM_BANKS)));

    Rowbuf_Entry * rowbuf_entry = &(dram->rowbuf_array[index]);

    /* If writing, max delay */
    if (is_dram_write) {
      delay = PRE_LATENCY + ACT_LATENCY + CAS_LATENCY + BUS_LATENCY;
    /* Buffer is empty, no need to precharge */
    } else if (!rowbuf_entry->valid) {
      delay = ACT_LATENCY + CAS_LATENCY + BUS_LATENCY;
      dram->stat_rowbuf_empty++;
    /* If row buffer hit, just use CAS latency */
    } else if (rowbuf_entry->row_id == row_id) {
      delay = CAS_LATENCY + BUS_LATENCY;
      dram->stat_rowbuf_hit++;
    /* Else, row buffer miss */
    } else {
      delay = PRE_LATENCY + ACT_LATENCY + CAS_LATENCY + BUS_LATENCY;
      dram->stat_rowbuf_miss++;
    }
    rowbuf_entry->row_id = row_id;
    rowbuf_entry->valid = true;
  }

  /* Update stats */
  if (is_dram_write) {
    dram->stat_write_access++;
    dram->stat_write_delay += delay;
  } else {
    dram->stat_read_access++;
    dram->stat_read_delay += delay;
  }

  return delay;
}
