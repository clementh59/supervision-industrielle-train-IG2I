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


#define ADDR_GR "10.22.205.103"
#define PORT_GR 3300

#define PORT 502
#define SA struct sockaddr

#define SERVEURNAME "10.22.205."
#define NAME_AUTO "201"
#define CHAINE_CARAC_FIN "\0"
#define SORTIR 5
#define MISE_EM 1
#define MISE_EA 2
#define SEND_TO 3
#define READ_TO 4
#define MAX_BUFF 50

int addrGuest = 0x801B;
int addrDest = 0x8015;

typedef struct {  // 1 octet
    unsigned char Type; //&0x04
    unsigned char Syze_Level; //&0x02
    unsigned char Refus; //&0x01
    unsigned char Extension; //&0x01
} Xway_typeNPDU;

typedef struct {  // 2 octets
    unsigned char num_Station; //&0xFF
    unsigned char num_Net; //&0x0F) << 4
    unsigned char num_Port; //&0x0F
} Xway_Addr_Type;

typedef struct { // 4 octets
    Xway_Addr_Type addr_Src;
    Xway_Addr_Type addr_Dest;
} Xway_Addr;

typedef struct {
    unsigned char code_Param; //&0xFF
    unsigned char val_Param[7]; //&0xFF
    unsigned char nParam;
} Xway_Param;

typedef struct {
    Xway_typeNPDU NPDU;
    Xway_Addr addr;
    Xway_Param *addr3_5;
    Xway_Param *addr6;
    unsigned char *donnee;
} Xway_Frame;

typedef struct {
    unsigned char id_Modbus[5]; //{0x0,0x0,0x0,0x1,0x0}
    unsigned char syze;
    Xway_Frame donnee;
} Encaps_ModBus;

typedef enum {
    TRUE = 1,
    FALSE = 0
} BOOL;

void initConnection(int *sockfd, int *connfd, int isAutomate);

void closeConnection(int sockfd);

void demandeRessource(char * res, int sock);

void rendRessource(char * res, int sock);

void communiqueGestionnaireRessource(char * code, char * res, int sock);

void disAuGRMonTrain(int sock);

