#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>

#define LOG_LEVELS 4

enum LogLevel {Debug = 0, Verbose = 1, Info = 2, Error = 3};

// Default log level to Info, if not set.
#ifndef LOGGER_LEVEL
#define LOGGER_LEVEL Info
#endif

void logger_init(FILE *fp, enum LogLevel level);
void logmsg(enum LogLevel level, const char *format, ...);
void logger_fini();


#endif