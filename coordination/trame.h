//
// Created by clement on 04/06/21.
//

#ifndef L4_TRAME_H
#define L4_TRAME_H

#include "utils.h"

/**
 * Initialise la trame passée en paramètre avec l'ncapsulation des paquets XWAY
 * @param trame - la variable à initialiser
 */
void initTrame(trame_t *trame);

/**
 * Modifie un octet de la trame
 * @param trame - la trame
 * @param index - l'index du tableau de la trame à modifier
 * @param val - la nouvelle valeur
 */
void setOctetOfTrame(trame_t *trame, char index, char val);

/**
 * Ajoute l'octet passé en paramètre à la trame, et incrémente la variable length de la trame
 * @param trame
 * @param val - la valeur de l'octet
 */
void ajouteOctetToTrame(trame_t *trame, char val);

/**
 * Ajoute la variable de deux octets à la trame
 * @param trame - la trame à laquelle on doit ajouter la variable
 * @param val - la variable sur deux octets à ajouter
 */
void ajouteDeuxOctetsToTrame(trame_t *trame, int val);

void creeUneTrameDeCommande(trame_t *trame, int sockfd, int addrGuest, int addrDest, int addr_premier_objet, int nb_obj, char *tableau);

#endif //L4_TRAME_H
