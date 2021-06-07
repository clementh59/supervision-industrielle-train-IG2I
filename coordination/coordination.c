//
// Created by clement on 04/06/21.
//

#include "utils.h"
#include "communications.h"
#include "trame.h"
#include "gestionRessources.h"
#include "commande_train.h"
#include "file_parser.h"

int addrDest = 0x8017;
int addrGuest = 0x801B;

int main() {
    int sockTCP; // automate
    int sockTCP_GR; // Gestionnaire ressources
    char buffer[20];

    initCommonLibrary();

#ifndef COMMUNICATE_FOR_REAL_GR
    trace(ERROR_COLOR, "Attention, la communication avec le GR est desactivée");
#endif
#ifndef COMMUNICATE_FOR_REAL_AUTOMATE
    trace(ERROR_COLOR, "Attention, la communication avec l'automate est desactivée");
#endif

    initConnectionAutomate(&sockTCP);
    initConnectionGR(&sockTCP_GR);

    traceDebug(PRGM_INFO_PRINT_COLOR, "Connection with Automate and GR OK");

    //commandeTroncon(sockTCP, addrGuest, addrDest, 16, 1);
    //commandeAiguillage(sockTCP, addrGuest, addrDest, 31, 1);

    listeCommandes_t listeCommandes;

    readFile(&listeCommandes, "train1.txt");

    disAuGRMonTrain(sockTCP_GR, listeCommandes.train);

    for (int i = 0; i < listeCommandes.nbCommandes; i++) {

        commande_t commande = listeCommandes.commandes[i];

        switch(commande.type) {
            case TYPE_AIGUILLAGE:
                commandeAiguillage(sockTCP, addrGuest, addrDest, commande.code, listeCommandes.train);
                break;
            case TYPE_INVERSION:
                commandeInversionTroncon(sockTCP, addrGuest, addrDest, commande.code, listeCommandes.train);
                break;
            case TYPE_TRONCON:
                commandeTroncon(sockTCP, addrGuest, addrDest, commande.code, listeCommandes.train);
                break;
            case TYPE_REND_RESSOURCE:
                for (int c = 0; c < commande.code; c++) {
                    buffer[c*2] = commande.ressources[c];
                    if (c!=commande.code-1)
                        buffer[c*2+1] = '/';
                    else
                        buffer[c*2+1] = '\0';
                }
                rendRessource(buffer, sockTCP_GR);
                break;
            case TYPE_PRISE_RESSOURCE:
                for (int c = 0; c < commande.code; c++) {
                    buffer[c*2] = commande.ressources[c];
                    if (c!=commande.code-1)
                        buffer[c*2+1] = '/';
                    else
                        buffer[c*2+1] = '\0';
                }
                demandeRessource(buffer, sockTCP_GR);
                break;
        }
    }

    closeConnectionAutomate(sockTCP);
    closeConnectionGR(sockTCP_GR);

    return 0;
}