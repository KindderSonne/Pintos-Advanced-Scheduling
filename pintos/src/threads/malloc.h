#ifndef THREADS_MALLOC_H
#define THREADS_MALLOC_H

#include <debug.h>
#include <stddef.h>

void malloc_init (void);
void *malloc (size_t) __attribute__ ((malloc));
void *calloc (size_t, size_t) __attribute__ ((malloc));
void *realloc (void *, size_t);
void free (void *);
size_t get_malloc_bytes_allocated(void);

#endif /* threads/malloc.h */
