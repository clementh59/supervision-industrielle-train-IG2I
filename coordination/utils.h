//
// Created by clement on 04/06/21.
//

#ifndef L4_UTILS_H
#define L4_UTILS_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/wait.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <semaphore.h>

#define COMMUNICATE_FOR_REAL_GR
#define COMMUNICATE_FOR_REAL_AUTOMATE
#define AFFICHE_TRAME_GR
// #define AFFICHE_TRAME_AUTOMATE

#define PORT_GR 3300

#define PORT 502
#define SA struct sockaddr

#define SERVEURNAME "10.22.205."
#define NAME_AUTO "203"
#define CHAINE_CARAC_FIN "\0"
#define MAX_XWAY_FRAME_LENGTH 80
#define MAX_RESSOURCES_PAR_COMMANDE 4 // le nombre max de ressources prises ou rendues simultanément
#define NB_COMMANDE_MAX 100 // Le nombre de commande max dans un fichier lu

#define COMMUNICATION_WITH_AUTOMATE_PRINT_COLOR RED
#define PRGM_INFO_PRINT_COLOR YELLOW

typedef struct {
    unsigned int length;
    unsigned char trame[MAX_XWAY_FRAME_LENGTH];
} trame_t;

#define TYPE_AIGUILLAGE 0
#define TYPE_TRONCON 1
#define TYPE_INVERSION 2
#define TYPE_PRISE_RESSOURCE 3
#define TYPE_REND_RESSOURCE 4
#define TYPE_TRAIN_ID 5

typedef struct {
    int type;
    int code;
    char ressources[MAX_RESSOURCES_PAR_COMMANDE][2];
} commande_t;

typedef struct {
    int train;
    commande_t commandes[NB_COMMANDE_MAX];
    int nbCommandes;
} listeCommandes_t;

typedef struct {
    char addrGR[20];
    int socketAutomate;
    int addrDest;
    sem_t *mutexEcritureAutomate;
} shared_var_t;

typedef struct {
    char fileName[20];
    int addrXWAY;
    trame_t *trameRecue;
    char readHasBeenTriggerred;
    shared_var_t * sharedVar;
    int nb_tours;
    char run; // run if 1, stop if 0
} train_state_t;

typedef struct {
    train_state_t* trainA;
    train_state_t* trainB;
} two_train_state_t;

#endif //L4_UTILS_H