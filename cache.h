#ifndef _CACHE_
#define _CACHE_

#include <inttypes.h>


/* Implement your data structures here */

typedef struct cache_t{
    int tag;
    int valid_bit;
    int numSetBits;
    int numOffsetBits;
    int assoc;
}CACHE;

#endif
