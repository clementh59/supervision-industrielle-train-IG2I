//
// Created by Clement on 01/06/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#include "../common.h"

#define CHECK(sts, msg)				\
	if ((sts) == -1) {				\
		perror(msg);				\
		exit(EXIT_FAILURE);			\
	}

#define PORT 3300
#define NB_TRAIN 4
#define NB_RESSOURCE 12
#define SIZE_BUFFER 70
#define SIZE_RBUFFER 50
#define SIZE_T1 2
#define SIZE_T2 5
#define PRINTER_LENGTH 100

typedef void * (*pf_void)(void *);

sem_t mutexPR;
sem_t *mutexR;

/**
 * Attend 0.5ms
 */
void attendUnePeriode() {
    usleep(500000);
}

/**
 * Return a color corresponding to the train passed in param
 * @param train - the train id
 * @return a string corresponding to the color
 */
char * getColorFromTrain(int train) {
    switch(train){
        case 1:
            return PURPLE;
        case 2 :
            return RED;
        case 3 :
            return YELLOW;
        case 4 :
            return CYAN;
    }
}

/**
 * Reserve la ressource demandée
 * @param {int} num - le numéro de la ressource
 * @param {int} train - le numéro du train
 */
void reserveLaRessource(int num, int train) {

    char printer[PRINTER_LENGTH]; // variable qui sert pour l'affichage dans la console

    if (num > 0 && num < NB_RESSOURCE+1) {
        sprintf(printer, "La ressource %d est demandée par %d", num, train);
        trace(getColorFromTrain(train), printer);
        sem_wait(&mutexR[num]);
        sprintf(printer, "La ressource %d est obtenue par %d", num, train);
        trace(getColorFromTrain(train), printer);
        sleep(1);
    }
}

/**
 * Rend la ressource demandée
 * @param {int} num - le numéro du train
 */
void rendLaRessource(int num, int train) {

    char printer[PRINTER_LENGTH]; // variable qui sert pour l'affichage dans la console

    if (num > 0 && num < NB_RESSOURCE+1) {
        sprintf(printer, "La ressource %d est rendu par %d", num, train);
        trace(getColorFromTrain(train), printer);
        sem_post(&mutexR[num]);
    }
}

/**
 *
 * @param num - le num de la ressource
 * @return 1 si la ressource est dispo - 0 sinon
 */
int regardeSiLaRessourceEstDisponible(int num) {

    int value;

    if (num > 0 && num < NB_RESSOURCE+1) {
        sem_getvalue(&mutexR[num], &value);
        return value;
    }

    return 0;
}

/**
 * Reserve les ressources passées en paramètres
 * @param nbRessources - le nombre de ressources à rendre
 * @param ressourcesDemandees - un tableau contenant les numéro de ressources à reserver
 */
void reserveLesRessources(int nbRessources, int *ressourcesDemandees, int train) {

    char printer[PRINTER_LENGTH]; // variable qui sert pour l'affichage dans la console


    if (nbRessources > 1) {

        int ressources_dispo = 0;

        // tant que toutes les ressources ne sont pas disponibles, je n'en prend aucune
        while (!ressources_dispo) {
            ressources_dispo = 1;
            // je bloque la prise de ressources pour être sur que personne ne prenne une ressource que je pensais disponible
            sem_wait(&mutexPR);
            for (int i = 0; i < nbRessources; i++) {
                if (regardeSiLaRessourceEstDisponible(ressourcesDemandees[i]) == 0) {
                    ressources_dispo = 0;
                    memset(printer, '\0', sizeof(char) * PRINTER_LENGTH);
                    sprintf(printer, "La ressource %d n'est pas dispo", ressourcesDemandees[i]);
                    traceDebug(getColorFromTrain(train), printer);
                }
            }
            if (ressources_dispo) {
                traceDebug(getColorFromTrain(train), "Les ressources sont toutes disponibles, je les prends");
                for (int i = 0; i < nbRessources; i++)
                    reserveLaRessource(ressourcesDemandees[i], train);
            }

            sem_post(&mutexPR);
            attendUnePeriode();
        }

    } else {
        reserveLaRessource(ressourcesDemandees[0], train);
    }

}

/**
 * Rend les ressources passées en paramètres
 * @param nbRessources - le nombre de ressources à rendre
 * @param ressourcesDemandees - un tableau contenant les numéro de ressources à rendre
 */
void rendLesRessources(int nbRessources, int *ressourcesDemandees, int train) {

    for (int i = 0; i < nbRessources; i++)
        rendLaRessource(ressourcesDemandees[i], train);

}

/**
 * Gère la communication avec le train selon le protocole définie dans README.md
 * @param sd - la socket d'écoute
 * @param num - le numéro du train
 */
