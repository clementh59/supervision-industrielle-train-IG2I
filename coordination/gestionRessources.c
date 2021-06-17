//
// Created by clement on 04/06/21.
//

#include "gestionRessources.h"
#include "communications.h"

/**
 * Envoi une commande au gestionnaire de ressources (demande ou rend une/des ressources)
 * @param code - le code de commande ("RR" ou "DR")
 * @param res - la ou les ressources demandées
 * @param sock - la socket de dialogue
 */
void communiqueGestionnaireRessource(char * code, char * res, int sock, int train) {
    // Je commence par calculer le nombre de ressources necessaires
    int m;
    int charcount;

    charcount = 0;
    for(m=0; res[m]; m++) {
        if(res[m] == '/') {
            charcount ++;
        }
    }

    charcount ++;

    unsigned char buff[50], rbuff[50];
    sprintf(buff,"%s:%d:%s", code, charcount, res);
#ifdef COMMUNICATE_FOR_REAL_GR
    envoieMessageGR(sock, buff, train);
    lisLeMessageGR(sock, rbuff, sizeof(rbuff), train);
    if (rbuff[0] != 'A') { // the first letter should be 'A' for "ACK"
        trace(ERROR_COLOR, "Le gestionnaire de ressource m'a envoyé un NACK");
    }
#endif
}

/**
 * Demander une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void demandeRessource(char * res, int sock, int train) {
    communiqueGestionnaireRessource("DR",res,sock, train);
}

/**
 * Rend une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void rendRessource(char * res, int sock, int train) {
    communiqueGestionnaireRessource("RR",res,sock, train);
}

/**
 * Envoi mon numéro de train au gestionnaire de ressources
 * @param sock
 */
void disAuGRMonTrain(int sock, int numeroTrain) {
    traceDebug(PRGM_INFO_PRINT_COLOR, "Je dis au GR mon train");
    char buff[50], rbuff[50];
    sprintf(buff,"T:%d", numeroTrain);
#ifdef COMMUNICATE_FOR_REAL_GR
    envoieMessageGR(sock, buff, numeroTrain);
    lisLeMessageGR(sock, rbuff, sizeof(rbuff), numeroTrain);
    afficheTrameRecuGR(rbuff, numeroTrain);
#endif
}