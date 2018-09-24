/* Simulate the performance of a cache using a reference stream of instruction, and load and store addresses. 
 * Types of caches: Unified cache or a separate I-cache and D-cache.
 * Tunable cache parameters: cache size, associativity, and cache-line size.
 * Replacement policy: Least-recently used (LRU).
 * Output: Hit (miss) rate.
 * Date created: 10/28/2017
 * Date modified: 2/27/2018 
 */

#define _STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "parse_user_options.h"
#include "cache.h"

#define DEBUG_FLAG 0

/* Functions are defined here */
void print_usage (void);
extern USER_OPTIONS* parse_user_options (int argc, char **argv);
extern CACHE *create_cache (unsigned int size, unsigned int associativity, unsigned int line_size);
void print_cache (CACHE *);
int access_cache (CACHE *cache, int *lru, int reference_type, uint64_t memory_address);
void simulate_unified_cache (CACHE *u_cache, FILE *fp);
void simulate_i_d_cache (CACHE *i_cache, CACHE *d_cache, FILE *fp);
void hexToBinary(uint64_t hex, int *outBits);
int findLRU(int* lrus, int index, int assoc);
void printLRU(int* lrus, int assoc, int index, int block);
struct address binaryToAddressFormat( int *inBits, int numSetBits, int numOffset);
/* Edit this function to compete the cache simulator. */
struct address{
     int tag;
     int index;
     int offset;
};

int access_cache (CACHE *cache,int *lru,  int reference_type, uint64_t memory_address)
{
    int hit = 0;


    if (DEBUG_FLAG)
    {
        printf ("Accesing cache for memory address %" PRIx64 ", type of reference %c \n", memory_address, reference_type);
    }
        int isCacheFull = 1;
        
       int binary[24];
    hexToBinary(memory_address, binary);
        int setBits = cache[0].numSetBits;
    struct address addr = binaryToAddressFormat( binary, setBits, cache[0].numOffsetBits);
        printf("mem = 0x%" PRIx64 "\n", memory_address);
         int assoc = cache[0].assoc;
    
        printf("tag: %d, ", addr.tag); 
        printf("index: %d, ", addr.index); 
        printf("offset: %d ", addr.offset);
        
       for ( int i = 0; i < assoc; i++){
            if (cache[addr.index*i].tag == addr.tag){
                hit = 1;
                printf("hit \n");
                for( int j = 0; j < assoc; j++){
                    lru[addr.index + assoc*(i+assoc*j)] = 1;
                }
                for( int j = 0; j < assoc; j++){ 
                    lru[addr.index + assoc*(i*assoc+j)] = 0;
                }
                printLRU(lru, assoc, addr.index, i);
            }
        }
         if (hit == 0){
        
             for ( int i = 0; i < assoc; i++){
                 if (cache[addr.index*i].tag == 0){
                     isCacheFull = 0;
                     cache[addr.index*i].tag  = addr.tag;
                    for( int j = 0; j < assoc; j++){
                         lru[addr.index + assoc*(i+assoc*j)] = 1;
                     }
                    for( int j = 0; j < assoc; j++){
                         lru[addr.index + assoc*(i*assoc+j)] = 0;
                    }
                    printLRU(lru, assoc, addr.index, i);
                    break; 
                 }
             }
         }
         if( isCacheFull == 1 ){
            int i = findLRU(lru, addr.index, assoc);
            cache[addr.index*i].tag = addr.tag;
            for( int j = 0; j < assoc; j++){
                    lru[addr.index + assoc*(i+assoc*j)] = 1;
            }
            for( int j = 0; j < assoc; j++){
                    lru[addr.index + assoc*(i*assoc+j)] = 0;
            }
            printLRU(lru, assoc, addr.index, i);
  
        }
        

    /* Parse the memory address (which is in hex) into the corresponding offset, index, and tag bits. */
    // mem address 123142
    
    /* Index into the cache set using the index bits. Check if the tag stored in the cache matches the tag 
     * correcponding to the memory address. If yes and the valid bit is set to true, then declare a 
     * cache hit. Else, declare a cache miss, and update the cache with the specified tag. If all sets are 
     * full, then use the LRU algorithm to evict the appropriate cache line. */
    
    
    return hit;
}
//loops through the LRU and the set index to print the lru and the block it is trying to access
void printLRU(int* lrus, int assoc, int index, int block){
    printf("Accesing block: %d \n", block);
    for (int i = 0; i<assoc; i++){
        for( int j = 0; j<assoc; j++){
            printf("%d \t",lrus[index+assoc*(i + j*assoc)]);
        }
        printf("\n");
    }
}
//takes in the LRU and the set index to return the index of the LRU block so access_cache() can replace it
int findLRU(int *lrus, int index, int assoc){
    int dec;
    int mult;
    int lruDec[4];
    for(int i = 0; i < assoc; i++){
        dec = 0;
        mult = 1;
        for(int j = assoc-1; j >-1; j--){
            dec += mult * lrus[index+assoc*(i + j*assoc)];
            mult *= 2; 
        }
        lruDec[i] = dec;
    }
    int min = lruDec[0];
    for( int i = 1; i < assoc; i++){
        if (lruDec[i] < min)
            min = lruDec[i]; 

    }
    //return index of lru block
    return min;
}
void hexToBinary(uint64_t hex, int *outBits){
    for (int i = 0; i < 24; i++){
        outBits[i] = (hex >> i) & 1;
    }
}
//takes a binary input and the number of bits needed to address the set and the offset
//will return a struct with the tag number, set index, and offset index access cache will use this information
struct address binaryToAddressFormat( int *inBits, int numSetBits, int numOffset){
    int dec = 0;
    int mult = 1;
    for (int i = 0; i < numOffset; i++){
        dec += mult * inBits[i];
        mult *= 2;
    }
    struct address add;
    add.offset = dec;
    dec = 0;
    mult = 1;
    for (int i = numOffset; i < numOffset + numSetBits; i++){
        dec += mult * inBits[i];
        mult *= 2;   
    }
    
