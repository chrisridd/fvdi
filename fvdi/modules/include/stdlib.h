#include "libkern.h"

/* From fvdi/engine/utility.c */
void *malloc(long size);
void *realloc(void *addr, long new_size);
long free(void *addr);
