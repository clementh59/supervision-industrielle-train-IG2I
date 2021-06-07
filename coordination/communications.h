//
// Created by clement on 04/06/21.
//

#ifndef L4_COMMUNICATIONS_H
#define L4_COMMUNICATIONS_H

#include "utils.h"
#include "../common.h"
#include "console.h"

/**
 * Crée une socket de connection avec le gestionnaire de ressources
 * @param sockfd - le numéro de la socket qui sera crée
 */
void initConnectionGR(int *sockfd);

/**
 * Crée une socket de connection avec l'automate
 * @param sockfd - le numéro de la socket qui sera crée
 */
void initConnectionAutomate(int *sockfd);

/**
 * Close la socket passée en param
 * @param sockfd - la socket à close
 */
void closeConnectionGR(int sockfd);

/**
 * Close la socket passée en param
 * @param sockfd - la socket à close
 */
void closeConnectionAutomate(int sockfd);


/**
 * Lis le message sur la socket sock et l'écrit dans la variable message
 */
void lisLeMessageGR(int sock, char* message, int len);

/**
 * Lis le message sur la socket sock et l'écrit dans la variable message
 */
void lisLeMessageAutomate(int sock, char* message, int len);

/**
 * Envoi le message sur la socket sock
 */
void envoieMessageGR(int sock, char *buffer);

/**
 * Envoi le message sur la socket sock
 */
void envoieMessageAutomate(int sock, char *buffer);


/**
 * Envoi la trame passée en paramètre à l'automate
 * @param sockfd - la socket de dialogue
 * @param trame - la trame à envoyer
 */
void envoiLaTrame(int sockfd, trame_t *trame);

/**
 * Attend la réponse automatique de l'automate
 * @param sock - la socket de dialogue
 */
void attendLaReponseDeLAutomate(int sock, trame_t *trame);

/**
 * Repond à la trame reçue
 * @param sock - la socket de dialogue
 * @param trame - la trame à laquelle if faut répondre
 */
void repondALaTrameRecue(int sock, trame_t *trameRecue);

#endif //L4_COMMUNICATIONS_H