    add.index = dec;
    dec = 0;
    mult = 1;
    for (int i = numOffset + numSetBits; i < 24; i++){
           dec += mult * inBits[i];
           mult *= 2;
    }   
    add.tag = dec;
    return add;
    
}
/* Simulates a unified cache for instructions and data. */
// ./simulate_cache -U 8 4 128 -f simple_trace 
void simulate_unified_cache (CACHE *u_cache, FILE *fp)
{
    int num_i_hits = 0, num_d_hits = 0;
    int num_instructions = 0, num_stores = 0, num_loads = 0;
    char reference_type;
    uint64_t memory_address;
    int status;
    int assoc = u_cache[0].assoc;
    int setNum = pow(2, u_cache[0].numSetBits);
    int *lru = malloc(setNum*(sizeof(int*)));
    for (int i = 0; i < assoc*assoc*setNum; i++){
        lru[i] = 0;
    }

    while (1){
        /* Obtain the type of reference: instruction fetch, load, or store, and the memory address. */
        status = fscanf (fp, " %c %" SCNx64, &reference_type, &memory_address);         
        if (status == EOF)
            break;

        /* Simulate the cache. */
         switch (reference_type) {
            case 'L':
                /* NO NEED TO IMPLEMENT THIS. */
                break;

             case 'S':
                /* NO NEED TO IMPLEMENT THIS. */                
                break;

             case 'I':
                num_instructions++;
                /* FIXME: IMPLEMENT CACHE FUNCTIONALITY. */
                num_i_hits += access_cache (u_cache, lru, reference_type, memory_address);
                break;

             default:
                break;
        }
    }

    /* Print some statistics. */
    printf("Total number of references to the cache: %d. \n", num_instructions + num_stores + num_loads);
    printf("Hit rate: %f. \n", (float)(num_i_hits + num_d_hits)/(float)(num_instructions + num_stores + num_loads)); 
}


int main (int argc, char **argv)
{
    FILE *input_fp;
    CACHE *u_cache = NULL; 
    // CACHE *i_cache = NULL, *d_cache = NULL;
    //

    /* Parse command line parameters. */
    USER_OPTIONS *user_options = parse_user_options (argc, argv);
    if (user_options == NULL){
        printf ("Error parsing command line arguments. \n");
        print_usage ();
        exit (0);
    }
    
    if (user_options->u_flag == 1){
        printf ("Creating unified cache; size: %dK, associativity: %d, cache line: %d bytes \n", user_options->u_cache_size, 
                                                                                                user_options->u_cache_associativity, 
                                                                                                user_options->u_cache_line_size);


         u_cache = create_cache (user_options->u_cache_size, 
                                user_options->u_cache_associativity, 
                                user_options->u_cache_line_size);


        if (u_cache == NULL){
            printf ("Error creating cache; parameters must be powers of 2. \n");
            exit (0);
        }
        if (DEBUG_FLAG) {
        print_cache (u_cache);
        }
    }
    
    printf ("Simulating the cache using trace file: %s. \n", user_options->trace_file_name);
    input_fp = fopen (user_options->trace_file_name, "r");
    if(input_fp == NULL){
        printf ("Error opening trace file. Exiting. \n");
        exit (-1);
    }
  
    if (user_options->u_flag == 1) {
        simulate_unified_cache (u_cache, input_fp);
        free ((void *)u_cache);
    }
            
    free ((void *)user_options);
    
    exit (0);
}

void print_usage (void)
{
    printf ("*****************USAGE************************ \n");
    printf ("To simulate a unified cache use the -U option: \n");
    printf ("./simulate_cache -U <cache size in Kilobytes> <set associativity> <cache line size in bytes> -f <trace file> \n");
    printf ("For example, ./simulate_cache -U 512 4 128 -f gcc_trace, simulates a 512 KB cache with \n");
    printf ("set accociativity of 4 and a cache line size of 128 bytes using the trace from gcc_trace \n \n");


    printf ("To simulate a system with separate I- and D-caches: \n");
    printf ("./simulate_cache -I <cache size> <set associativity> <cache line size> -D <cache size> <set associativity> <cache line size> -f <trace file> \n");
    printf ("For example, ./simulate_cache -I 128 1 64 -D 512 4 128 -f gcc_trace, simulates a 128 KB I cache with \n");
    printf ("set accociativity of 1 (direct mapped) and a cache line size of 64 bytes, and a 512 KB D cache with \n");
    printf ("set asscociativity of 4 and a cache line size of 128 bytes using the trace from gcc_trace \n");
}
