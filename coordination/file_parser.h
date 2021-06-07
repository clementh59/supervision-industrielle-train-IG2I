//
// Created by clement on 05/06/21.
//

#ifndef L4_FILE_PARSER_H
#define L4_FILE_PARSER_H

#include "utils.h"
#include "../common.h"

/**
 *
 * @param line_split - the variable that will contain the split line
 * @param line - the line to split
 * @return le nombre de mot extrait
 */
int splitLine(char ** line_split, char *line);

/**
 *
 * @param word - exemple : "TRAIN" ou "RR"
 * @return le code correspondant à la commande
 */
int getCommandTypeFromFirstWord(char *word);

/**
 * Rempli la liste de commandes en fonction des infos contenues dans le fichier nommé filename
 * @param listeCommandes - la liste de commande à remplir
 * @param fileName - le fichier à lire
 */
void readFile(listeCommandes_t *listeCommandes, char * fileName);

#endif //L4_FILE_PARSER_H
