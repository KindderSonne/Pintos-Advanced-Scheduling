#include <stdio.h>
#include <stdlib.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"
#include "threads/palloc.h"

static thread_func mem_usage_thread;

#define THREAD_COUNT 30
#define ALLOC_SIZE (1024 * 16) // 16KB
#define ITERATIONS 100

void
test_priority_mem_usage (void)
{
  int i;
  int priority;

  msg ("Creating %d threads with different priorities to test memory usage...", THREAD_COUNT);

  for (i = 0; i < THREAD_COUNT; i++)
    {
      char name[32];
      snprintf(name, sizeof name, "mem-usage-%d", i);
      priority = i % (PRI_MAX - PRI_MIN + 1) + PRI_MIN; // Chia đều priority
      /* The higher number, the higher priority */
      thread_create (name, priority, mem_usage_thread, NULL);
    }

  /* Let the threads run for a while. Adjust as needed. */
  timer_sleep (10 * TIMER_FREQ);

  /* Print memory usage statistics. */
  size_t allocated_pages = palloc_get_allocated_pages();
  size_t used_pages = palloc_get_used_pages();
  size_t malloc_allocated = get_malloc_bytes_allocated();

  msg ("Memory usage statistics:");
  msg ("  Total pages allocated: %zu", allocated_pages);
  msg ("  Pages currently in use: %zu", used_pages);
  msg ("  Bytes allocated by malloc: %zu", malloc_allocated);

  msg ("Priority memory usage test finished.");
  pass();
}

static void
mem_usage_thread (void *aux UNUSED)
{
  int i;
  void *ptr;

  for (i = 0; i < ITERATIONS; i++)
    {
      ptr = malloc (ALLOC_SIZE);
      if (ptr == NULL) {
        msg ("Failed to allocate memory in iteration %d", i);
        return;
      }
      /* Do some work with the allocated memory to simulate real usage. */
      memset(ptr, i % 256, ALLOC_SIZE);
      
      free (ptr);

    }
}