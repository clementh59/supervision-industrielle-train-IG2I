//
// Created by clement on 31/05/21.
//

#include "../common.h"
#include "tester.h"

int timeStamp_start = 0;

void *handleTrain1() {
    traceDebug(PURPLE, "Je suis le train1");
    int sock;
    initConnection(&sock);
    disAuGRMonTrain(sock, 1);
    traceDebug(PURPLE, "\tLe train 1 demande la ressource 1");
    demandeRessource("1", sock);
    traceDebug(PURPLE, "\tLe train 1 a la ressource 1");
    attendXMs(1500);
    traceDebug(PURPLE, "\tLe train 1 rend la ressource 1");
    rendRessource("1", sock);
    traceDebug(PURPLE, "\tLe train 1 n'as plus de ressource");
    closeConnection(sock);
}

void *handleTrain2() {
    traceDebug(RED, "Je suis le train2");
    int sock;
    initConnection(&sock);
    disAuGRMonTrain(sock, 2);
    calculateBeginTimeStamp(&timeStamp_start);
    attendXMs(250);
    traceDebug(RED, "\tLe train 2 demande les ressources 1 et 2");
    demandeRessource("1/2", sock);
    traceDebug(RED, "\tLe train 2 a les ressources 1 et 2");
    verifieSiLeTempsEstAcceptable("(Test simple) - Test 1", 1500, &timeStamp_start);

    attendXMs(750);
    rendRessource("1/2", sock);
    traceDebug(RED, "\tLe train 2 n'as plus de ressource");
    traceDebug(RED, "\tLe train 2 demande la ressource 2");
    demandeRessource("2", sock);
    traceDebug(RED, "\tLe train 2 a la ressource 2");
    attendXMs(750);
    rendRessource("2", sock);
    traceDebug(RED, "\tLe train 2 n'a plus de ressource");
    verifieSiLeTempsEstAcceptable("(Test simple) - Test 2", 3000, &timeStamp_start);
    closeConnection(sock);
}

int main() {
    pthread_t th[2];

    initCommonLibrary();
    pthread_create(&th[0], NULL, handleTrain1, NULL);
    pthread_create(&th[1], NULL, handleTrain2, NULL);

    for(int i=0;i<2;i++)
        pthread_join(th[i],NULL);

    return 0;
}
