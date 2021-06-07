//
// Created by clement on 04/06/21.
//

#include "console.h"

/**
 * Affiche la trame envoyée à l'automate
 * @param trame - la trame
 */
void afficheTrameEnvoyeeAutomate(trame_t trame) {
#ifdef AFFICHE_TRAME_AUTOMATE
    char message[4], buffer[70];
    memset(message, '\0', 4);
    memset(buffer, '\0', 70);

    sprintf(buffer, "\t\tENVOIE : ");

    strcat(buffer, message);
    for (short i = 0; i < trame.length; i++) {
        sprintf(message, "%X ", trame.trame[i]);
        strcat(buffer, message);
    }
    trace(COMMUNICATION_WITH_AUTOMATE_PRINT_COLOR, buffer);
#endif
}

/**
 * Affiche la trame reçue par l'automate
 * @param trame - la trame
 */
void afficheTrameRecuAutomate(trame_t trame) {
#ifdef AFFICHE_TRAME_AUTOMATE
    char message[4], buffer[70];
    memset(message, '\0', 4);
    memset(buffer, '\0', 70);

    sprintf(buffer, "\t\tRECU : ");

    strcat(buffer, message);
    for (short i = 0; i < trame.length; i++) {
        sprintf(message, "%X ", trame.trame[i]);
        strcat(buffer, message);
    }
    trace(COMMUNICATION_WITH_AUTOMATE_PRINT_COLOR, buffer);
#endif
}

/**
 * Affiche la trame envoyée au GR
 * @param buff - la trame
 */
void afficheTrameEnvoyeeGR(char *buff) {
#ifdef AFFICHE_TRAME_GR
    char message[30];
    sprintf(message, "\t\tENVOIE : %s", buff);
    trace(COMMUNICATION_WITH_GR_PRINT_COLOR, message);
#endif
}

/**
 * Affiche la trame recue par le GR
 * @param buff - la trame
 */
void afficheTrameRecuGR(char *buff) {
#ifdef AFFICHE_TRAME_GR
    char message[30];
    sprintf(message, "\t\tRECU : %s", buff);
    trace(COMMUNICATION_WITH_GR_PRINT_COLOR, message);
#endif
}