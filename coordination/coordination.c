//
// Created by clement on 04/06/21.
//

#include <pthread.h>

#include "utils.h"
#include "communications.h"
#include "trame.h"
#include "gestionRessources.h"
#include "commande_train.h"
#include "file_parser.h"
#include "pilotageTrain.h"

int addrDest = 0x8017;
int addrGuest = 0x8000;

int main(int argc, char ** argv) {
    int sockTCP; // automate
    pthread_t thread1, thread2, thread3;
    int xway_addr_a_tempo, xway_addr_b_tempo;
    sem_t mutexEcritureAutomate;

    shared_var_t sharedVar;
    train_state_t stateTrainA, stateTrainB;
    two_train_state_t trains_state;

    initCommonLibrary();

    if (argc != 6) {
        trace(ERROR_COLOR, "Il faut fournir les bons paramètres en entrée (ex : ./coordination train1.txt train3.txt XWAY_ADDR_TRAIN_A XWAY_ADDR_TRAIN_B GESTIONNAIRE_RESSOURCE_IP)");
        exit(-1);
    }


#ifndef COMMUNICATE_FOR_REAL_GR
    trace(ERROR_COLOR, "Attention, la communication avec le GR est desactivée");
#endif
#ifndef COMMUNICATE_FOR_REAL_AUTOMATE
    trace(ERROR_COLOR, "Attention, la communication avec l'automate est desactivée");
#endif

    initConnectionAutomate(&sockTCP);
    traceDebug(PRGM_INFO_PRINT_COLOR, "Connection with Automate OK");

    // Je récupère les 2 adresses xway passées en paramètre du programme.
    str2int(&xway_addr_a_tempo, argv[3], 10);
    str2int(&xway_addr_b_tempo, argv[4], 10);

    // J'initialise la variable de config du train A
    strcpy(stateTrainA.fileName, argv[1]);
    stateTrainA.addrXWAY = addrGuest | xway_addr_a_tempo;
    stateTrainA.sharedVar = &sharedVar;
    stateTrainA.readHasBeenTriggerred = 0;
    stateTrainA.trameRecue = malloc(sizeof(trame_t));

    // J'initialise la variable de config du train B
    strcpy(stateTrainB.fileName, argv[2]);
    stateTrainB.addrXWAY = addrGuest | xway_addr_b_tempo;
    stateTrainB.sharedVar = &sharedVar;
    stateTrainB.readHasBeenTriggerred = 0;
    stateTrainB.trameRecue = malloc(sizeof(trame_t));

    // J'initialise la shared Var
    sharedVar.socketAutomate = sockTCP;
    sharedVar.addrDest = addrDest;
    sem_init(&mutexEcritureAutomate,1,1);
    sharedVar.mutexEcritureAutomate = &mutexEcritureAutomate;
    strcpy(sharedVar.addrGR, argv[5]);

    // J'initialise la variable qui servira pour le thread de lecture
    trains_state.trainA = &stateTrainA;
    trains_state.trainB = &stateTrainB;

    // Je crée un thread pour le pilotage du train A
    pthread_create(&thread1, NULL, (pf_void) pilotageTrain, &stateTrainA);
    // Je crée un thread pour le pilotage du train B
    pthread_create(&thread2, NULL, (pf_void) pilotageTrain, &stateTrainB);

    // Je crée un thread pour la lecture des messages de l'automate
    pthread_create(&thread3, NULL, (pf_void) lectureAutomateThread, &trains_state);

    // Le programme run en boucle
    while (1);

    closeConnectionAutomate(sockTCP);

    return 0;
}