//
// Created by clement on 04/06/21.
//

#ifndef L4_COMMUNICATIONS_H
#define L4_COMMUNICATIONS_H

#include "utils.h"
#include "../common.h"
#include "console.h"
#include "trame.h"
#include "code.h"

/**
 * Crée une socket de connection avec le gestionnaire de ressources
 * @param sockfd - le numéro de la socket qui sera crée
 * @param ip - l'ip du gestionnaire de ressources
 */
void initConnectionGR(int *sockfd, char* ip);

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
 * Envoi le message sur la socket sock
 */
void envoieMessageGR(int sock, char *buffer);

/**
 * Envoi la trame passée en paramètre à l'automate
 * @param sharedVar
 * @param trame - la trame à envoyer
 */
void envoiLaTrame(shared_var_t *sharedVar, trame_t *trame);

/**
 * Attend la réponse de l'automate
 * @param train_state
 * @param trame - la trame qui contiendra le message lu
 * @return 1 si il s'agit d'une réponse à ma commande - 0 s'il s'agit d'une commande
 */
int  attendLaReponseDeLAutomate(train_state_t *train_state, trame_t *trame);

/**
 * Repond à la trame reçue par un trame 5 niveaux
 * @param sharedVar
 * @param trame - la trame à laquelle if faut répondre
 */
void repondALaTrameRecue(shared_var_t *sharedVar, trame_t *trameRecue);

/**
 * @param trame
 * @return 1 si la trame passée en paramètre est une trame de commande. 0 sinon.
 */
int estUneTrameDeCommande(trame_t *trame);

/**
 * Lis en continu les messages envoyés par l'automate et les redirige au bon endroit.
 * @param trains_state
 */
void lectureAutomateThread(two_train_state_t *trains_state);

#endif //L4_COMMUNICATIONS_H
