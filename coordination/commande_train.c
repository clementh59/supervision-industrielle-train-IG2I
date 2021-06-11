//
// Created by clement on 04/06/21.
//

#include "commande_train.h"
#include "trame.h"
#include "communications.h"

#define RES_OK_COMMANDE 0xFE
#define SLEEP_ENTRE_COMMANDES 250000 // j'attend 250ms entre chaque commande

/**
 * Envoie un message à l'automate pour piloter un aiguillage
 * @param state
 * @param valeur - le code de l'aiguillage
 * @param train
 */
void commandeAiguillage(train_state_t *state, char valeur, int train) {
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

    char message[50];
    sprintf(message, "Je commande l'aiguillage %d", valeur);
    trace(BOLD_GREEN, message);

    commande(state, addrVar, valeur);
}

/**
 * Envoie un message à l'automate pour alimenter un tronçon
 * @param state
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeTroncon(train_state_t *state, char valeur, int train) {
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

    char message[50];
    sprintf(message, "Je commande le tronçon %d", valeur);
    trace(BOLD_GREEN, message);

    commande(state, addrVar, valeur);
}

/**
 * Envoie un message à l'automate pour inverser un tronçon
 * @param state
 * @param valeur - le code du tronçon
 * @param train
 */
void commandeInversionTroncon(train_state_t *state, char valeur, int train) {
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

    char message[50];
    sprintf(message, "Je commande l'inversion %d", valeur);
    trace(BOLD_GREEN, message);

#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    usleep(500000);
#endif
    commande(state, addrVar, valeur);
}

/**
 * Modifie une variable de l'automate à l'adresse indiquée et avec la valeur indiquée
 * @param train_state
 * @param addrVar
 * @param valeur
 */
void commande(train_state_t *train_state, int addrVar, char valeur) {
    unsigned char tableau[1];
    trame_t trameEnvoyee, trameRecue1, trameRecue2;

    tableau[0] = valeur;

    creeUneTrameDeCommande(&trameEnvoyee, train_state->sharedVar->socketAutomate, train_state->addrXWAY, train_state->sharedVar->addrDest, addrVar, 0x0001, tableau);

    envoiLaTrame(train_state->sharedVar, &trameEnvoyee);

    attendLaReponseDeLAutomate(train_state, &trameRecue1);

    // Je vérifie que l'automate a bien compris ma requête
    if (trameRecue1.trame[trameRecue1.length-1] != RES_OK_COMMANDE) {
        trace(ERROR_COLOR, "Je n'ai pas reçu FE de la part de l'automate");
        exit(0);
    }

    attendLaReponseDeLAutomate(train_state, &trameRecue2);

    repondALaTrameRecue(train_state->sharedVar, &trameRecue2);

#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    // Je vérifie que l'automate me renvoie bien la valeur envoyée
    if (trameRecue2.trame[trameRecue2.length-2] != valeur) {
        printf("%X \n", trameRecue2.trame[trameRecue2.length-2]);
        trace(ERROR_COLOR, "L'automate ne m'a pas renvoyée la valeur de commande envoyée");
        exit(0);
    }
#endif

#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    usleep(SLEEP_ENTRE_COMMANDES); //j'attend un certain temps entre chaque commande
#endif
}

/**
 * Envoie le nombre de tours qu'il me reste à l'automate
 * @param train_state - le state du train concerné
 * @param train - le numéro du train
 */
void envoieMonNombreDeTours(train_state_t *train_state, int train) {
    unsigned char tableau[1];
    trame_t trameEnvoyee, trameRecue1;
    int addrVar;

    tableau[0] = train_state->nb_tours;

    char message[50];
    sprintf(message, "Je préviens qu'il me reste %d tour(s) à faire", train_state->nb_tours);
    trace(BOLD_GREEN, message);

    switch (train) {
        case 1 :
            addrVar = 512;
            break;
        case 2 :
            addrVar = 513;
            break;
        case 3 :
            addrVar = 514;
            break;
        case 4 :
            addrVar = 514;
            break;
    }

    creeUneTrameDeCommande(&trameEnvoyee, train_state->sharedVar->socketAutomate, train_state->addrXWAY, train_state->sharedVar->addrDest, addrVar, 0x0001, tableau);
    envoiLaTrame(train_state->sharedVar, &trameEnvoyee);

    attendLaReponseDeLAutomate(train_state, &trameRecue1);

    // Je vérifie que l'automate a bien compris ma requête
    if (trameRecue1.trame[trameRecue1.length-1] != RES_OK_COMMANDE) {
        trace(ERROR_COLOR, "Je n'ai pas reçu FE de la part de l'automate");
        exit(0);
    }
}

/**
 * Attend que le train soit en mode run pour quitter la fonction
 * @param state_train
 */
void attendQueLeTrainSoitEnModeRUN(train_state_t *state_train) {
    // Je vérifie si on m'a demandé de stopper le train. Si oui, j'attend qu'on me demande de repartir.
    while (state_train->run == 0) {
        printf("J'attend pour continuer\n");
        usleep(100000);
    }
}