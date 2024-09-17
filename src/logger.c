#include "logger.h"
#include <stdarg.h>     // for va_list and related functions
#include <stdio.h>      // for FILE, fopen, fwrite, fclose, fflush, perror
#include <string.h>     // for strlen, snprintf

FILE *log_fp = NULL;
const char *LogMessages[LOG_LEVELS] = {"DEBUG", "VERBOSE", "", "ERROR"};
enum LogLevel m_level;

void logger_init(FILE *fp, enum LogLevel level) {
    log_fp = fp;
    m_level = level;
}

void logmsg(enum LogLevel level, const char *format, ...) {
    va_list arg;

    FILE* fp;
    // if fp is not specified log to stdout
    if(log_fp == NULL) {
        fp = stdout;
    } else {
        fp = log_fp;
    }

    if(level >= m_level) {
        if(strlen(LogMessages[level]) > 0) {
            fprintf(fp, "%s: ", LogMessages[level]);
        }
        va_start(arg, format);
        vfprintf(fp, format, arg);
        va_end(arg);
        fprintf(fp, "\n");
    }
}
