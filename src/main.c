#include "logger.h"

int main() {
    // Initialize the logger
    logger_init("/home/dirk/Custodian/LOG", "test", 0);
    
    // Log a message
    log_write(Info, __FILE__, __LINE__, __func__, "Hello, world!");
    
    // Clean up the logger
    logger_fini();
    
    return 0;
}