void communicationTrain(int *sd, int num) {
    char buffer[SIZE_BUFFER], rbuffer[SIZE_RBUFFER],traitementD[SIZE_T1],traitementR[SIZE_T2], requete[SIZE_RBUFFER];
    int nbCarac, nbRessourcesDemandees;
    int ressourcesDemandees[4];
    char printer[PRINTER_LENGTH]; // variable qui sert pour l'affichage dans la console

    sprintf(printer, "Hello train %d", num);
    trace(BLUE, printer);

    do{

        nbCarac=read(*sd,rbuffer,sizeof(rbuffer));

        strcpy(requete, rbuffer);

        if (nbCarac > 0) {

            // Je commence par récupérer le nombre de ressources demandées, ainsi que ce que je dois faire avec les
            // ressources et les numéro ressources concernées.
            sscanf(rbuffer, "%c%c:%d:%s", &traitementD[0], &traitementD[1], &nbRessourcesDemandees, traitementR);

            // je récupère ensuite les numéros de ressources dans la chaine de caractère concernée.
            switch (nbRessourcesDemandees) {
                case 1:
                    sscanf(traitementR, "%d", &ressourcesDemandees[0]);
                    break;
                case 2:
                    sscanf(traitementR, "%d/%d", &ressourcesDemandees[0], &ressourcesDemandees[1]);
                    break;
                case 3:
                    sscanf(traitementR, "%d/%d/%d", &ressourcesDemandees[0], &ressourcesDemandees[1],
                           &ressourcesDemandees[2]);
                    break;
                case 4:
                    sscanf(traitementR, "%d/%d/%d/%d", &ressourcesDemandees[0], &ressourcesDemandees[1],
                           &ressourcesDemandees[2], &ressourcesDemandees[3]);
                    break;
            }

            if (traitementD[0] == 'R' && traitementD[1] == 'R') { // Le train rend des ressources
                rendLesRessources(nbRessourcesDemandees, ressourcesDemandees, num);
                sprintf(buffer, "ACK=%s", requete);
            } else if (traitementD[0] == 'D' && traitementD[1] == 'R') { // le train prend des ressources
                reserveLesRessources(nbRessourcesDemandees, ressourcesDemandees, num);
                sprintf(buffer, "ACK=%s", requete);
            } else {
                memset(printer, '\0', sizeof(char)*PRINTER_LENGTH );
                sprintf(printer, "La trame reçu par le train %d ne respecte pas le protocole défini", num);
                trace(ERROR_COLOR, printer);
                sprintf(buffer, "NACK=%s", requete);
            }
            write(*sd, buffer, strlen(buffer)+1);
        }
    }
    while(nbCarac>0);

    memset(printer, '\0', sizeof(char)*PRINTER_LENGTH );
    sprintf(printer, "Bye train %d", num);
    trace(BLUE, printer);
}

/**
 * Ce programme écoute la socket pour avoir le numéro du train connecté à cette socket.
 * Une fois le numéro de train obtenu, il lance la fonction
 * @param sd - la socket d'écoute
 */
void initialisationTrain(int *sd) {
    char buffer[SIZE_BUFFER], rbuffer[SIZE_RBUFFER], requete[SIZE_RBUFFER], printer[PRINTER_LENGTH];
    int numeroTrain = 0;

    read(*sd,rbuffer,sizeof(rbuffer));
    strcpy(requete, rbuffer);
    sscanf(rbuffer, "T:%d", &numeroTrain);
    memset(buffer, '\0', sizeof(char)*SIZE_BUFFER );
    sprintf(buffer, "ACK=%s", requete);
    write(*sd, buffer, strlen(buffer)+1);
    communicationTrain(sd, numeroTrain);
}

/**
 * Cette fonction crée le serveur sur lequel les trains vont venir se connecter
 * Il est ensuite à l'écoute des connections sur sa socket et crée un thread pour gérer chaque nouvelle connection
 */
void serveur(int *se, int *sd) {
    struct sockaddr_in mon_address, client_address;
    int mon_address_longueur, lg;
    int numero_Client = 0;
    unsigned char buffer[512];
    pthread_t *th;
    int *idT;

    idT = malloc(NB_TRAIN* sizeof(int));
    th = malloc(NB_TRAIN* sizeof(pthread_t));
    mutexR = malloc(NB_RESSOURCE*sizeof(sem_t));

    sem_init(&mutexPR,1,1);

    for(short i=0;i<NB_RESSOURCE;i++)
        sem_init(&mutexR[i],1,1);

    bzero(&mon_address,sizeof(mon_address));
    mon_address.sin_port = htons(PORT);
    mon_address.sin_family = AF_INET;
    mon_address.sin_addr.s_addr = htonl(INADDR_ANY);

    /* creation de socket */

    if ((*se = socket(AF_INET,SOCK_STREAM,0))== -1) {
        printf("socket creation failed\n");
        exit(0);
    }

    /* bind serveur - socket */
    bind(*se,(struct sockaddr *)&mon_address,sizeof(mon_address));

    /* ecoute sur la socket */
    listen(*se,5);

    /* accept la connexion */
    mon_address_longueur = sizeof(client_address);

    while(1) {
        sd[numero_Client] = accept(*se,
                               (struct sockaddr *)&client_address,
                               &mon_address_longueur);

        CHECK(pthread_create(th+numero_Client, NULL, (pf_void) initialisationTrain, &sd[numero_Client]), "ERROR CHECK");

        numero_Client++;
    }
}

int main() {

    int se,sd[10];

    initCommonLibrary();

    serveur(&se,sd);

    return 0;
}