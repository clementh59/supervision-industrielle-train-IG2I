//
// Created by clement on 10/06/21.
//

#include "pilotageTrain.h"

/**
 * Gère le pilotage d'un train (avec la communication avec le gestionnaire de ressources)
 * @param stateTrain - les paramètres du train à faire avancer
 */
void pilotageTrain(train_state_t *stateTrain) {

    int sockTCP_GR;
    char buffer[20];
    listeCommandes_t listeCommandes;

    initConnectionGR(&sockTCP_GR, stateTrain->sharedVar->addrGR);

    readFile(&listeCommandes, stateTrain->fileName);

    disAuGRMonTrain(sockTCP_GR, listeCommandes.train);

    while(1) {

        for (int i = 0; i < listeCommandes.nbCommandes; i++) {

            commande_t commande = listeCommandes.commandes[i];

            switch (commande.type) {
                case TYPE_AIGUILLAGE:
                    commandeAiguillage(stateTrain, commande.code, listeCommandes.train);
                    break;
                case TYPE_INVERSION:
                    commandeInversionTroncon(stateTrain, commande.code, listeCommandes.train);
                    break;
                case TYPE_TRONCON:
                    commandeTroncon(stateTrain, commande.code, listeCommandes.train);
                    break;
                case TYPE_REND_RESSOURCE:
                    for (int c = 0; c < commande.code; c++) {
                        buffer[c * 2] = commande.ressources[c];
                        if (c != commande.code - 1)
                            buffer[c * 2 + 1] = '/';
                        else
                            buffer[c * 2 + 1] = '\0';
                    }
                    rendRessource(buffer, sockTCP_GR);
                    break;
                case TYPE_PRISE_RESSOURCE:
                    for (int c = 0; c < commande.code; c++) {
                        buffer[c * 2] = commande.ressources[c];
                        if (c != commande.code - 1)
                            buffer[c * 2 + 1] = '/';
                        else
                            buffer[c * 2 + 1] = '\0';
                    }
                    demandeRessource(buffer, sockTCP_GR);
                    break;
            }
        }

    }

    closeConnectionGR(sockTCP_GR);
}
