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

    stateTrain->nb_tours = 0;
    stateTrain->run = 0;
    stateTrain->readHasBeenTriggerred = 0;

    initConnectionGR(&sockTCP_GR, stateTrain->sharedVar->addrGR);

    readFile(&listeCommandes, stateTrain->fileName);

    disAuGRMonTrain(sockTCP_GR, listeCommandes.train);

    while (1) {
        // Tant qu'on me dit de run et que le nombre de tours restant est supérieur à 0, je run
        while (stateTrain->run == 1 && stateTrain->nb_tours > 0) {

            for (int i = 0; i < listeCommandes.nbCommandes; i++) {

                attendQueLeTrainSoitEnModeRUN(stateTrain);

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

            stateTrain->nb_tours--;
            envoieMonNombreDeTours(stateTrain, listeCommandes.train);

        }

        usleep(100000);
    }

    closeConnectionGR(sockTCP_GR);
}
