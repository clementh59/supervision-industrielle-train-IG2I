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

#define COMMUNICATE_FOR_REAL_GR
#define COMMUNICATE_FOR_REAL_AUTOMATE
#define AFFICHE_TRAME_GR
#define AFFICHE_TRAME_AUTOMATE

#define ADDR_GR "127.0.0.1"
#define PORT_GR 3300

#define PORT 502
#define SA struct sockaddr

#define SERVEURNAME "10.22.205."
#define NAME_AUTO "203"
#define CHAINE_CARAC_FIN "\0"
#define MAX_XWAY_FRAME_LENGTH 80
#define MAX_RESSOURCES_PAR_COMMANDE 4
#define NB_COMMANDE_MAX 100

#define COMMUNICATION_WITH_GR_PRINT_COLOR CYAN
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
    unsigned char ressources[MAX_RESSOURCES_PAR_COMMANDE];
} commande_t;

typedef struct {
    int train;
    commande_t commandes[NB_COMMANDE_MAX];
    int nbCommandes;
} listeCommandes_t;

#endif //L4_UTILS_H