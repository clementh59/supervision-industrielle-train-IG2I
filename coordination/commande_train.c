//
// Created by clement on 04/06/21.
//

#include "commande_train.h"
#include "trame.h"
#include "communications.h"

#define RES_OK_COMMANDE 0xFE

/**
 * Envoie un message à l'automate pour piloter un aiguillage
 * @param sock - la socket de dialogue
 * @param valeur - le code de l'aiguillage
 * @param train
 */
void commandeAiguillage(int sock, int addrGuest, int addrDest, char valeur, int train) {
    int addrVar;
    switch (train) {
        case 1 :
            addrVar = 502;
            break;
        case 2 :
            addrVar = 505;
            break;
        case 3 :
            addrVar = 508;
            break;
        case 4 :
            addrVar = 511;
            break;

    }
    commande(sock, addrGuest, addrDest, addrVar, valeur);
}

/**
 * Envoie un message à l'automate pour alimenter un tronçon
 * @param sock - la socket de dialogue
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeTroncon(int sock, int addrGuest, int addrDest, char valeur, int train) {
    int addrVar;
    switch (train) {
        case 1 :
            addrVar = 500;
            break;
        case 2 :
            addrVar = 503;
            break;
        case 3 :
            addrVar = 506;
            break;
        case 4 :
            addrVar = 509;
            break;

    }
    commande(sock, addrGuest, addrDest, addrVar, valeur);
}

/**
 * Envoie un message à l'automate pour inverser un tronçon
 * @param sock - la socket de dialogue
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeInversionTroncon(int sock, int addrGuest, int addrDest, char valeur, int train) {
    int addrVar;
    switch (train) {
        case 1 :
            addrVar = 501;
            break;
        case 2 :
            addrVar = 504;
            break;
        case 3 :
            addrVar = 507;
            break;
        case 4 :
            addrVar = 510;
            break;

    }
    commande(sock, addrGuest, addrDest, addrVar, valeur);
}

/**
 * Modifie une variable de l'automate à l'adresse indiquée et avec la valeur indiquée
 * @param sock
 * @param addrGuest
 * @param addrDest
 * @param addrVar
 * @param valeur
 * @param train
 */
void commande(int sock, int addrGuest, int addrDest, int addrVar, char valeur) {
    unsigned char tableau[1];
    trame_t trameEnvoyee, trameRecue1, trameRecue2;

    tableau[0] = valeur;

    creeUneTrameDeCommande(&trameEnvoyee, sock, addrGuest, addrDest, addrVar, 0x0001, tableau);
    envoiLaTrame(sock, &trameEnvoyee);
    attendLaReponseDeLAutomate(sock, &trameRecue1);
    // todo: check if I received RES_OK_COMMANDE
    attendLaReponseDeLAutomate(sock, &trameRecue2);
    repondALaTrameRecue(sock, &trameRecue2);
}