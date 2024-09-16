#pragma once
#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#define log_info(format, ...) log_write('I', __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define log_warn(format, ...) log_write('W', __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)
#define log_error(format, ...) log_write('E', __FILE__, __LINE__, __FUNCTION__, format, ##__VA_ARGS__)

#define print_error(msg, ...) { printf(msg, ##__VA_ARGS__); printf("\n"); log_error(msg, ##__VA_ARGS__); }
#define print_warn(msg, ...) { printf(msg, ##__VA_ARGS__); printf("\n"); log_warn(msg, ##__VA_ARGS__); }
#define print_info(msg, ...) { printf(msg, ##__VA_ARGS__); printf("\n"); log_info(msg, ##__VA_ARGS__); }


int open_file(const char* filename);
int write_header(struct tm* t);
void write_line(const char* format, char prefix, const char* filename, int line,
                const char* function, va_list vl );
void* writer_thread(void* arg);
void log_write(const char* prefix, const char* filename, int line, const char* function, const char* format, ...);
int logger_init(const char* log_folder, const char* prefix, bool thread);
// Replacement for printf 
void logger_fini();


#endif