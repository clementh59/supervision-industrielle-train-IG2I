//
// Created by clement on 04/06/21.
//

#ifndef L4_COMMANDE_TRAIN_H
#define L4_COMMANDE_TRAIN_H

#include "utils.h"
#include "communications.h"

/**
 * Envoie un message à l'automate pour piloter un aiguillage
 * @param sock - la socket de dialogue
 * @param valeur - le code de l'aiguillage
 * @param train
 */
void commandeAiguillage(int sock, int addrGuest, int addrDest, char valeur, int train);

/**
 * Envoie un message à l'automate pour alimenter un tronçon
 * @param sock - la socket de dialogue
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeTroncon(int sock, int addrGuest, int addrDest, char valeur, int train);

/**
 * Envoie un message à l'automate pour inverser un tronçon
 * @param sock - la socket de dialogue
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeInversionTroncon(int sock, int addrGuest, int addrDest, char valeur, int train);

/**
 * Modifie une variable de l'automate à l'adresse indiquée et avec la valeur indiquée
 * @param sock
 * @param addrGuest
 * @param addrDest
 * @param addrVar
 * @param valeur
 */
void commande(int sock, int addrGuest, int addrDest, int addrVar, char valeur);

#endif //L4_COMMANDE_TRAIN_H
