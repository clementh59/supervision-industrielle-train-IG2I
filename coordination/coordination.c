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
int addrGuest = 0x8000;

int main(int argc, char ** argv) {
    int sockTCP; // automate
    int sockTCP_GR; // Gestionnaire ressources
    char buffer[20];
    listeCommandes_t listeCommandes;


    initCommonLibrary();

    if (argc != 4) {
        trace(ERROR_COLOR, "Il faut fournir les bons paramètres en entrée (ex : ./coordination train1.txt MY_XWAY_ADDR GESTIONNAIRE_RESSOURCE_IP)");
        exit(-1);
    }

    int tempo;
    str2int(&tempo, argv[2],10);
    addrGuest = addrGuest | tempo;

#ifndef COMMUNICATE_FOR_REAL_GR
    trace(ERROR_COLOR, "Attention, la communication avec le GR est desactivée");
#endif
#ifndef COMMUNICATE_FOR_REAL_AUTOMATE
    trace(ERROR_COLOR, "Attention, la communication avec l'automate est desactivée");
#endif

    //todo: récupérer my_addr_xway

    initConnectionAutomate(&sockTCP);
    initConnectionGR(&sockTCP_GR, argv[3]);

    traceDebug(PRGM_INFO_PRINT_COLOR, "Connection with Automate and GR OK");

    readFile(&listeCommandes, argv[1]);

    disAuGRMonTrain(sockTCP_GR, listeCommandes.train);

    while(1) {

        for (int i = 0; i < listeCommandes.nbCommandes; i++) {

            commande_t commande = listeCommandes.commandes[i];

            switch (commande.type) {
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

    closeConnectionAutomate(sockTCP);
    closeConnectionGR(sockTCP_GR);

    return 0;
}