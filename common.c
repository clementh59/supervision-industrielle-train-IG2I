//
// Created by clement on 04/06/21.
//

#include "common.h"

sem_t mutexAffichage;

void initCommonLibrary() {
    sem_init(&mutexAffichage,1,1);
    traceDebug(YELLOW, "initCommonLibrary() done");
}

void trace(char * color, char *message) {

    if (strcmp(color, INVISIBLE)!=0) {
        // retrieve the actual time and put it in a string
        int millisec;
        struct tm *tm_info;
        struct timeval tv;
        char str_time[26];
        gettimeofday(&tv, NULL);
        millisec = lrint(tv.tv_usec / 1000.0); // Round to nearest millisec
        if (millisec >= 1000) { // Allow for rounding up to nearest second
            millisec -= 1000;
            tv.tv_sec++;
        }
        tm_info = localtime(&tv.tv_sec);
        strftime(str_time, 26, "%Y:%m:%d %H:%M:%S", tm_info);


        // wait for the mutex to be able to print something
        sem_wait(&mutexAffichage);

        if (strcmp(ERROR_COLOR, color) == 0 || strcmp(BOLD_GREEN, color) == 0)
            setTerminalColor(color);
        else
            setTerminalColor(WHITE);

        printf("%s.%03d - ", str_time, millisec);

        setTerminalColor(color);
        printf("%s\n", message);

        // release the mutex
        sem_post(&mutexAffichage);
    }
}

void traceDebug(char * color, char *message) {
#ifdef DEBUG
    trace(color, message);
#endif
}

void setTerminalColor(char * color) {
    printf("%s",color);
}