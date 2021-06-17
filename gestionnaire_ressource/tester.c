//
// Created by clement on 05/06/21.
//

#include "tester.h"

/**
 * Met dans la variable timeStamp_start le nombre de ms actuel
 */
void calculateBeginTimeStamp(int * timeStamp_start) {
    int millisec;
    struct timeval tv;
    struct tm *tm_info;

    gettimeofday(&tv, NULL);
    millisec = lrint(tv.tv_usec / 1000.0); // Round to nearest millisec
    if (millisec >= 1000) { // Allow for rounding up to nearest second
        millisec -= 1000;
        tv.tv_sec++;
    }

    tm_info = localtime(&tv.tv_sec);

    *timeStamp_start = tm_info->tm_sec*1000 + millisec;
}

/**
 * Return la comparaison entre la variable timeStamp_start et le temps actuel (return un nb en ms)
 */
int calculeLeTempsDepuisLeDebut(int * timeStamp_start) {
    int millisec;
    struct timeval tv;
    struct tm *tm_info;

    gettimeofday(&tv, NULL);
    millisec = lrint(tv.tv_usec / 1000.0); // Round to nearest millisec
    if (millisec >= 1000) { // Allow for rounding up to nearest second
        millisec -= 1000;
        tv.tv_sec++;
    }

    tm_info = localtime(&tv.tv_sec);

    int timeStamp = tm_info->tm_sec*1000 + millisec;

    return timeStamp - *timeStamp_start;
}

void verifieSiLeTempsEstAcceptable(char * message, int tempsAttendu, int * timeStamp_start) {
    int temps = calculeLeTempsDepuisLeDebut(timeStamp_start);
    char printer[80]; // variable qui sert pour l'affichage dans la console

    if (temps > (tempsAttendu)*1.25 || temps < tempsAttendu*0.25) {
        sprintf(printer, "%s failed (%d / %d)", message, temps, tempsAttendu);
        trace(ERROR_COLOR, printer);
    }
    else{
        sprintf(printer, "%s succeeded (%d / %d)", message, temps, tempsAttendu);
        trace(BOLD_GREEN, printer);
    }
}

/**
 *
 * @param sockfd
 * @param connfd
 * @param isAutomate - 0 si c'est sur le gestionnaire de ressource, 1 si c'est l'automate
 */
void initConnection(int *sockfd) {
    char addr[16];
    struct sockaddr_in servaddr, cli;

    // socket create and verification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    // je me connecte sur le gestionnaire de ressources
    addr[0] = '\0';
    strcat(addr, ADDR_GR);
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(addr);
    servaddr.sin_port = htons(PORT_GR);

    // connect the client socket to server socket
    if (connect(*sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed\n");
        exit(0);
    }
}

void closeConnection(int sockfd) {
    close(sockfd);
}

/**
 * Lis le message sur la socket sock et l'écrit dans la variable message
 */
void lisLeMessage(int sock, char* message, int len){
    if (read(sock, message, len) < 0) {
        trace(ERROR_COLOR, "Il y a eu une erreur lors de la lecture de la socket");
        exit(-1);
    }
}

/**
 * Envoi le message sr la socket sock
 */
void envoieMessage(int sock, char *buffer) {
    write(sock, buffer, strlen(buffer)+1);
}

void communiqueGestionnaireRessource(char * code, char * res, int sock) {
    int m, charcount;

    // Je commence par calculer le nombre de ressources necessaires
    charcount = 0;
    for(m=0; res[m]; m++) {
        if(res[m] == '/') {
            charcount ++;
        }
    }

    charcount ++;

    unsigned char buff[MAX_BUFF];
    unsigned char rbuff[MAX_BUFF];
    sprintf(buff,"%s:%d:%s", code, charcount, res);
    envoieMessage(sock, buff);
    lisLeMessage(sock, rbuff, sizeof(rbuff));
}

/**
 * Demander une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void demandeRessource(char * res, int sock) {
    communiqueGestionnaireRessource("DR",res,sock);
}

/**
 * Rend une ressource au gestionnaire de ressource
 * @param res - le numéro de la ressource à demander (ex : 1/2 pour demander les res 1 et 2)
 */
void rendRessource(char * res, int sock) {
    communiqueGestionnaireRessource("RR",res,sock);
}

void disAuGRMonTrain(int sock, int numeroTrain) {
    char buff[50];
    sprintf(buff,"T:%d", numeroTrain);
    envoieMessage(sock, buff);
    lisLeMessage(sock, buff, sizeof(buff));
}

/**
 * Attend pendant X millisecondes
 * @param ms - le temps à attendre en ms
 */
void attendXMs(int ms) {
    usleep(1000*ms);
}