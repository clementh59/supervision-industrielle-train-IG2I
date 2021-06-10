//
// Created by clement on 04/06/21.
//

#ifndef L4_COMMANDE_TRAIN_H
#define L4_COMMANDE_TRAIN_H

#include "utils.h"
#include "communications.h"

/**
 * Envoie un message à l'automate pour piloter un aiguillage
 * @param state
 * @param valeur - le code de l'aiguillage
 * @param train
 */
void commandeAiguillage(train_state_t *state, char valeur, int train);

/**
 * Envoie un message à l'automate pour alimenter un tronçon
 * @param state
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeTroncon(train_state_t *state, char valeur, int train);

/**
 * Envoie un message à l'automate pour inverser un tronçon
 * @param state
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeInversionTroncon(train_state_t *state, char valeur, int train);

/**
 * Modifie une variable de l'automate à l'adresse indiquée et avec la valeur indiquée
 * @param train_state
 * @param addrVar
 * @param valeur
 */
void commande(train_state_t *train_state, int addrVar, char valeur);

#endif //L4_COMMANDE_TRAIN_H
