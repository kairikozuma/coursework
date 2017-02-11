#ifndef DRAM_H
#define DRAM_H

#include "types.h"

// latency constants
#define ACT_LATENCY 45
#define CAS_LATENCY 45
#define PRE_LATENCY 45
#define BUS_LATENCY 10
#define DEFAULT_LATENCY 100

// DRAM configuration
#define ROW_BUF_SIZE 1024
#define NUM_BANKS 16
#define MAX_ROW_BUF 256

typedef struct rowbuf_entry_t {
  bool valid;
  uns64 row_id;
} Rowbuf_Entry;

typedef struct dram_t {
  Rowbuf_Entry * rowbuf_array;
  uns64 stat_read_access;
  uns64 stat_write_access;
  uns64 stat_write_delay;
  uns64 stat_read_delay;
  uns64 stat_rowbuf_hit;
  uns64 stat_rowbuf_miss;
  uns64 stat_rowbuf_empty;
} DRAM;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

DRAM   *dram_new();
void    dram_print_stats(DRAM *dram);
/* Part B access */
uns64   dram_access(DRAM *dram,Addr lineaddr, Flag is_dram_write);
/* Part CDE access */
uns64   dram_access_mode_CDE(DRAM *dram,Addr lineaddr, Flag is_dram_write);


#endif // DRAM_H
