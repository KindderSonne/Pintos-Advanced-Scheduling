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
test_mlfqs_mem_usage (void)
{
  int i;

  ASSERT (thread_mlfqs);

  msg ("Creating %d threads to test memory usage...", THREAD_COUNT);

  for (i = 0; i < THREAD_COUNT; i++)
    {
      char name[32];
      snprintf(name, sizeof name, "mem-usage-%d", i);
      thread_create (name, PRI_DEFAULT, mem_usage_thread, NULL);
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

  msg ("Memory usage test finished.");
  pass();
}

static void
mem_usage_thread (void *aux UNUSED)
{
  int i;
  void *ptr;

  thread_set_nice (10); /* Set a higher nice value for these threads. */

  for (i = 0; i < ITERATIONS; i++)
    {
      ptr = malloc (ALLOC_SIZE);
      if (ptr == NULL) {
        msg ("Failed to allocate memory in iteration %d", i);
        return;
      }
      /* Do some work with the allocated memory to simulate real usage. */
      memset(ptr, i % 256, ALLOC_SIZE);
      printf("Thread %s: iteration %d, allocated %zu bytes at %p\n", thread_name(), i, ALLOC_SIZE, ptr); // Thêm dòng này
      free (ptr);
      printf("Thread %s: iteration %d, freed memory at %p\n", thread_name(), i, ptr); // Thêm dòng này

      /* Print malloc_bytes_allocated after each iteration. */
      size_t malloc_allocated = get_malloc_bytes_allocated();
      printf("Thread %s: iteration %d, malloc_bytes_allocated = %zu\n", thread_name(), i, malloc_allocated);
    }
}
