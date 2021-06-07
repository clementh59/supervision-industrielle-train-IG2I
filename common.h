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

#define RED "\033[0;31m"
#define ERROR_COLOR "\033[1;31m"
#define BLACK "\033[0;30m"
#define GREEN "\033[0;32m"
#define BOLD_GREEN "\033[1;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define INVISIBLE "INVISIBLE"

#define DEBUG

void initCommonLibrary();

void trace(char * color, char *message);

void traceDebug(char * color, char *message);

void setTerminalColor(char * color);

#endif //L4_COMMON_H
