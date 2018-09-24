#ifndef _PARSE_USER_OPTIONS_H_
#define _PARSE_USER_OPTIONS_H_


typedef struct user_options_t{
    unsigned int u_flag; /* Flag indicating choice of unified cache by the user. */
    unsigned int i_flag; /* Flag for I-cache. */
    unsigned int d_flag; /* Flag for D-cache. */
    unsigned int u_cache_size;
    unsigned int u_cache_associativity;
    unsigned int u_cache_line_size;
    unsigned int i_cache_size; 
    unsigned int i_cache_associativity; 
    unsigned int i_cache_line_size;
    unsigned int d_cache_size;
    unsigned int d_cache_associativity;
    unsigned int d_cache_line_size;
    char *trace_file_name;
}USER_OPTIONS;

#endif
