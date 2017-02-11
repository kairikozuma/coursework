/**
 * Cache simulator
 * Set associative
 * Fixed block size
 * Write back policy
 */
#ifndef __CACHESIM_H
#define __CACHESIM_H
typedef unsigned long long addr_t;
typedef unsigned long long counter_t;

/**
 * Initializes the cache with specified block_size, cache_size, and ways
 * @param block_size integer bytes of cache line
 * @param cache_size integer total cache size
 * @param ways       integer number of ways
 */
void cachesim_init(int block_size, int cache_size, int ways);

/**
 * Simulate access for a cache
 * @param physical_add physical address to access
 * @param write        whether the access is a write
 */
void cachesim_access(addr_t physical_add, int write);

/**
 * Print accesses, hits, miss, and writeback stats
 */
void cachesim_print_stats(void);

/**
 * Print cache content
 */
void cachesim_print();

/**
 * Destroy the cache safely
 */
void destroy();

#endif
