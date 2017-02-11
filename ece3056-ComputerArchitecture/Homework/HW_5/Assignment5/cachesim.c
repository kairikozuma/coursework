#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cachesim.h"

// Global statistic counters
counter_t accesses = 0, hits = 0, read_count = 0, write_count = 0, misses = 0, misses_read = 0, misses_write = 0, writebacks = 0;

// Define size of word
typedef unsigned int cache_word_t;

// Define a boolean variable of one byte
typedef unsigned char obolean;

/**
 * Cache line
 */
typedef struct cache_line_t{
  boolean valid;
  boolean dirty;
  cache_word_t tag;
  cache_word_t *word;
  struct cache_line_t *next_lru;
} cache_line_t;

/**
 * Cache set
 */
typedef struct cache_set_t{
  int stack_count;
  cache_line_t *cache_line_array;
  // LRU stack pointers
  cache_line_t *stack_mru; // Most recently used line
  cache_line_t *stack_lru; // Least recently used line
} cache_set_t;

/**
 * Cache
 */
typedef struct cache_t{
   cache_set_t *cache_set_array;
   int ways;
   int sets;
   int cache_size;
   int block_size;
   int words_per_block;
   int set_bits;
   int byte_offset_bits;
   int tag_bits;
} cache_t ;

// Global cache_t pointer that points to the cache
cache_t *cache_g;

void cachesim_init(int blocksize, int cachesize, int ways) {
  // Allocate memory for cache
  cache_g = malloc(sizeof(cache_t));

  //TODO: cleanup
  // Initialize cache member variables
  cache_g->ways = ways;
  cache_g->sets = cachesize / (blocksize * ways);
  cache_g->cache_size = cachesize;
  cache_g->block_size = blocksize;
  cache_g->words_per_block = cache_g->block_size / (cache_word_t) sizeof(cache_word_t);
  cache_g->set_bits = (int) log2(cache_g->sets);
  cache_g->byte_offset_bits = (int) 2 + log2(cache_g->words_per_block);
  cache_g->tag_bits = (int) 32 - cache_g->byte_offset_bits - cache_g->set_bits;

  // Initialization value
  cache_word_t val = 0x00000000;

  // Allocate memory for set array within cache
  cache_g->cache_set_array = (cache_set_t*) malloc(sizeof(cache_set_t) * cache_g->sets);

  // Initialize pointer to cache set
  cache_set_t *set_p = cache_g->cache_set_array;

  // Iterate over sets
  for (size_t i = 0; i < cache_g->sets; i++) {

    // Allocate array of cache lines (N lines for N ways)
    set_p->cache_line_array = (cache_line_t*) malloc(sizeof(cache_line_t) * cache_g->ways);

    // Set LRU stack to NULL
    set_p->stack_mru = 0;
    set_p->stack_lru = 0;

    // Initialize count of lines
    set_p->stack_count = 0;

    // Initialize pointer to cache line
    cache_line_t *line_p = set_p->cache_line_array;

    // Iterate over ways
    for (size_t j = 0; j < cache_g->ways; j++) {

      // Initialize cache line information
      line_p->valid = 0;
      line_p->dirty = 0;
      line_p->tag = 0x000000000;
      line_p->next_lru = 0;

      // Allocate block of words
      line_p->word = (cache_word_t*) malloc(sizeof(cache_word_t) * cache_g->block_size);

      // Initialize pointer to blocks
      cache_word_t *word_p = line_p->word;

      // Iterate over block
      for(size_t k = 0; k < cache_g->words_per_block; k++) {
        (*word_p++) = val++; //Initial value for block
      }
      line_p++; // Next line
    }
    set_p++; // Next set
  }
}

