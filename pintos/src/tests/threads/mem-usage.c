/* mem-usage.c
 *
 * Tests memory usage in Pintos.
 *
 * Creates multiple child threads that allocate memory and
 * monitors the amount of free memory.
 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"

#define THREAD_CNT 5
#define ALLOC_SIZE (4 * 1024) // 4 KB per allocation
#define ALLOC_CNT 256          // 256 allocations per thread (1 MB per thread)

static void memory_thread (void *aux);
static size_t get_total_free_pages(void);

void
test_mem_usage (void) 
{
  int i;
  size_t free_pages_before, free_pages_after;

  msg("Starting memory usage test...");

  free_pages_before = get_total_free_pages();
  msg ("Free pages before: %zu", free_pages_before);

  msg ("Starting %d threads...", THREAD_CNT);
  for (i = 0; i < THREAD_CNT; i++) 
    {
      char name[25];
      snprintf(name, sizeof name, "mem_thread %d", i);
      thread_create (name, PRI_DEFAULT, memory_thread, NULL);
    }

  // Wait for children to finish
  for(i = 0; i < THREAD_CNT; i++){
      thread_yield();
  }
  free_pages_after = get_total_free_pages();
  msg ("Free pages after: %zu", free_pages_after);
  msg ("Approximate memory used: %zu KB", (free_pages_before - free_pages_after) * PGSIZE / 1024);
  pass();
}

static void
memory_thread (void *aux UNUSED) 
{
  int i;
  char *allocations[ALLOC_CNT];

  for (i = 0; i < ALLOC_CNT; i++) {
    allocations[i] = (char *) malloc(ALLOC_SIZE);
    if (allocations[i] == NULL) {
      fail("malloc failed in thread %s", thread_current()->name);
    }
    // Fill memory with some data to ensure it's allocated
    memset(allocations[i], 0xAA, ALLOC_SIZE); 
  }

  // Do nothing, just hold the allocated memory

  // Free the allocated memory
  for (i = 0; i < ALLOC_CNT; i++) {
    free(allocations[i]);
  }
}

static size_t get_total_free_pages(void){
    return palloc_get_free_cnt() + ualloc_get_free_cnt();
}