#define _POSIX_C_SOURCE 199309L  // Enable POSIX.1b features

#include "logger.h"

#include <unistd.h>     // for access
#include <fcntl.h>      // for open, O_* flags
#include <sys/stat.h>   // for stat, mkdir
#include <pthread.h>    // for pthread_t and related functions
#include <stdarg.h>     // for va_list and related functions
#include <time.h>       // for time functions
#include <stdbool.h>
#include <sys/time.h>
#include <stdint.h>     // for uint64_t and related types
#include <stdio.h>      // for FILE, fopen, fwrite, fclose, fflush, perror
#include <stdlib.h>     // for EXIT_FAILURE
#include <string.h>     // for strlen, snprintf
#include <errno.h>      // for errno


#define LOG_MAX_MSG_LEN 256

static pthread_t writer = 0;
static char buffer[LOG_MAX_MSG_LEN] = { 0 };
FILE* fh = NULL;

// Retrives the current time in ms 
uint64_t time_in_microseconds() {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
        return (uint64_t)ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
    }
    return 0; // or handle the error as needed
}

int open_file(const char* filename) {
    fh = fopen(filename, "w"); // Open logger file for writing
    if(fh == NULL) {
        perror("failed to open logger file");
        return EXIT_FAILURE;
    }
    return 0;
}

int write_header(struct tm* t) {
    char s[64] = { 0 }; // hh.mm.ss\0
    strftime(s, 64, "-------------- %H:%M:%S -----------------\n", t);

    if(fwrite(s, sizeof(char), strlen(s), fh) != strlen(s)) {
        return -1;
    }

    if(fflush(fh) != 0) {
        return -1;
    }
    return 0;
}

/*
writes a log entry to file. Formats the log msg useing va_list and stores into buffer

*/
void write_line(const char* format, char prefix, const char* filename, int line,
                        const char* function, va_list vl ){
    vsnprintf(buffer, LOG_MAX_MSG_LEN, format, vl);

    time_t now;
    struct tm t;
    time(&now);
    localtime_r(&now, &t);

    char time_s[13] = { 0 }; // hh:mm:ss:mmm\0
    strftime(time_s, 13, "%H:%M:%S", &t);

    uint32_t ms = (uint32_t)((time_in_microseconds() % 1000000) / 1000);

#ifndef _DEBUG // in debug print full path. in release only print file name.
    if (strrchr(filename, '/') != NULL) {
        filename = strrchr(filename, '/') + 1;
    }
#endif
    char txt[LOG_MAX_MSG_LEN * 4];
    snprintf(txt, sizeof(txt), "[%c] %s.%03d: %s(%d): %s %s\n", prefix, time_s, ms, filename, line, function, buffer);

    if(fwrite(txt, sizeof(char), strlen(txt), fh) != strlen(txt)) {
        // handle write error
    }

#ifdef _DEBUG
    printf("%s", txt);  // For debugging, print to standard output
#endif
    fflush(fh);
}

void* writer_thread(void* arg) {
    // TODO
    return NULL;
}

// Variadic function
void log_write(const char* prefix, const char* filename, int line, const char* function, const char* format, ...) {
    if(writer != 0) {
        //TODO
    } else {
        va_list vl;
        va_start(vl, format);
        write_line(format, (prefix != NULL) ? prefix[0] : '\0', filename, line, function, vl);
        va_end(vl);
    }
}

int logger_init(const char* log_folder, const char* prefix, bool create_thread) {
    int err = 0;
    if(log_folder != NULL) {
        if(mkdir(log_folder, 0755) && errno != EEXIST) {
            return errno;
        }
        time_t now;
        struct tm t;
        time(&now);
        localtime_r(&now, &t);

        char s[11] = { 0 }; //MM.DD.YYYY\0
        strftime(s, 11, "%m.%d.%Y", &t);

        size_t l = strlen(log_folder) + strlen(s) + (strlen(prefix) + 1) + 6; // .log\0
        char filename[l];
        snprintf(filename, l, "%s/%s.%s.log", log_folder, prefix, s);

        if((err = open_file(filename)) != 0) {
            return err;
        }

        if((err = write_header(&t)) != 0) {
            return err;
        }
    }

    if(create_thread) {
        if(pthread_create(&writer, NULL, writer_thread, NULL)) {
            return errno;
        }
    }
    return err;
    }

void logger_fini() {
    if (fh != NULL) {
        fclose(fh);
        fh = NULL;
    }
}