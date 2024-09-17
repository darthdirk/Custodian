#include "logger.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>




int main() {

    FILE *logFile = fopen("log.txt", "w");
    if (logFile == NULL) {
        perror("failed to open logger file");
        return EXIT_FAILURE;
    }
    // Initialize the logger
    logger_init(logFile, 0);
    
    // Log a message
    logmsg(0, "This is a debug message.");
    logmsg(1, "This is a verbose message.");
    logmsg(3, "This is a error message.");
    
    // Clean up the logger
    fclose(logFile);
    
    return 0;
}