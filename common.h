//
// Created by clement on 04/06/21.
//

#ifndef L4_COMMON_H
#define L4_COMMON_H

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#define RED "\033[0;31m"
#define ERROR_COLOR "\033[1;31m"
#define BLACK "\033[0;30m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define YELLOW "\033[0;33m"
#define BOLD_YELLOW "\033[1;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define BOLD_PURPLE "\033[1;35m"
#define CYAN "\033[0;36m"
#define BOLD_CYAN "\033[1;36m"
#define WHITE "\033[0;37m"
#define INVISIBLE "INVISIBLE"

#define DEBUG

typedef void * (*pf_void)(void *);

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;

/**
 * This function needs to be called to init the library
 */
void initCommonLibrary();

/** Convert string s to int out.
 *
 * @param[out] out The converted int. Cannot be NULL.
 *
 * @param[in] s Input string to be converted.
 *
 *     The format is the same as strtol,
 *     except that the following are inconvertible:
 *
 *     - empty string
 *     - leading whitespace
 *     - any trailing characters that are not part of the number
 *
 *     Cannot be NULL.
 *
 * @param[in] base Base to interpret string in. Same range as strtol (2 to 36).
 *
 * @return Indicates if the operation succeeded, or why it failed.
 */
str2int_errno str2int(int *out, char *s, int base);

/**
 * Print the message in the console
 * @param color - the color of the message
 * @param message - the message
 */
void trace(char * color, char *message);

/**
 * Call trace only if DEBUG is defined
 * @param color
 * @param message
 */
void traceDebug(char * color, char *message);

/**
 * Change the color of the terminal cursor
 * @param color
 */
void setTerminalColor(char * color);

/**
 * Return a color corresponding to the train passed in param
 * @param train - the train id
 * @return a string corresponding to the color
 */
char * getBoldColorFromTrain(int train);

#endif //L4_COMMON_H
