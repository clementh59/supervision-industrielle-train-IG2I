//
// Created by clement on 04/06/21.
//

#ifndef L4_GESTIONRESSOURCES_H
#define L4_GESTIONRESSOURCES_H

/**
 * Envoi une commande au gestionnaire de ressources (demande ou rend une/des ressources)
 * @param code - le code de commande ("RR" ou "DR")
 * @param res - la ou les ressources demandées
 * @param sock - la socket de dialogue
 */
void communiqueGestionnaireRessource(char * code, char * res, int sock, int train);

/**
 * Demander une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void demandeRessource(char * res, int sock, int train);

/**
 * Rend une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void rendRessource(char * res, int sock, int train);

/**
 * Envoi mon numéro de train au gestionnaire de ressources
 * @param sock
 */
void disAuGRMonTrain(int sock, int numeroTrain);

#endif //L4_GESTIONRESSOURCES_H
