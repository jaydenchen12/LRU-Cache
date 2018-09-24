#include <inttypes.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse_user_options.h"

USER_OPTIONS *parse_user_options(int argc, char **argv)
{
    extern char *optarg;
	extern int optind;
    int option;
    int err = 0;
    
    USER_OPTIONS *user_options = (USER_OPTIONS *)malloc(sizeof(USER_OPTIONS));
    if (user_options == NULL)
        return user_options;
    
    user_options->u_flag = 0;
    user_options->i_flag = 0;
    user_options->d_flag = 0;     
    int fflag = 0;
    
    while((option = getopt(argc, argv, "U:I:D:f:")) != -1){
        switch (option){
            case 'U': 
                user_options->u_flag = 1; /* User wants to simulate a unified cache. */
                user_options->u_cache_size = atoi(argv[optind - 1]);
                user_options->u_cache_associativity = atoi(argv[optind]);
                user_options->u_cache_line_size = atoi(argv[optind + 1]);
                break;

            case 'I':
                user_options->i_flag = 1;
                user_options->i_cache_size = atoi(argv[optind - 1]);
                user_options->i_cache_associativity = atoi(argv[optind]);
                user_options->i_cache_line_size = atoi(argv[optind + 1]);
                break;

            case 'D':
                user_options->d_flag = 1;
                user_options->d_cache_size = atoi(argv[optind - 1]);
                user_options->d_cache_associativity = atoi(argv[optind]);
                user_options->d_cache_line_size = atoi(argv[optind + 1]);
                break;

            case 'f': 
                if (fflag == 0){
                    user_options->trace_file_name = optarg;
                    fflag = 1;
                }
                else 
                    err = 1;
                break;

            case '?':
                err = 1;
                break;
        }
           
    }
       
     /* Error check the arguments. */ 
    if (!((!user_options->u_flag && user_options->i_flag && user_options->d_flag) ||
         (user_options->u_flag && !user_options->i_flag && !user_options->d_flag)))
         err = 1;
       
    if (fflag == 0)
        err = 1;

    if (err == 1)
        return NULL;
    else
        return user_options;
}


