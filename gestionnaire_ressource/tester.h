//
// Created by clement on 05/06/21.
//

#ifndef L4_TESTER_H
#define L4_TESTER_H

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
#include <pthread.h>
#include <math.h>

#include "../common.h"

#define ADDR_GR "127.0.0.1"
#define PORT_GR 3300

#define SA struct sockaddr

#define CHAINE_CARAC_FIN "\0"
#define MAX_BUFF 50

/**
 *
 * @param sockfd
 * @param connfd
 * @param isAutomate - 0 si c'est sur le gestionnaire de ressource, 1 si c'est l'automate
 */
void initConnection(int *sockfd);

void closeConnection(int sockfd);

/**
 * Demander une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void demandeRessource(char * res, int sock);

/**
 * Rend une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void rendRessource(char * res, int sock);

void communiqueGestionnaireRessource(char * code, char * res, int sock);

void disAuGRMonTrain(int sock, int numeroTrain);

/**
 * Envoi le message sr la socket sock
 */
void envoieMessage(int sock, char *buffer);

/**
 * Lis le message sur la socket sock et l'écrit dans la variable message
 */
void lisLeMessage(int sock, char* message, int len);

/**
 * Met dans la variable timeStamp_start le nombre de ms actuel
 */
void calculateBeginTimeStamp(int * timeStamp_start);

/**
 * Return la comparaison entre la variable timeStamp_start et le temps actuel (return un nb en ms)
 */
int calculeLeTempsDepuisLeDebut(int * timeStamp_start);

void verifieSiLeTempsEstAcceptable(char * message, int tempsAttendu, int * timeStamp_start);

/**
 * Attend pendant X millisecondes
 * @param ms - le temps à attendre en ms
 */
void attendXMs(int ms);

#endif //L4_TESTER_H
