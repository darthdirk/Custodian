#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <time.h>
#include <stdarg.h>

#define LOG_LEVELS 4

// values are for if > 1 log this
enum LogLevel {Debug = 0, Verbose = 1, Info = 2, Error = 3};

// Default log level to Info, if not set.
#ifndef LOGGER_LEVEL
#define Logger_LEVEL Info
#endif


int open_file(const char* filename);
int write_header(struct tm* t);
void write_line(const char* format, char prefix, const char* filename, int line,
                        const char* function, va_list vl );
void* writer_thread(void* arg);
void log_write(enum LogLevel level, const char* filename, int line, const char* function, const char* format, ...);

int logger_init(const char* log_folder, const char* level, int create_thread);
// Replacement for printf 
void logger_fini();


#endif