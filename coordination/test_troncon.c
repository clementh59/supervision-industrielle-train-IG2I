//
// Created by clement on 04/06/21.
//

#include "utils.h"
#include "communications.h"
#include "trame.h"
#include "gestionRessources.h"
#include "commande_train.h"
#include "file_parser.h"
#include <pthread.h>

int addrDest = 0x8017;
int addrGuest = 0x801B;

int main() {
    int sockTCP; // automate
    sem_t mutex;
    pthread_t thread;

    initCommonLibrary();

    initConnectionAutomate(&sockTCP);

    traceDebug(PRGM_INFO_PRINT_COLOR, "Connection with Automate OK");

    train_state_t train_stateA, train_stateB;
    two_train_state_t trains_state;
    shared_var_t sharedVar;

    sharedVar.socketAutomate = sockTCP;
    sharedVar.addrDest = addrDest;
    sem_init(&mutex,1,1);
    sharedVar.mutexEcritureAutomate = &mutex;
    train_stateA.sharedVar = &sharedVar;
    train_stateA.readHasBeenTriggerred = 0;
    trains_state.trainA = &train_stateA;
    trains_state.trainB = &train_stateB;
    train_stateA.addrXWAY = addrGuest;
    train_stateA.trameRecue = malloc(sizeof(trame_t));

    pthread_create(&thread, NULL, (pf_void) lectureAutomateThread, &trains_state);
    usleep(100000);

    /*commandeAiguillage(sockTCP, addrGuest, addrDest, 37, 1);
    commandeTroncon(sockTCP, addrGuest, addrDest, 16, 1);
    commandeInversionTroncon(sockTCP, addrGuest, addrDest, 26, 1);
    commandeTroncon(sockTCP, addrGuest, addrDest, 6, 1);*/

    closeConnectionAutomate(sockTCP);

    return 0;
}