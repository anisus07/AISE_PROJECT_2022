#ifndef _ALLOC_DEBUGGER_H_
#define _ALLOC_DEBUGGER_H_

#define _GNU_SOURCE
#include <dlfcn.h>
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_ALLOC 20

typedef struct alloc_table_s
{
  size_t count;
  void *ptr[MAX_ALLOC];
  size_t size[MAX_ALLOC];
} alloc_table_t;

void *malloc(size_t size);
void free(void *ptr);

void affic();
#endif
