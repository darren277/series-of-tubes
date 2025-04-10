#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define LOG_REQUEST(fmt, ...) printf("\033[1;34m[REQUEST] " fmt "\033[0m\n", ##__VA_ARGS__)
#define LOG_RESPONSE(fmt, ...) printf("\033[1;32m[RESPONSE] " fmt "\033[0m\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) fprintf(stderr, "\033[1;31m[ERROR] " fmt "\033[0m\n", ##__VA_ARGS__)

#endif // LOGGING_H 