/* mlfqs-heavy-load.c
 *
 * A more demanding test for MLFQS in Pintos.
 *
 * Creates many threads with different priorities, nice values, and behaviors
 * (CPU-bound, I/O-bound). Runs for a longer time to stress the scheduler.
 */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

#define THREAD_CNT 50           // Total number of threads
#define RUN_TIME 60            // Run time in seconds
#define CPU_BOUND_THREADS 20    // Number of CPU-bound threads
#define IO_BOUND_THREADS 20     // Number of I/O-bound threads
#define MIXED_THREADS 10        // Number of threads with mixed behavior
#define PRINT_INTERVAL 5        // Interval for printing load average

static int64_t start_time;

static void cpu_bound_thread (void *aux);
static void io_bound_thread (void *aux);
static void mixed_thread (void *aux);

void
test_mlfqs_heavy_load (void) 
{
  int i;
  
  ASSERT (thread_mlfqs);

  start_time = timer_ticks ();

  msg ("Starting heavy load test for MLFQS...");

  // Create CPU-bound threads
  for (i = 0; i < CPU_BOUND_THREADS; i++) {
    char name[30];
    int nice_value = (i % 5) - 2; // Nice values from -2 to 2
    snprintf(name, sizeof name, "cpu_bound_%d", i);
    thread_create (name, PRI_DEFAULT + i % 5, cpu_bound_thread, (void*)(intptr_t)nice_value); 
  }

  // Create I/O-bound threads
  for (i = 0; i < IO_BOUND_THREADS; i++) {
    char name[30];
    int nice_value = (i % 5) + 3; // Nice values from 3 to 7
    snprintf(name, sizeof name, "io_bound_%d", i);
    thread_create (name, PRI_DEFAULT - i % 5, io_bound_thread, (void*)(intptr_t)nice_value);
  }

  // Create mixed-behavior threads
  for (i = 0; i < MIXED_THREADS; i++) {
    char name[30];
    int nice_value = (i % 10) - 5; // Nice values from -5 to 4
    snprintf(name, sizeof name, "mixed_%d", i);
    thread_create (name, PRI_DEFAULT, mixed_thread,(void*)(intptr_t)nice_value);
  }

  // Print load average periodically
  for (i = 0; i < RUN_TIME / PRINT_INTERVAL; i++) {
    int64_t sleep_until = start_time + TIMER_FREQ * (i + 1) * PRINT_INTERVAL;
    timer_sleep (sleep_until - timer_ticks ());
    int load_avg = thread_get_load_avg ();
    msg ("After %d seconds, load average=%d.%02d.",
         (i + 1) * PRINT_INTERVAL, load_avg / 100, load_avg % 100);
  }

  msg("Heavy load test finished.");
  pass();
}

static void
cpu_bound_thread (void *arg) 
{
  int nice_value = (int)(intptr_t)arg;
  thread_set_nice (nice_value);

  while (timer_elapsed (start_time) < RUN_TIME * TIMER_FREQ) {
    // Spin in a tight loop to simulate CPU-bound work
    for (int i = 0; i < 1000000; i++); 
  }
}

static void
io_bound_thread (void *arg) 
{
  int nice_value = (int)(intptr_t)arg;
  thread_set_nice (nice_value);

  while (timer_elapsed (start_time) < RUN_TIME * TIMER_FREQ) {
    timer_sleep (5); // Simulate I/O operation
    // Do some short computation
    for(int i = 0; i < 100000; i++);
  }
}

static void
mixed_thread (void *arg) 
{
  int nice_value = (int)(intptr_t)arg;
  thread_set_nice (nice_value);

  while (timer_elapsed (start_time) < RUN_TIME * TIMER_FREQ) {
    // Alternate between CPU-bound and I/O-bound work
    if (timer_ticks() % (TIMER_FREQ * 2) == 0){
        timer_sleep(10);
    } else {
        for (int i = 0; i < 500000; i++);
    }
  }
}