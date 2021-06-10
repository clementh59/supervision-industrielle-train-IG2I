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

/* Convert string s to int out.
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
str2int_errno str2int(int *out, char *s, int base) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
}