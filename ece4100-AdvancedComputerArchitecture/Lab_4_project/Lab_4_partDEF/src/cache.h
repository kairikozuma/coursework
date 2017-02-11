#ifndef CACHE_H
#define CACHE_H

#include "types.h"
/////////////////////////////////////////////////////////////////////////////////////////////
// Define the Data Structures here with correct field (Look at Appendix A for more details)
//////////////////////////////////////////////////////////////////////////////////////////////
typedef struct cache_line_t {
  bool valid;
  bool dirty;
  uns64 tag;
  uns64 core_id;
  uns64 last_access_time;
} Cache_Line;

typedef struct cache_set_t {
  Cache_Line * cache_line_array;
} Cache_Set;

typedef struct cache_t {
  Cache_Set * cache_set_array;
  uns64 ways;
  uns64 sets;
  uns64 line_size;
  uns64 index_bits;
  uns64 policy;
  Cache_Line last_evicted_line;
  uns64 last_evicted_line_addr;

  /* Stats */
  uns64 stat_read_access;
  uns64 stat_write_access;
  uns64 stat_read_miss;
  uns64 stat_write_miss;
  uns64 stat_dirty_evicts;

} Cache;


//////////////////////////////////////////////////////////////////////////////////////////////
// Following functions might be helpful(Look at Appendix A for more details)
//////////////////////////////////////////////////////////////////////////////////////////////


Cache  *cache_new(uns64 size, uns64 assocs, uns64 linesize, uns64 repl_policy);
Flag    cache_access         (Cache *c, Addr lineaddr, uns is_write, uns core_id);
void    cache_install        (Cache *c, Addr lineaddr, uns is_write, uns core_id);
void    cache_print_stats    (Cache *c, char *header);
uns     cache_find_victim    (Cache *c, uns set_index, uns core_id);

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#endif // CACHE_H
