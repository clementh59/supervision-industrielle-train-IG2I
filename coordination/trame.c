//
// Created by clement on 04/06/21.
//

#include "trame.h"

/**
 * Initialise la trame passée en paramètre avec l'ncapsulation des paquets XWAY
 * @param trame - la variable à initialiser
 */
void initTrame(trame_t *trame) {
    trame->trame[0] = 0;
    trame->trame[1] = 0;
    trame->trame[2] = 0;
    trame->trame[3] = 1;
    trame->trame[4] = 0;
    trame->trame[5] = 0; // ce champ sera modifié lors de l'envoi de la trame
    trame->trame[6] = 0;
    trame->length = 7;
}

/**
 * Modifie un octet de la trame
 * @param trame - la trame
 * @param index - l'index du tableau de la trame à modifier
 * @param val - la nouvelle valeur
 */
void setOctetOfTrame(trame_t *trame, char index, char val) {
    trame->trame[index] = val;
}

/**
 * Ajoute l'octet passé en paramètre à la trame, et incrémente la variable length de la trame
 * @param trame
 * @param val - la valeur de l'octet
 */
void ajouteOctetToTrame(trame_t *trame, char val) {
    trame->trame[trame->length] = val;
    trame->length++;
}

/**
 * Ajoute la variable de deux octets à la trame
 * @param trame - la trame à laquelle on doit ajouter la variable
 * @param val - la variable sur deux octets à ajouter
 */
void ajouteDeuxOctetsToTrame(trame_t *trame, int val) {
    ajouteOctetToTrame(trame, val & 0x00FF);
    ajouteOctetToTrame(trame, (val & 0xFF00) >> 8);
}

/**
 *
 * @param sockfd
 */
void creeUneTrameDeCommande(trame_t *trame, int sockfd, int addrGuest, int addrDest, int addr_premier_objet, int nb_obj, char *tableau) {
    initTrame(trame);
    ajouteOctetToTrame(trame, 0xF0);
    ajouteDeuxOctetsToTrame(trame, addrGuest);
    ajouteDeuxOctetsToTrame(trame, addrDest);
    ajouteOctetToTrame(trame, 0x37); // Code requête
    ajouteOctetToTrame(trame, 0x06); // Code catégorie (0-7)

    ajouteOctetToTrame(trame, 0x68); // segment
    ajouteOctetToTrame(trame, 0x07); // type

    ajouteDeuxOctetsToTrame(trame, addr_premier_objet); // addr premier objet (ex: 0A -> %MW10 car 0A -> 10)
    ajouteDeuxOctetsToTrame(trame, nb_obj); // nb d'objet à ecrire

    for (int i = 0; i < nb_obj; i++) {
        ajouteDeuxOctetsToTrame(trame, tableau[i]);
    }
}
