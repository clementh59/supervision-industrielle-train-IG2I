//
// Created by clement on 10/06/21.
//

#ifndef L4_PILOTAGETRAIN_H
#define L4_PILOTAGETRAIN_H

#include "utils.h"
#include "communications.h"
#include "trame.h"
#include "gestionRessources.h"
#include "commande_train.h"
#include "file_parser.h"

/**
 * Gère le pilotage d'un train (avec la communication avec le gestionnaire de ressources)
 * @param stateTrain - les paramètres du train à faire avancer
 */
void pilotageTrain(train_state_t *stateTrain);


#endif //L4_PILOTAGETRAIN_H
