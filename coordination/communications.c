//
// Created by clement on 04/06/21.
//

#include "communications.h"

/**
 * Crée une socket de connection avec le gestionnaire de ressources
 * @param sockfd - le numéro de la socket qui sera crée
 * @param ip - l'ip du gestionnaire de ressources
 */
void initConnectionGR(int *sockfd, char *ip) {
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
 * Envoi le message sur la socket sock
 */
void envoieMessageGR(int sock, char *buffer) {
#ifdef COMMUNICATE_FOR_REAL_GR
    write(sock, buffer, strlen(buffer)+1);
#endif
    afficheTrameEnvoyeeGR(buffer);
}

/**
 * Envoi la trame passée en paramètre à l'automate
 * @param sharedVar
 * @param trame - la trame à envoyer
 */
void envoiLaTrame(shared_var_t *sharedVar, trame_t *trame) {
    trame->trame[5] = trame->length - 6;
#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
    sem_wait(sharedVar->mutexEcritureAutomate);
    write(sharedVar->socketAutomate, trame->trame, trame->length);
    sem_post(sharedVar->mutexEcritureAutomate);
#endif
    afficheTrameEnvoyeeAutomate(*trame);
}

/**
 * Attend la réponse de l'automate
 * @param train_state
 * @param trame - la trame qui contiendra le message lu
 * @return 1 si il s'agit d'une réponse à ma commande - 0 s'il s'agit d'une commande
 */
int attendLaReponseDeLAutomate(train_state_t *train_state, trame_t *trame) {
    while (train_state->readHasBeenTriggerred == 0) {
        usleep(50); // j'attend 50 micro s
    }

    train_state->readHasBeenTriggerred = 0; // je clear le flag

    copieTrame(trame, train_state->trameRecue);
    train_state->trameRecue->length = 0; // je reinitialise la trame

    afficheTrameRecuAutomate(*trame);
}

/**
 * Repond à la trame reçue par un trame 5 niveaux
 * @param sharedVar
 * @param trame - la trame à laquelle if faut répondre
 */
void repondALaTrameRecue(shared_var_t *sharedVar, trame_t *trameRecue) {
    trame_t trameAEnvoyer;

    for (int i = 0; i < 8; i++) {
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

    envoiLaTrame(sharedVar, &trameAEnvoyer);
}

/**
 * @param trame
 * @return 1 si la trame passée en paramètre est une trame de commande. 0 sinon.
 */
int estUneTrameDeCommande(trame_t *trame) {
    if (trame->length <= 22)
        return 0;

    if (trame->trame[22] == CODE_NB_TOURS || trame->trame[22] == CODE_RUN || trame->trame[22] == CODE_STOP)
        return 1;

    return 0;
}

// Je crée des fonctions de simulations si je ne dois pas communiquer pour de vrai avec l'automate
#ifndef COMMUNICATE_FOR_REAL_AUTOMATE
int trameActuelle = 0;

/**
 * Utile pour la simulation.
 * @param trame
 */
void creeUneTrameDACKEnvoyeeParLautomate(trame_t *trame) {
    // trame FE
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 1);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 7);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 240);
    ajouteOctetToTrame(trame, 23);
    ajouteOctetToTrame(trame, 128);
    ajouteOctetToTrame(trame, 27);
    ajouteOctetToTrame(trame, 128);
    ajouteOctetToTrame(trame, 80);
    ajouteOctetToTrame(trame, 254);
}

/**
 * Crée une trame envoyee par l'automate (la trame contient un mot de valeur `value`). Utile pour la simulation.
 * @param trame - la trame qui contiendra la trame crée par a fonction
 * @param value
 */
void creeUneTrameDEnvoyeeParLautomate(trame_t *trame, char value) {
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 1);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 0x12);
    ajouteOctetToTrame(trame, 0);
    ajouteOctetToTrame(trame, 0xF1);
    ajouteOctetToTrame(trame, 0x17);
    ajouteOctetToTrame(trame, 0x80);
    ajouteOctetToTrame(trame, 0x1B);
    ajouteOctetToTrame(trame, 0x80);
    ajouteOctetToTrame(trame, 9);
    ajouteOctetToTrame(trame, 0x4D);
    ajouteOctetToTrame(trame, 0x37);
    ajouteOctetToTrame(trame, 0x07);
    ajouteOctetToTrame(trame, 0x68);
    ajouteOctetToTrame(trame, 0x07);
    ajouteOctetToTrame(trame, 0x01);
    ajouteOctetToTrame(trame, 0x00);
    ajouteOctetToTrame(trame, 0x01);
    ajouteOctetToTrame(trame, 0x00);
    ajouteOctetToTrame(trame, value);
    ajouteOctetToTrame(trame, 0x00);
}

/**
 * Simule une séquence de trame reçu
 * @param trame
 */
