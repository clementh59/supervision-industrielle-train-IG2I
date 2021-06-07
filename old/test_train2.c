//
// Created by clement on 04/06/21.
//

/*#include <unistd.h>
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

//#define COMMUNICATE_FOR_REAL

#define ADDR_GR "10.22.205.103"
#define PORT_GR 3300

#define PORT 502
#define SA struct sockaddr

#define SERVEURNAME "10.22.205."
#define NAME_AUTO "203"
#define CHAINE_CARAC_FIN "\0"
#define SORTIR 5
#define MISE_EM 1
#define MISE_EA 2
#define SEND_TO 3
#define READ_TO 4
#define MAX_BUFF 50
#define MAX_XWAY_FRAME_LENGTH 80

int addrGuest = 0x801B;
int addrDest = 0x8015;

typedef struct {
    unsigned int length;
    unsigned char trame[MAX_XWAY_FRAME_LENGTH];
} trame_t;

typedef enum {
    TRUE = 1,
    FALSE = 0
} BOOL;

void initConnection(int *sockfd, int *connfd, int isAutomate);

void closeConnection(int sockfd);*/

/**
 *
 * @param sockfd - la sock de connexion
 * @param addrGuest - ex: 0x801B
 * @param addrDest - ex: 0x8015
 * @param addr_premier_objet - ex: 0x3200 correspond à l'adresse 0x32 qui correspond à %MW50
 * @param nb_obj - le nombre d'objets à écrire (1 obj = 2 octets)
 * @param tableau - le tableau contenant les octets à envoyer
 */
void Envoi_Tableau_Octet(int sockfd, int addrGuest, int addrDest, int addr_premier_objet, int nb_obj, char *tableau) {
    unsigned char buffer[50];
    unsigned char rbuffer[MAX_BUFF];
    int len = 0;
    trame_t trame;

    initTrame(&trame);
    ajouteOctetToTrame(&trame, 0xF0);
    ajouteDeuxOctetsToTrame(&trame, addrGuest);
    ajouteDeuxOctetsToTrame(&trame, addrDest);
    ajouteOctetToTrame(&trame, 0x37); // Code requête
    ajouteOctetToTrame(&trame, 0x06); // Code catégorie (0-7)

    ajouteOctetToTrame(&trame, 0x68); // segment
    ajouteOctetToTrame(&trame, 0x07); // type

    ajouteDeuxOctetsToTrame(&trame, addr_premier_objet); // addr premier objet (ex: 0A -> %MW10 car 0A -> 10)
    ajouteDeuxOctetsToTrame(&trame, nb_obj); // nb d'objet à ecrire

    for (int i = 0; i < nb_obj; i++) {
        ajouteDeuxOctetsToTrame(&trame, tableau[i]);
    }

    buffer[5] = len - 6; // length of the message

    envoiLaTrame(sockfd, &trame);

    /*write(sockfd, buffer, len);
    sleep(1);
    read(sockfd, rbuffer, sizeof(rbuffer));
    printf("\n reponse: ");
    for (short i = 0; i <= rbuffer[5]; i++) {
        printf("%x ", rbuffer[i + 5]);
    }
    printf("\n\n");
    for (short i = 0; i <= rbuffer[5]; i++) {
        printf("%x ", rbuffer[i]);
    }
    //viderBufferE();
    sleep(1);
    printf("J'ai vidé le buffer\n");
    read(sockfd, rbuffer, sizeof(rbuffer));
    printf("\n reponse: ");
    for (short i = 0; i <= rbuffer[5]+7; i++) {
        printf("%x ", rbuffer[i]);
    }
    printf("\n");
    unsigned char buf2[25]={0,0,0,1,0,9,0,0xF1,
                            addrGuest & 0xFF,0x80, //mon PC
                            addrDest & 0xFF,0x80, //l'automate
                            0x19,
                            rbuffer[13],
                            0xFE}; //reponse pour l'automate
    printf("\nwrite Automate :\n");
    for(int i=0;i<buf2[5]+7;i++)
    {printf(" %x",buf2[i]);}
    printf("\n");
    write(sockfd,buf2,buf2[5]+7);*/
}

void commandeAiguillage(int sock, char valeur) {
    unsigned char tableau1[1];
    tableau1[0] = valeur;
    Envoi_Tableau_Octet(sock, 0x801b, 0x8017, 0x01f6, 0x0001, tableau1);
}

void commandeTroncon(int sock, char valeur) {
    unsigned char tableau1[1];
    tableau1[0] = valeur;
    Envoi_Tableau_Octet(sock, 0x801b, 0x8017, 0x01f4, 0x0001, tableau1);
}

int main() {
    int sortir = 0;
    int sockTCP; // automate
    int sockTCP_GR; // Gestionnaire ressources
    unsigned char rbuffer[MAX_BUFF];

    printf("Je me connecte à la socket\n");
    //initConnection(&sockTCP, &connTCP, 1);
    printf("Connection OK\n");
    //initConnection(&sockTCP_GR, &connTCP_GR, 0);

    printf("Demande de démarrage train 1\n");

    // A1b //// 0x01f6 = %MW502
    //tableau1[0] = 50;Envoi_Tableau_Octet(sockTCP, 0x801B, 0x8017, 0x01f6, TRUE, 0x0001, tableau1);

    // T16 //// 0x01f4 = %MW500

    //commandeAiguillage(100);
    commandeTroncon(sockTCP, 16);
    sleep(1);
    commandeTroncon(sockTCP, 7);
    //tableau1[0] = 16;Envoi_Tableau_Octet(sockTCP, 0x801b, 0x8017, 0x01f4, TRUE, 0x0001, tableau1);
    //tableau1[0] = 7;Envoi_Tableau_Octet(sockTCP, 0x801b, 0x8017, 0x01f4, TRUE, 0x0001, tableau1);

    // Ti6 //// 0x01f5 = %MW501
    //tableau1[0] = 26;Envoi_Tableau_Octet(sockTCP, 0x801B, 0x8017, 0x01f5, TRUE, 0x0001, tableau);

    sleep(1);

    //closeConnection(sockTCP);
    //closeConnection(sockTCP_GR);

    return 0;
}