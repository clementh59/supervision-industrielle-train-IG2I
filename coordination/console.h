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
 * @param train - l'id du train
 */
void afficheTrameEnvoyeeGR(char *buff, int train);

/**
 * Affiche la trame recue par le GR
 * @param buff - la trame
 * @param train - l'id du train
 */
void afficheTrameRecuGR(char *buff, int train);

#endif //L4_CONSOLE_H