void getReadTrame(trame_t *trame) {

    trame->length = 0;

    switch (trameActuelle) {
        case 0:
            ajouteOctetToTrame(trame, 0);
            ajouteOctetToTrame(trame, 0);
            ajouteOctetToTrame(trame, 0);
            ajouteOctetToTrame(trame, 1);
            ajouteOctetToTrame(trame, 0);
            ajouteOctetToTrame(trame, 0x14);
            ajouteOctetToTrame(trame, 0);
            ajouteOctetToTrame(trame, 0xF1);
            ajouteOctetToTrame(trame, 0x17);
            ajouteOctetToTrame(trame, 0x80);
            ajouteOctetToTrame(trame, 0x1B);
            ajouteOctetToTrame(trame, 0x80);
            ajouteOctetToTrame(trame, 9);
            ajouteOctetToTrame(trame, 0x4D);
            ajouteOctetToTrame(trame, 0x37);
            ajouteOctetToTrame(trame, 0x07);
            ajouteOctetToTrame(trame, 0x68);
            ajouteOctetToTrame(trame, 0x07);
            ajouteOctetToTrame(trame, 0x01);
            ajouteOctetToTrame(trame, 0x00);
            ajouteOctetToTrame(trame, 0x01);
            ajouteOctetToTrame(trame, 0x00);
            ajouteOctetToTrame(trame, CODE_NB_TOURS);
            ajouteOctetToTrame(trame, 0x00);
            ajouteOctetToTrame(trame, 2);
            ajouteOctetToTrame(trame, 0x00);
            break;
        case 1:
            // trame CMD RUN
            creeUneTrameDEnvoyeeParLautomate(trame, CODE_RUN);
            break;
        case 2:
            creeUneTrameDACKEnvoyeeParLautomate(trame);
            break;
        case 3:
            creeUneTrameDEnvoyeeParLautomate(trame, 50);
            break;
        case 4:
            creeUneTrameDACKEnvoyeeParLautomate(trame);
            break;
        case 5:
            creeUneTrameDEnvoyeeParLautomate(trame, 32);
            break;
        case 6:
            creeUneTrameDACKEnvoyeeParLautomate(trame);
            break;
        case 7:
            creeUneTrameDEnvoyeeParLautomate(trame, 32);
            break;
        case 8:
            creeUneTrameDACKEnvoyeeParLautomate(trame);
            break;
        case 9:
            creeUneTrameDEnvoyeeParLautomate(trame, 32);
            break;
        case 10:
            creeUneTrameDACKEnvoyeeParLautomate(trame);
            break;
        case 11:
            creeUneTrameDEnvoyeeParLautomate(trame, 32);
            break;
        case 12:
            creeUneTrameDACKEnvoyeeParLautomate(trame);
            break;
        case 13:
            creeUneTrameDEnvoyeeParLautomate(trame, 32);
            break;
        case 14:
            creeUneTrameDEnvoyeeParLautomate(trame, CODE_STOP);
            break;
        case 15:
            sleep(5);
            creeUneTrameDEnvoyeeParLautomate(trame, CODE_RUN);
            break;
    }

    if (trame->length == 0) {
        if (trameActuelle < 43) {
            if (trameActuelle % 2 == 0)
                creeUneTrameDACKEnvoyeeParLautomate(trame);
            else
                creeUneTrameDEnvoyeeParLautomate(trame, 32);
        } else {
            if (trameActuelle % 2 != 0)
                creeUneTrameDACKEnvoyeeParLautomate(trame);
            else
                creeUneTrameDEnvoyeeParLautomate(trame, 32);
        }
    }

    trameActuelle++;
}

#endif

/**
 * Lis en continu les messages envoyés par l'automate et les redirige au bon endroit.
 * @param trains_state
 */
void lectureAutomateThread(two_train_state_t *trains_state) {
    while (1) {
#ifdef COMMUNICATE_FOR_REAL_AUTOMATE
        trame_t trame;
        unsigned char rbuffer[MAX_XWAY_FRAME_LENGTH];
        read(trains_state->trainA->sharedVar->socketAutomate, rbuffer, sizeof(rbuffer));
        trame.length = 0;
        for (int i = 0; i < rbuffer[5]+6 ; i++) {
            trame.trame[i] = rbuffer[i];
            trame.length++;
        }
#else
        usleep(500000);
        trame_t trame;
        unsigned char rbuffer[MAX_XWAY_FRAME_LENGTH];
        getReadTrame(&trame);
#endif

        int xwayAddr = getXWAYAddrFromReceivedFrame(trame);
        train_state_t *train;

        if (xwayAddr == trains_state->trainA->addrXWAY) {
            train = trains_state->trainA;
        } else if (xwayAddr == trains_state->trainB->addrXWAY) {
            train = trains_state->trainB;
        } else {
            trace(ERROR_COLOR, "L'automate a envoyé un message a une adresse XWAY inconnue");
            exit(-1);
        }

        if (estUneTrameDeCommande(&trame)) {
            switch (trame.trame[22]) {
                case CODE_STOP:
                    trace(PRGM_INFO_PRINT_COLOR, "J'ai recu un message qui me dit de STOP");
                    train->run = 0;
                    // Je dois répondre un FE
                    repondALaTrameRecue(train->sharedVar, &trame);
                    break;
                case CODE_RUN:
                    trace(PRGM_INFO_PRINT_COLOR, "J'ai recu un message qui me dit de RUN");
                    train->run = 1;
                    // Je dois répondre un FE
                    repondALaTrameRecue(train->sharedVar, &trame);
                    break;
                case CODE_NB_TOURS:
                    trace(PRGM_INFO_PRINT_COLOR, "J'ai recu un message qui me dit de changer le nombre de tour");
                    train->nb_tours = trame.trame[24];
                    // Je dois répondre un FE
                    repondALaTrameRecue(train->sharedVar, &trame);
                    break;
            }
        } else {
            copieTrame(train->trameRecue, &trame);
            train->readHasBeenTriggerred = 1;
        }
    }
}