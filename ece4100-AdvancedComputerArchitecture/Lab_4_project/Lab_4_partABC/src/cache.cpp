#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cache.h"

/* Values from sim.cpp */
extern uns64 cycle;
extern uns64 SWP_CORE0_WAYS;

/* Helper function for log2() */
uns64 log2(uns64 num) {
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

Cache * cache_new(uns64 size, uns64 assocs, uns64 linesize, uns64 repl_policy) {
  /* Allocate memory for cache struct */
  Cache * cache = (Cache *) malloc(sizeof(Cache));

  /* Empty cache line for initial victim line */
  Cache_Line cache_line;
  cache_line.valid = false;
  cache_line.dirty = false;
  cache_line.tag = 0x0;
  cache_line.core_id = 0x0;
  cache_line.last_access_time = 0x0;

  /* Set other variables for cache */
  cache->ways = assocs;
  cache->sets = (size / assocs) / linesize;
  cache->index_bits = log2(cache->sets);
  cache->line_size = linesize;
  cache->policy = repl_policy;
  cache->last_evicted_line = cache_line;
  cache->stat_read_access = 0;
  cache->stat_write_access = 0;
  cache->stat_read_miss = 0;
  cache->stat_write_miss = 0;
  cache->stat_dirty_evicts = 0;

  /* Allocate memory for cache set */
  cache->cache_set_array = (Cache_Set *) calloc(cache->sets, sizeof(Cache_Set));

  /* Set seed for PRNG */
  srand(time(0));

  /* Allocate memory for lines in each cahce set */
  for (uns64 i = 0; i < cache->sets; i++) {
    cache->cache_set_array[i].cache_line_array = (Cache_Line *) calloc(cache->ways, sizeof(Cache_Line));
  }
  return cache;
}

/************* HELPER FUNCTIONS ********************/
/* Calculate set index */
uns64 calc_index(Cache *c, Addr lineaddr) {
  return ((0xFFFFFFFFFFFFFFFF) >> (64 - c->index_bits)) & lineaddr;
}

/* Calculate tag */
uns64 calc_tag(Cache *c, Addr lineaddr) {
  return lineaddr >> c->index_bits;
}

/* Calculate line address from tag and index */
uns64 calc_lineaddr(Cache *c, uns64 tag, uns64 index) {
  return (tag << c->index_bits) | index;
}
/**************************************************/

Flag cache_access(Cache * c, Addr lineaddr, uns is_write, uns core_id) {

  /* Calculate tag and set index */
  uns64 index = calc_index(c, lineaddr);
  uns64 tag = calc_tag(c, lineaddr);

  /* Update stats */
  if (is_write) {
    c->stat_write_access++;
  } else {
    c->stat_read_access++;
  }

  /* Get cache line */
  Cache_Set * cache_set = &(c->cache_set_array[index]);

  /* Check if tag matches any in set */
  for (uns64 i = 0; i < c->ways; i++) {
    Cache_Line * cache_line = &(cache_set->cache_line_array[i]);
    /* If valid and tag matches, cache hit */
    if (cache_line->valid && cache_line->tag == tag) {
      /* If it is a write, set dirty bit */
      if (is_write) {
        cache_line->dirty = true;
      }
      cache_line->last_access_time = cycle;
      return HIT;
    }
  }

  /* Return miss if no match */
  if (is_write) {
    c->stat_write_miss++;
  } else {
    c->stat_read_miss++;
  }
  return MISS;
}

void cache_install(Cache *c, Addr lineaddr, uns is_write, uns core_id) {

  /* Calculate tag and set index */
  uns64 index = calc_index(c, lineaddr);
  uns64 tag = calc_tag(c, lineaddr);

  /* Get the victim way */
  uns victim_way = cache_find_victim(c, index, core_id);

  /* Copy victim line into temporary variable */
  Cache_Line victim_line = c->cache_set_array[index].cache_line_array[victim_way];

  /* Get pointer to victim line */
  Cache_Line * cache_line = &(c->cache_set_array[index].cache_line_array[victim_way]);

  /* Copy last evicted line */
  c->last_evicted_line = victim_line;
  c->last_evicted_line_addr = calc_lineaddr(c, victim_line.tag, index);

  /* Update eviction stats */
  if (c->last_evicted_line.valid && c->last_evicted_line.dirty) {
    c->stat_dirty_evicts++;
  }

  /* Install line */
  cache_line->valid = true;
  cache_line->dirty = (is_write != 0);
  cache_line->core_id = core_id;
  cache_line->last_access_time = cycle;
  cache_line->tag = tag;
}

uns cache_find_victim(Cache *c, uns set_index, uns core_id) {
  /* Get cache set */
  Cache_Set * cache_set = &(c->cache_set_array[set_index]);

  /* Way in set to remove */
  uns victim_way = 0;

  /* LRU replacement policy */
  if (c->policy == 0) {
    /* Track victim line index and earliest start */
    uns64 earliest_access_time = cache_set->cache_line_array[0].last_access_time;
    for (uns64 i = 0; i < c->ways; i++) {
      Cache_Line * cache_line = &(cache_set->cache_line_array[i]);

      /* If line is invalid, use it */
      if (!(cache_line->valid)) {
        victim_way = i;
        break;
      /* Else, set victim line index to valid line with earliest start */
      } else if (cache_line->last_access_time < earliest_access_time) {
        earliest_access_time = cache_line->last_access_time;
        victim_way = i;
      }
    }
  /* Random replacement policy */
  } else if (c->policy == 1) {
    victim_way = (rand() % c->ways);
  }

  /* Return victim way*/
  return victim_way;
}

////////////////////////////////////////////////////////////////////
// ------------- TRY NOT TO MODIFY THE PRINT STATS FUNCTION -----------
////////////////////////////////////////////////////////////////////

void    cache_print_stats    (Cache *c, char *header){
  double read_mr =0;
  double write_mr =0;

  if(c->stat_read_access){
    read_mr=(double)(c->stat_read_miss)/(double)(c->stat_read_access);
  }

  if(c->stat_write_access){
    write_mr=(double)(c->stat_write_miss)/(double)(c->stat_write_access);
  }

  printf("\n%s_READ_ACCESS    \t\t : %10llu", header, c->stat_read_access);
  printf("\n%s_WRITE_ACCESS   \t\t : %10llu", header, c->stat_write_access);
  printf("\n%s_READ_MISS      \t\t : %10llu", header, c->stat_read_miss);
  printf("\n%s_WRITE_MISS     \t\t : %10llu", header, c->stat_write_miss);
  printf("\n%s_READ_MISS_PERC  \t\t : %10.3f", header, 100*read_mr);
  printf("\n%s_WRITE_MISS_PERC \t\t : %10.3f", header, 100*write_mr);
  printf("\n%s_DIRTY_EVICTS   \t\t : %10llu", header, c->stat_dirty_evicts);

  printf("\n");
}
