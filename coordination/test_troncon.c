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

    initCommonLibrary();

    initConnectionAutomate(&sockTCP);

    traceDebug(PRGM_INFO_PRINT_COLOR, "Connection with Automate OK");

    commandeAiguillage(sockTCP, addrGuest, addrDest, 51, 1);
    commandeAiguillage(sockTCP, addrGuest, addrDest, 37, 1);
    commandeTroncon(sockTCP, addrGuest, addrDest, 16, 1);
    commandeInversionTroncon(sockTCP, addrGuest, addrDest, 26, 1);
    commandeTroncon(sockTCP, addrGuest, addrDest, 6, 1);

    closeConnectionAutomate(sockTCP);

    return 0;
}