// Helper method that handles access to set with LRU implementation
void set_LRU_access(cache_set_t *set_p, cache_word_t tag, int write) {
  // Declare line to put new data into
  cache_line_t *line_p;

  boolean hit = 0;

  // If no lines in set, add line at start of array
  if (set_p->stack_count == 0) {
    line_p = set_p->cache_line_array;
    set_p->stack_mru = line_p;
    set_p->stack_lru = line_p;
    set_p->stack_count++;
  // Check if top of stack is a hit, no reordering necessary
  } else if (set_p->stack_mru->tag == tag) {
    line_p = set_p->stack_mru;
    hit = 1;
  // Check if number of ways is 1, no associativity
  } else if (set_p->stack_count == 1 && cache_g->ways == 1 && set_p->stack_mru->tag != tag) {
    if (set_p->stack_mru->tag != tag) {
      if (set_p->stack_lru->dirty == 1) {
        writebacks++;
      }
    } else {
      hit = 1;
    }
    line_p = set_p->stack_mru;
  // Else, search linked list for tag
  } else {
    cache_line_t *line_temp_p = set_p->stack_mru;

    while (line_temp_p->next_lru != 0 && line_temp_p->next_lru->tag != tag) {
      line_temp_p = line_temp_p->next_lru;
    }

    // If hit exists, move to top of LRU
    if (line_temp_p->next_lru != 0) {
      hit = 1;
      line_p = line_temp_p->next_lru;
      if (line_p == set_p->stack_lru) {
        set_p->stack_lru = line_temp_p;
      }
      line_temp_p->next_lru = line_p->next_lru;
      line_p->next_lru = set_p->stack_mru;
      set_p->stack_mru = line_p;

    // Else, add line
    } else {
      // If set not full, add to first open spot
      if (set_p->stack_count < cache_g->ways) {
        line_p = set_p->cache_line_array;
        while (line_p->valid == 1) {
          line_p++;
        }

        // Insert at top of LRU stack
        line_p->next_lru = set_p->stack_mru;
        set_p->stack_mru = line_p;

        // Increment count of lines in stack
        set_p->stack_count++;

      // If set is full, remove LRU line and add to that spot
      } else {
        // Find the second to last LRU line
        // Temporary pointer to find second to last LRU
        cache_line_t *line_stl_p = set_p->stack_mru;
        while (line_stl_p->next_lru != set_p->stack_lru) {
          line_stl_p = line_stl_p->next_lru;
        }
        // If dirty, increment writeback counter
        if (set_p->stack_lru->dirty == 1) {
          writebacks++;
        }
        line_p = set_p->stack_lru;
        line_p->next_lru = set_p->stack_mru;
        set_p->stack_mru = line_p;
        set_p->stack_lru = line_stl_p;
        set_p->stack_lru->next_lru = 0;
      }
    }
  }

  // Increment accesses
  accesses++;

  // Increment counters and set line information
  if (hit) {
    hits++;
  } else {
    line_p->tag = tag;
    line_p->valid = 1;
    line_p->dirty = 0;
    misses++;
    if (write) {
      misses_write++;
    } else {
      misses_read++;
    }
  }
  if (write) {
    line_p->dirty = 1;
    write_count++;
  } else {
    read_count++;
  }
}

void cachesim_access(addr_t physical_addr, int write) {

  // Extract tag bits
  cache_word_t tag = (cache_word_t) physical_addr >> (32 - cache_g->tag_bits);

  // Extract set index
  cache_word_t set_index = (cache_word_t) (physical_addr & (0xFFFFFFFF >> cache_g->tag_bits)) >> cache_g->byte_offset_bits;

  // Index into cache set
  cache_set_t *set_p = &cache_g->cache_set_array[set_index];

  // printf("Address:%08x Tag: %08x Set: %08x\n", (cache_word_t) physical_addr, tag, set_index);

  // Boolean of whether access was hit
  set_LRU_access(set_p, tag, write);
}

/**
 * Print the cache content
 */
void cachesim_print() {
  // Initialize local variables
  int sets = cache_g->sets;
  int ways = cache_g->ways;
  int words_per_block = cache_g->words_per_block;

  // Print sets, ways, and words per block
  printf("Cache Size: %d\nWays: %d\nSets: %d\nBlock Size: %d\nWordsPerBlock %d\nByteOffset Bits: %d\nSet Bits: %d\nTag Bits: %d\n", cache_g->cache_size, ways, sets, cache_g->block_size, words_per_block, cache_g->byte_offset_bits, cache_g->set_bits, cache_g->tag_bits);

  // Initialize pointer to cache set
  cache_set_t *set_p = cache_g->cache_set_array;

  // Iterate over sets
  for (size_t i = 0; i < sets; i++) {
    // Use "[" and "]" to separate sets
    printf("[ ");

    // Initialize pointer to cache line
    cache_line_t *line_p = set_p->cache_line_array;

    // Iterate over ways
    for (size_t j = 0; j < ways; j++) {

      printf("<V:%d D:%d Tag:%08x> ", line_p->valid, line_p->dirty, line_p->tag);

      // Initialize pointer to blocks
      cache_word_t *word_p = line_p->word;

      // Iterate over block*
      for(size_t k = 0; k < words_per_block; k++) {
        // Print the value of each word
        printf("%08x ", *word_p++); // Next block
      }
      // Use "|" to separate ways
      // Do not print the "|" after the last way
      if (j != ways - 1) {
        printf("| ");
      }
      line_p++; // Next line
    }
    printf("]\n");
    set_p++; // Next set
  }
}

void cachesim_print_stat_headers() {
  printf("Accesses,Hits,Misses,ReadMiss,ReadCount,WriteMiss,WriteCount,WriteBack\n");
}

void cachesim_print_stats() {
  // Verbose stats
  // printf("%llu, %llu, %llu, %llu, %llu, %llu, %llu, %llu\n", accesses, hits, misses, misses_read, read_count, misses_write, write_count, writebacks);
  // Regular stats
  printf("%llu, %llu, %llu, %llu\n", accesses, hits, misses, writebacks);
}

void cachesim_destroy() {
  // Free block pointer and cache line array pointer
  for (size_t i = 0; i < cache_g->sets; i++) {
    for (size_t j = 0; j < cache_g->ways; j++) {
      free(cache_g->cache_set_array[i].cache_line_array[j].word);
    }
    free(cache_g->cache_set_array[i].cache_line_array);
  }

  // Free cache set array pointer
  free(cache_g->cache_set_array);

  // Free global cache_t pointer
  free(cache_g);
}
