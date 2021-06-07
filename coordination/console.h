//
// Created by clement on 04/06/21.
//

#ifndef L4_CONSOLE_H
#define L4_CONSOLE_H

#include "utils.h"
#include "../common.h"

/**
 * Affiche la trame envoyée à l'automate
 * @param trame - la trame
 */
void afficheTrameEnvoyeeAutomate(trame_t trame);

/**
 * Affiche la trame reçue par l'automate
 * @param trame - la trame
 */
void afficheTrameRecuAutomate(trame_t trame);

/**
 * Affiche la trame envoyée au GR
 * @param buff - la trame
 */
void afficheTrameEnvoyeeGR(char *buff);

/**
 * Affiche la trame recue par le GR
 * @param buff - la trame
 */
void afficheTrameRecuGR(char *buff);

#endif //L4_CONSOLE_H
