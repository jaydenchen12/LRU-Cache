#define _STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include "cache.h"
#include <math.h>

CACHE *create_cache (unsigned int size, unsigned int assoc, unsigned int line_size);
int isPowerOfTwo(unsigned int i);
/* Creates and initializes the cache structure based on the size (KB), assoc, and 
 * line size (Bytes) parameters. 
 * */
CACHE *create_cache (unsigned int size, unsigned int assoc, unsigned int line_size)
{

    if (!isPowerOfTwo(size) || !isPowerOfTwo(assoc) || !isPowerOfTwo(line_size)){
        printf("one or more input not power of two. please check and try again");
        return 0;
    } 
    int numBlocks = (size * 1024)/line_size;
    int numSets = numBlocks / assoc;
    int numSetBits = log(numSets)/log(2);
    // assuming 2 byte per word
    int numOffsetBit = log(line_size/8)/log(2); 
    CACHE *cache = malloc(sizeof( CACHE) *numBlocks);
    for ( int i = 0; i < numBlocks; i++){
        cache[i].tag = 0;
        cache[i].valid_bit = 0;
        cache[i].numSetBits = numSetBits;
        cache[i].assoc = assoc;
        cache[i].numOffsetBits = numOffsetBit;
    }

    return cache;

}
    
int isPowerOfTwo(unsigned int i){
    
    if ((i & (i - 1)) == 0)
        return 1;
    else
        return 0;
}