void viderBufferE() {
    int c = 0;
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

Xway_typeNPDU createNPDU(short type, short syze_level, short refus, short Extension) {
    Xway_typeNPDU returnValue;

    returnValue.Type = type & 0x04;
    returnValue.Syze_Level = type & 0x02;
    returnValue.Refus = type & 0x01;
    returnValue.Extension = type & 0x01;

    return returnValue;
}

/**
 *
 * @param sockfd
 * @param connfd
 * @param isAutomate - 0 si c'est sur le gestionnaire de ressource, 1 si c'est l'automate
 */
void initConnection(int *sockfd, int *connfd, int isAutomate) {
    char addr[16];
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    } else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

    if (isAutomate) {
        // je me connecte sur l'automate
        addr[0] = '\0';
        strcat(addr, SERVEURNAME);
        strcat(addr, NAME_AUTO);
        strcat(addr, CHAINE_CARAC_FIN);
        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(addr);
        printf("Addr automate : %s\n", addr);
        servaddr.sin_port = htons(PORT);
    } else {
        // je me connecte sur le gestionnaire de ressources
        addr[0] = '\0';
        strcat(addr, ADDR_GR);
        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr(addr);
        printf("Addr GR : %s\n", addr);
        servaddr.sin_port = htons(PORT_GR);
    }

    // connect the client socket to server socket
    if (connect(*sockfd, (SA *) &servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed\n");
        exit(0);
    } else
        printf("connected to the server\n\n");
}

void closeConnection(int sockfd) {
    close(sockfd);
}

void Run_Stop_Auto(int sockfd, BOOL Run, int addrGuest, int addrDest) {
    unsigned char buffer[50];
    unsigned char rbuffer[MAX_BUFF];
    int len = 0;

    // I init the buffer
    for (short i = 0; i < 50; i++) {
        buffer[i] = 0;
    }

    buffer[3] = 1;
    buffer[5] = 8; // length of the message
    buffer[7] = 0xF0;
    buffer[9] = (addrGuest >> 8) & 0xFF; // My addr xway
    buffer[8] = addrGuest & 0xFF; // My addr xway
    buffer[11] = (addrDest >> 8) & 0xFF; // server addr xway
    buffer[10] = addrDest & 0xFF; // server addr xway
    if (Run == TRUE)
        buffer[12] = 0x24;
    else
        buffer[12] = 0x25;
    buffer[13] = 0x6;

    len = 14;

    printf("\n requete: ");
    for (short i = 0; i < len; i++) {
        printf("%x ", buffer[i]);
    }
    printf("\n\n");
    write(sockfd, buffer, len);
    read(sockfd, rbuffer, sizeof(rbuffer));
    printf("\n reponse: ");
    for (short i = 0; i <= rbuffer[5]; i++) {
        printf("%x ", rbuffer[i + 5]);
    }
    printf("\n\n");
    viderBufferE();
}

/**
 *
 * @param sockfd - la sock de connexion
 * @param addrGuest - ex: 0x801B
 * @param addrDest - ex: 0x8015
 * @param addr_premier_objet - ex: 0x3200 correspond à l'adresse 0x32 qui correspond à %MW50
 * @param nb_obj - le nombre d'objets à écrire (1 obj = 2 octets)
 * @param tableau - le tableau contenant les octets à envoyer
 */
void Envoi_Tableau_Octet(int sockfd, int addrGuest, int addrDest, int addr_premier_objet, BOOL mot, int nb_obj,
                         char *tableau) {
    unsigned char buffer[50];
    unsigned char rbuffer[MAX_BUFF];
    int len = 0;

    // I init the buffer
    for (short i = 0; i < 50; i++) {
        buffer[i] = 0;
    }

    buffer[3] = 1;
    buffer[7] = 0xF0;
    buffer[9] = (addrGuest >> 8) & 0xFF; // My addr xway
    buffer[8] = addrGuest & 0xFF; // My addr xway
    buffer[11] = (addrDest >> 8) & 0xFF; // server addr xway
    buffer[10] = addrDest & 0xFF; // server addr xway

    buffer[12] = 0x37; // Code requête
    buffer[13] = 0x07; // Code catégorie (0-7)

    if (mot) {
        buffer[14] = 0x68; // segment
        buffer[15] = 0x07; // type
    } else {
        buffer[14] = 0x64; // segment
        buffer[15] = 0x05; // type
    }

    buffer[17] = (addr_premier_objet >> 8) & 0xFF; // addr premier objet suite
    buffer[16] = addr_premier_objet & 0xFF; // addr premier objet (ex: 0A -> %MW10 car 0A -> 10)
    buffer[19] = (nb_obj >> 8) & 0xFF; // nombre d'objet à écrire
    buffer[18] = nb_obj; // nombre d'objet à écrire

    len = 20;

    for (int i = 0; i < nb_obj; i++) {
        if (mot) {
            buffer[20 + i * 2] = tableau[i * 2];
            buffer[21 + i * 2] = tableau[i * 2 + 1];
            len += 2;
        } else {
            buffer[20 + i] = tableau[i];
            len++;
        }

    }

    buffer[5] = len - 6; // length of the message

    printf("\n requete: ");
    for (short i = 0; i < len; i++) {
        printf("%x ", buffer[i]);
    }
    printf("\n\n");
    write(sockfd, buffer, len);
    read(sockfd, rbuffer, sizeof(rbuffer));
    printf("\n reponse: ");
    for (short i = 0; i <= rbuffer[5]; i++) {
        printf("%x ", rbuffer[i + 5]);
    }
    printf("\n\n");
    viderBufferE();
}

/**
 *
 * @param sockfd - la sock de connexion
 * @param addrGuest - ex: 0x801B
 * @param addrDest - ex: 0x8015
 * @param addr_premier_objet - ex: 0x3200 correspond à l'adresse 0x32 qui correspond à %MW50
 * @param mot - TRUE si c'est un mot, FALSE si c'est un bit
 * @param nb_obj - le nombre d'objets à écrire (1 obj = 2 octets)
 * @param tableau - le tableau qui contiendra la réponse
 */
void Lire_Tableau_Octet(int sockfd, int addrGuest, int addrDest, int addr_premier_objet, BOOL mot, int nb_obj,
                        char *rbuffer) {
    unsigned char buffer[50];
    int len = 0;

    // I init the buffer
    for (short i = 0; i < 50; i++) {
        buffer[i] = 0;
    }

    buffer[3] = 1;
    buffer[7] = 0xF0;
    buffer[9] = (addrGuest >> 8) & 0xFF; // My addr xway
    buffer[8] = addrGuest & 0xFF; // My addr xway
    buffer[11] = (addrDest >> 8) & 0xFF; // server addr xway
    buffer[10] = addrDest & 0xFF; // server addr xway
    buffer[12] = 0x36; // Code requête
    buffer[13] = 0x07; // Code catégorie (0-7)

    if (mot) {
        buffer[14] = 0x68; // segment
        buffer[15] = 0x07; // type
    } else {
        buffer[14] = 0x64; // segment
        buffer[15] = 0x05; // type
    }


    buffer[17] = (addr_premier_objet >> 8) & 0xFF; // addr premier objet suite
    buffer[16] = addr_premier_objet & 0xFF; // addr premier objet (ex: 0A -> %MW10 car 0A -> 10)
    buffer[19] = (nb_obj >> 8) & 0xFF; // nombre d'objet à écrire
    buffer[18] = nb_obj; // nombre d'objet à écrire

    len = 20;
    buffer[5] = len - 6; // length of the message

    printf("\n requete: ");
    for (short i = 0; i < len; i++) {
        printf("%x ", buffer[i]);
    }
    printf("\n\n");
    write(sockfd, buffer, len);
    read(sockfd, rbuffer, sizeof(rbuffer));
    printf("\n reponse: ");
    for (short i = 0; i <= rbuffer[5]; i++) {
        printf("%x ", rbuffer[i + 5]);
    }
    printf("\n\n");
    viderBufferE();
}

/**
 * Lis le message sur la socket sock et l'écrit dans la variable message
 */
void lisLeMessage(int sock, char* message, int len){
    read(sock, message, len);
}

/**
 * Envoi le message sr la socket sock
 */
void envoieMessage(int sock, char *buffer) {
    write(sock, buffer, strlen(buffer)+1);
}

void communiqueGestionnaireRessource(char * code, char * res, int sock) {
    // Je commence par calculer le nombre de ressources necessaires
    int m;
    int charcount;

    charcount = 0;
    for(m=0; res[m]; m++) {
        if(res[m] == '/') {
            charcount ++;
        }
    }

    charcount ++;

    unsigned char buff[50];
    sprintf(buff,"%s:%d:%s*", code, charcount, res);
    envoieMessage(sock, buff);
    lisLeMessage(sock, buff, sizeof(buff));
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

void disAuGRMonTrain(int sock) {
    char buff[50];
    sprintf(buff,"T:1");
    envoieMessage(sock, buff);
    lisLeMessage(sock, buff, sizeof(buff));
}

int main() {
    int sortir = 0;
    int sockTCP, connTCP; // automate
    int sockTCP_GR, connTCP_GR; // Gestionnaire ressources
    unsigned char rbuffer[MAX_BUFF];

    unsigned char tableau[6] = {0x07, 0x00};

    //initConnection(&sockTCP, &connTCP, 1);
    initConnection(&sockTCP_GR, &connTCP_GR, 0);

    printf("Demande de démarrage train 1\n");

    disAuGRMonTrain(sockTCP_GR);
    /*demandeRessource("1/2/3", sockTCP_GR);
    sleep(3);
    rendRessource("1/2/3", sockTCP_GR);

    exit(0);*/

    while (1) {
        //printf("J'attend l'interrupteur MA\n");
        //Lire_Tableau_Octet(sockTCP, addrGuest, addrDest, /*ADDR 1er obj*/0x0001, TRUE, /*NB obj*/ 0x0001, rbuffer);
        // todo check si le rbuffer est bon ?
        //printf("MA OK\n");
        printf("Je demande R1 2 et 3");
        demandeRessource("1/2/3", sockTCP_GR);
        // Demande A1b
        // Demande TN0
        // Demande TJ0b
        // Demande A0b
        // tn1
        // t11
        // a7b
        demandeRessource("6/7", sockTCP_GR);
        // tj1b
        // t16
        rendRessource("6", sockTCP_GR);
        // ti6
        demandeRessource("5/6", sockTCP_GR);
        // tj1d
        // tn6
        // t17
        rendRessource("5/6/7"); //todo : opti ?

    }

    closeConnection(sockTCP);
    closeConnection(sockTCP_GR);

    return 0;
}
