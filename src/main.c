#include "logger.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>


typedef struct {
    int verbose;
    int level;
    char *root_path;
} appargs;

char* get_home_directory() {
    char *curdir;
    curdir = realpath("./d", NULL);
    if(curdir == NULL) {
        return NULL;
    }
    return curdir;
}

void print_usage() {
    printf("Custodian. A little friend for your computer.\n");
    printf("Options:\n");
    printf("\t-v\t\t verbose output\n");
    printf("\t-d\t\t debug output (more output). \n");
    printf("\t-r <path> \t specify the root path to scan for recommendations.\n");
    printf("\t-q\t\t quiet output\n");
    printf("\t-h\t\t print this stuff.\n");
}

appargs handle_args(int argc, char **argv) {
    appargs result;

    //set defaults
    result.verbose = 0;
    result.level = LOGGER_LEVEL;
    result.root_path = get_home_directory();

    if(result.root_path == NULL){
        if (errno == EINVAL) {
            logmsg(3, "Error resolving path");
        }
    }

    int option;
    opterr = 0;

    while((option = getopt(argc, argv, "hdqvr:")) != -1) {
        switch(option) {
            case 'h':
                print_usage();
                exit(0);
                break;
            case 'v':
                result.verbose = 1;
                if(result.level > Verbose) {
                    result.level = Verbose;
                }
                break;
            case 'q':
                //quiet mode
                result.verbose = 0;
                result.level = Error;
                break;
            case 'd':
                //debug mode
                result.verbose = 1;
                result.level = Debug;
                break;
            case 'r':
                result.root_path = optarg;
                break;
            case '?':
                if(optopt == 'r') {
                    fprintf(stderr, "Option -%c requires a path argument.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "unknown option '-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
                    exit(1);
                }
                break;
            default:
                abort();
        }
    }
logmsg(0, "verbose = %d, root = %s\n", result.verbose, result.root_path);
//for(index = optind; index < argc; index++)
//printf("Non-option argument %s\n", argv[index]);
return result;

}

int main(int argc, char **argv) {
    FILE *logFile = fopen("log.txt", "w");
    if (logFile == NULL) {
        perror("failed to open logger file");
        return EXIT_FAILURE;
    }
    // Initialize the logger
    logger_init(logFile, 0);
    handle_args(argc, argv);
  
    // Log a message
    logmsg(0, "This is a debug message.");
    logmsg(1, "This is a verbose message.");
    logmsg(3, "This is a error message.");
    
    // Clean up the logger
    fclose(logFile);
    
    return 0;
}