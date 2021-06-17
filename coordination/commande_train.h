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

/**
 * Envoie le nombre de tours qu'il me reste à l'automate
 * @param train_state - le state du train concerné
 * @param train - le numéro du train
 */
void envoieMonNombreDeTours(train_state_t *train_state, int train);

/**
 * Attend que le train soit en mode run pour quitter la fonction
 * @param state_train
 */
void attendQueLeTrainSoitEnModeRUN(train_state_t *state_train);

/**
 * Envoie un premier message à l'automate pour lui dire bonjour (j'écris le numéro du train sur une variable)
 * @param state_train - le state du train concerné
 * @param train - le uméro du train
 */
void disBonjourALautomate(train_state_t *state_train, int train);

#endif //L4_COMMANDE_TRAIN_H
