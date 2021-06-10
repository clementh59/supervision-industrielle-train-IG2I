//
// Created by clement on 04/06/21.
//

#include "communications.h"

/**
 * Crée une socket de connection avec le gestionnaire de ressources
 * @param sockfd - le numéro de la socket qui sera crée
 * @param ip - l'ip du gestionnaire de ressources
 */
void initConnectionGR(int *sockfd, char* ip) {
#ifdef COMMUNICATE_FOR_REAL_GR
    char addr[16];
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        trace(ERROR_COLOR, "socket creation failed...\n");
        exit(0);
    }

    traceDebug(YELLOW, "Socket for GR successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));


    // je me connecte sur le gestionnaire de ressources
    addr[0] = '\0';
    printf("Je me connecte à %s\n", ip);
    strcat(addr, ip);
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(addr);
    servaddr.sin_port = htons(PORT_GR);


    // connect the client socket to server socket
    if (connect(*sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        trace(ERROR_COLOR, "connection with the server failed\n");
        exit(0);
    }

    traceDebug(YELLOW, "Connected to the GR");
#endif
}

/**
 * Crée une socket de connection avec l'automate
 * @param sockfd - le numéro de la socket qui sera crée
 */
void initConnectionAutomate(int *sockfd) {
#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    char addr[16];
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd == -1) {
        trace(ERROR_COLOR,"socket creation failed...");
        exit(0);
    }


    trace(YELLOW, "Socket for automate successfully created..");

    bzero(&servaddr, sizeof(servaddr));

    // je me connecte sur l'automate
    addr[0] = '\0';
    strcat(addr, SERVEURNAME);
    strcat(addr, NAME_AUTO);
    strcat(addr, CHAINE_CARAC_FIN);
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(addr);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(*sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        trace(ERROR_COLOR,"connection with the automate failed");
        exit(0);
    }
#endif
}

/**
 * Close la socket passée en param
 * @param sockfd - la socket à close
 */
void closeConnectionGR(int sockfd) {
#ifdef COMMUNICATE_FOR_REAL_GR
    close(sockfd);
#endif
}

/**
 * Close la socket passée en param
 * @param sockfd - la socket à close
 */
void closeConnectionAutomate(int sockfd) {
#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    close(sockfd);
#endif
}

/**
 * Lis le message sur la socket sock et l'écrit dans la variable message
 */
void lisLeMessageGR(int sock, char *message, int len) {
#ifdef COMMUNICATE_FOR_REAL_GR
    read(sock, message, len);
#endif
    afficheTrameRecuGR(message);
}

/**
 * Lis le message sur la socket sock et l'écrit dans la variable message
 */
void lisLeMessageAutomate(int sock, char *message, int len) {
#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    read(sock, message, len);
#endif
}

/**
 * Envoi le message sur la socket sock
 */
void envoieMessageGR(int sock, char *buffer) {
#ifdef COMMUNICATE_FOR_REAL_GR
    write(sock, buffer, strlen(buffer)+1);
#endif
    afficheTrameEnvoyeeGR(buffer);
}

/**
 * Envoi le message sur la socket sock
 */
void envoieMessageAutomate(int sock, char *buffer) {
#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    write(sock, buffer, strlen(buffer)+1);
#endif
}

/**
 * Envoi la trame passée en paramètre à l'automate
 * @param sockfd - la socket de dialogue
 * @param trame - la trame à envoyer
 */
void envoiLaTrame(int sockfd, trame_t *trame) {
    trame->trame[5] = trame->length - 6;
#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    write(sockfd, trame->trame, trame->length);
#endif
    afficheTrameEnvoyeeAutomate(*trame);
}

/**
 * Attend la réponse automatique de l'automate
 * @param sock - la socket de dialogue
 */
void attendLaReponseDeLAutomate(int sock, trame_t *trame) {
#ifndef COMMUNICATE_FOR_REAL_AUTOMATE
    // J'envoi une trame test pour simuler l'automate
    trame->trame[0] = 0xFE;
    trame->trame[1] = 0xFE;
    trame->trame[2] = 0xFE;
    trame->trame[3] = 0xFE;
    trame->trame[4] = 0xFE;
    trame->length = 5;
#else
    unsigned char rbuffer[MAX_XWAY_FRAME_LENGTH];
    read(sock, rbuffer, sizeof(rbuffer));
    trame->length = 0;
    for (int i = 0; i < rbuffer[5]+6 ; i++) {
        trame->trame[i] = rbuffer[i];
        trame->length++;
    }
#endif
    afficheTrameRecuAutomate(*trame);
}

/**
 * Repond à la trame reçue par un trame 5 niveaux
 * @param sock - la socket de dialogue
 * @param trame - la trame à laquelle if faut répondre
 */
void repondALaTrameRecue(int sock, trame_t *trameRecue) {
    trame_t trameAEnvoyer;

    for (int i = 0; i < 8; i ++) {
        trameAEnvoyer.trame[i] = trameRecue->trame[i];
    }

    trameAEnvoyer.trame[8] = trameRecue->trame[10]; // On inverse les addr xway
    trameAEnvoyer.trame[9] = trameRecue->trame[11];
    trameAEnvoyer.trame[10] = trameRecue->trame[8]; // On inverse les addr xway
    trameAEnvoyer.trame[11] = trameRecue->trame[9];
    trameAEnvoyer.trame[12] = 0x19; // Adressage 5 niveaux
    trameAEnvoyer.trame[13] = trameRecue->trame[13];
    trameAEnvoyer.trame[14] = 0xFE;
    trameAEnvoyer.length = 15;

    envoiLaTrame(sock, &trameAEnvoyer);
}