//
// Created by clement on 05/06/21.
//

#include "file_parser.h"

/**
 *
 * @param line_split - the variable that will contain the split line
 * @param line - the line to split
 * @return le nombre de mot extrait
 */
int splitLine(char ** line_split, char *line) {
    for (int i = 0; i < 8; i++)
        memset(line_split[i], '\0', sizeof(char) * 11);

    int index_word = 0;
    int index_char = 0;
    for (char c = 0; c < strlen(line); c++) {
        if (line[c] != ' ' && line[c] != '#') { // on est sur un caractere que l'on doit garder
            line_split[index_word][index_char++] = line[c];
        } else if (line[c] == '#') { // on arrive sur un commentaire, on arrete le traitement
            break;
        } else if (index_char > 0) { // on verifie tout de même que le mot que l'on va ajouter n'est pas vide
            index_word++;
            index_char = 0;
        }
    }

    if (index_char > 0)
        index_word++;

    return index_word;
}

/**
 *
 * @param word - exemple : "TRAIN" ou "RR"
 * @return le code correspondant à la commande
 */
int getCommandTypeFromFirstWord(char *word) {
    // Un switch n'est pas possible avec des chaines de caractères en C
    if (strcmp("TRAIN", word) == 0) {
        return TYPE_TRAIN_ID;
    }

    if (strcmp("RR", word) == 0) {
        return TYPE_REND_RESSOURCE;
    }

    if (strcmp("DR", word) == 0) {
        return TYPE_PRISE_RESSOURCE;
    }

    if (strcmp("TRONCON", word) == 0) {
        return TYPE_TRONCON;
    }

    if (strcmp("INVERSION", word) == 0) {
        return TYPE_INVERSION;
    }

    if (strcmp("AIGUILLAGE", word) == 0) {
        return TYPE_AIGUILLAGE;
    }

    return -1;
}

/**
 * Rempli la liste de commandes en fonction des infos contenues dans le fichier nommé filename
 * @param listeCommandes - la liste de commande à remplir
 * @param fileName - le fichier à lire
 */
void readFile(listeCommandes_t *listeCommandes, char * fileName) {
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char * line_split[8];
    commande_t commande;

    if( access(fileName, F_OK) != 0 ) {
        trace(ERROR_COLOR, "Le fichier passé en paramètre n'existe pas");
        exit(-1);
    }

    for (int i = 0; i < 8; i++)
        line_split[i] = malloc(11*sizeof(char));

    fp = fopen(fileName, "r");

    listeCommandes->nbCommandes = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        memset(&commande, 0, sizeof(commande_t));

        int res = splitLine(line_split, line);

        if (len > 0 && res > 0) {

            commande.type = getCommandTypeFromFirstWord(line_split[0]);

            switch (commande.type) {
                case TYPE_TRAIN_ID:
                    listeCommandes->train = atoi(line_split[1]);
                    break;
                case TYPE_AIGUILLAGE:
                    commande.code = atoi(line_split[1]);
                    listeCommandes->commandes[listeCommandes->nbCommandes] = commande;
                    listeCommandes->nbCommandes++;
                    break;
                case TYPE_TRONCON:
                    commande.code = atoi(line_split[1]);
                    listeCommandes->commandes[listeCommandes->nbCommandes] = commande;
                    listeCommandes->nbCommandes++;
                    break;
                case TYPE_INVERSION:
                    commande.code = atoi(line_split[1]);
                    listeCommandes->commandes[listeCommandes->nbCommandes] = commande;
                    listeCommandes->nbCommandes++;
                    break;
                case TYPE_PRISE_RESSOURCE:
                    for (int i = 1; i < res; i++)
                        strcpy(commande.ressources[i-1], line_split[i]);
                    commande.code = res-1;
                    listeCommandes->commandes[listeCommandes->nbCommandes] = commande;
                    listeCommandes->nbCommandes++;
                    break;
                case TYPE_REND_RESSOURCE:
                    for (int i = 1; i < res; i++)
                        strcpy(commande.ressources[i-1], line_split[i]);
                    commande.code = res-1;
                    listeCommandes->commandes[listeCommandes->nbCommandes] = commande;
                    listeCommandes->nbCommandes++;
                    break;
            }

        }
    }

    char message[50];
    sprintf(message, "Le nombre de commande dans le fichier lu est %d", listeCommandes->nbCommandes);
    traceDebug(PRGM_INFO_PRINT_COLOR, message);
    fclose(fp);
}