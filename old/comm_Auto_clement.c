//#include "Biblio/StructureComm.h"

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <ifaddrs.h>

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

typedef struct{  // 1 octet
    unsigned char Type; //&0x04
    unsigned char Syze_Level; //&0x02
    unsigned char Refus; //&0x01
    unsigned char Extension; //&0x01
}Xway_typeNPDU;

typedef struct{  // 2 octets
    unsigned char num_Station; //&0xFF
    unsigned char num_Net; //&0x0F) << 4
    unsigned char num_Port; //&0x0F
}Xway_Addr_Type;

typedef struct{ // 4 octets
    Xway_Addr_Type addr_Src;
    Xway_Addr_Type addr_Dest;
}Xway_Addr;

typedef struct{
    unsigned char code_Param; //&0xFF
    unsigned char val_Param[7]; //&0xFF
    unsigned char nParam;
}Xway_Param;

typedef struct{
    Xway_typeNPDU NPDU;
    Xway_Addr addr;
    Xway_Param *addr3_5;
    Xway_Param *addr6;
    unsigned char *donnee;
}Xway_Frame;

typedef struct{
    unsigned char id_Modbus[5]; //{0x0,0x0,0x0,0x1,0x0}
    unsigned char syze;
    Xway_Frame donnee;
}Encaps_ModBus;

typedef enum{
    TRUE=1,
    FALSE=0
} BOOL;

void initConnection(int *sockfd, int *connfd);
void closeConnection(int sockfd);

void viderBufferE(){
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

Xway_typeNPDU createNPDU(short type, short syze_level, short refus, short Extension){
    Xway_typeNPDU returnValue;

    returnValue.Type=type&0x04;
    returnValue.Syze_Level=type&0x02;
    returnValue.Refus=type&0x01;
    returnValue.Extension=type&0x01;

    return returnValue;
}

void initConnection(int *sockfd, int *connfd){
    char addr[16];
    struct sockaddr_in servaddr, cli;
    addr[0]='\0';

    // socket create and varification
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));
    strcat(addr,SERVEURNAME);
    strcat(addr,NAME_AUTO);
    strcat(addr,CHAINE_CARAC_FIN);
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(addr);
    printf("%s\n",addr);
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(*sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed\n");
        exit(0);
    }
    else
        printf("connected to the server\n");
    viderBufferE();
}

void closeConnection(int sockfd){
    close(sockfd);
}

void Run_Stop_Auto(int sockfd,BOOL Run,int addrGuest,int addrDest){
    unsigned char buffer[50];
    unsigned char rbuffer[MAX_BUFF];
    int len = 0;

    // I init the buffer
    for(short i=0;i<50;i++){
        buffer[i]=0;
    }

    buffer[3]=1;
    buffer[5]=8; // length of the message
    buffer[7]=0xF0;
    buffer[9]=(addrGuest>>8)&0xFF; // My addr xway
    buffer[8]=addrGuest&0xFF; // My addr xway
    buffer[11]=(addrDest>>8)&0xFF; // server addr xway
    buffer[10]=addrDest&0xFF; // server addr xway
    if(Run==TRUE)
        buffer[12]=0x24;
    else
        buffer[12]=0x25;
    buffer[13]=0x6;

    len = 14;

    printf("\n requete: ");
    for(short i=0; i<len;i++){
        printf("%x ",buffer[i]);
    }
    printf("\n\n");
    write(sockfd,buffer,len);
    read(sockfd,rbuffer,sizeof(rbuffer));
    printf("\n reponse: ");
    for(short i=0; i<=rbuffer[5];i++){
        printf("%x ",rbuffer[i+5]);
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
void Envoi_Tableau_Octet(int sockfd, int addrGuest,int addrDest, int addr_premier_objet, BOOL mot, int nb_obj, char * tableau) {
    unsigned char buffer[50];
    unsigned char rbuffer[MAX_BUFF];
    int len = 0;

    // I init the buffer
    for(short i=0;i<50;i++){
        buffer[i]=0;
    }

    buffer[3]=1;
    buffer[7]=0xF0;
    buffer[9]=(addrGuest>>8)&0xFF; // My addr xway
    buffer[8]=addrGuest&0xFF; // My addr xway
    buffer[11]=(addrDest>>8)&0xFF; // server addr xway
    buffer[10]=addrDest&0xFF; // server addr xway

    buffer[12]= 0x37; // Code requête
    buffer[13]= 0x07; // Code catégorie (0-7)

    if (mot) {
        buffer[14] = 0x68; // segment
        buffer[15] = 0x07; // type
    }
    else {
        buffer[14] = 0x64; // segment
        buffer[15] = 0x05; // type
    }

    buffer[17] = (addr_premier_objet>>8)&0xFF; // addr premier objet suite
    buffer[16] = addr_premier_objet&0xFF; // addr premier objet (ex: 0A -> %MW10 car 0A -> 10)
    buffer[19] = (nb_obj>>8)&0xFF; // nombre d'objet à écrire
    buffer[18] = nb_obj; // nombre d'objet à écrire

    len = 20;

    for (int i = 0; i < nb_obj; i++) {
        if (mot) {
            buffer[20+i*2] = tableau[i*2];
            buffer[21+i*2] = tableau[i*2+1];
            len += 2;
        } else {
            buffer[20+i] = tableau[i];
            len ++;
        }

    }

    buffer[5]=len-6; // length of the message

    printf("\n requete: ");
    for(short i=0; i<len;i++){
        printf("%x ",buffer[i]);
    }
    printf("\n\n");
    write(sockfd,buffer,len);

    // READ 1
    read(sockfd,rbuffer,sizeof(rbuffer));
    printf("\n reponse: ");
    for(short i=0; i<=rbuffer[5];i++){
        printf("%x ",rbuffer[i+5]);
    }
    printf("\n\n");
    viderBufferE();

    // READ 2
    read(sockfd,rbuffer,sizeof(rbuffer));
    printf("\n reponse: ");
    for(short i=0; i<=rbuffer[5];i++){
        printf("%x ",rbuffer[i+5]);
    }
    printf("\n\n");
    viderBufferE();



    // WRITE 2
    len = 0;

    // I init the buffer
    for(short i=0;i<50;i++){
        buffer[i]=0;
    }

    buffer[3]=1;
    buffer[7]=0xF1;
    buffer[11]=(addrGuest>>8)&0xFF; // My addr xway
    buffer[10]=addrGuest&0xFF; // My addr xway
    buffer[9]=(addrDest>>8)&0xFF; // server addr xway
    buffer[8]=addrDest&0xFF; // server addr xway

    buffer[12]= 0x19;
    buffer[13]= rbuffer[13];
    buffer[14]= 0xFE;

    len = 15;

    buffer[5]=len-6; // length of the message

    printf("\n requete: ");
    for(short i=0; i<len;i++){
        printf("%x ",buffer[i]);
    }
    printf("\n\n");
    write(sockfd,buffer,len);
}

/**
 *
 * @param sockfd - la sock de connexion
 * @param addrGuest - ex: 0x801B
 * @param addrDest - ex: 0x8015
 * @param addr_premier_objet - ex: 0x3200 correspond à l'adresse 0x32 qui correspond à %MW50
 * @param mot - TRUE si c'est un mot, FALSE si c'est un bit
 * @param nb_obj - le nombre d'objets à écrire (1 obj = 2 octets)
 * @param tableau - le tableau contenant les octets à envoyer
 */
void Lire_Tableau_Octet(int sockfd, int addrGuest, int addrDest, int addr_premier_objet, BOOL mot, int nb_obj, char * tableau) {
    unsigned char buffer[50];
    unsigned char rbuffer[MAX_BUFF];
    int len = 0;

    // I init the buffer
    for(short i=0;i<50;i++){
        buffer[i]=0;
    }

    buffer[3]=1;
    buffer[7]=0xF0;
    buffer[9]=(addrGuest>>8)&0xFF; // My addr xway
    buffer[8]=addrGuest&0xFF; // My addr xway
    buffer[11]=(addrDest>>8)&0xFF; // server addr xway
    buffer[10]=addrDest&0xFF; // server addr xway
    buffer[12] = 0x36; // Code requête
    buffer[13] = 0x07; // Code catégorie (0-7)

    if (mot) {
        buffer[14] = 0x68; // segment
        buffer[15] = 0x07; // type
    }
    else {
        buffer[14] = 0x64; // segment
        buffer[15] = 0x05; // type
    }


    buffer[17] = (addr_premier_objet>>8)&0xFF; // addr premier objet suite
    buffer[16] = addr_premier_objet&0xFF; // addr premier objet (ex: 0A -> %MW10 car 0A -> 10)
    buffer[19] = (nb_obj>>8)&0xFF; // nombre d'objet à écrire
    buffer[18] = nb_obj; // nombre d'objet à écrire

    len = 20;
    buffer[5]=len-6; // length of the message

    printf("\n requete: ");
    for(short i=0; i<len;i++){
        printf("%x ",buffer[i]);
    }
    printf("\n\n");
    write(sockfd,buffer,len);
    read(sockfd,rbuffer,sizeof(rbuffer));
    printf("\n reponse: ");
    for(short i=0; i<=rbuffer[5];i++){
        printf("%x ",rbuffer[i+5]);
    }
    printf("\n\n");
    viderBufferE();
}

int menu(){
    int choix;

    system("clear");
    printf("+++++++++++++++++++++++++++++++\n");
    printf("CHOIX AUTOMATE\n");
    printf("\t 1) Mise en marche\n");
    printf("\t 2) Mise a l'arret\n");
    printf("\t 3) Ecriture d'un tableau d'octet\n");
    printf("\t 4) Lecture d'un tableau d'octet\n");
    printf("\t 5) Quitter\n");
    printf("Choix: ");scanf("%d",&choix);
    printf("+++++++++++++++++++++++++++++++\n");
    viderBufferE();

    return choix;
}

int main() {
    int sortir=0;
    int sockTCP, connTCP;

    unsigned char tableau[6] = {0x07, 0x00};

    initConnection(&sockTCP,&connTCP);

    while(sortir!=SORTIR){
        sortir = menu();
        switch(sortir){
            case MISE_EM:
                printf("\n\n ---------- MISE EN MARCHE DE L'AUTOMATE ---------- \n\n");
                Run_Stop_Auto(sockTCP,TRUE,0x801B,0x8017);
                break;
            case MISE_EA:
                printf("\n\n ---------- MISE A L'ARRET DE L'AUTOMATE ---------- \n\n");
                Run_Stop_Auto(sockTCP,FALSE,0x801B,0x8017);
                break;
            case SEND_TO:
                printf("\n\n ---------- ENVOI D'UN TABLEAU D'OCTET A L'AUTOMATE ---------- \n\n");
                //Envoi_Tableau_Octet(sockTCP, 0x801B, 0x8015, 0x0001, FALSE, 0x0004, tableau);
                Envoi_Tableau_Octet(sockTCP, 0x801B, 0x8017, 0x0032, TRUE, 0x0001, tableau);
                break;
            case READ_TO:
                printf("\n\n ---------- LECTURE D'UN TABLEAU D'OCTET DE L'AUTOMATE ---------- \n\n");
                //Lire_Tableau_Octet(sockTCP, 0x801B, 0x8015, 0x0001, FALSE, 0x0004, tableau);
                Lire_Tableau_Octet(sockTCP, 0x801B, 0x8017, 0x0001, TRUE, 0x0001, tableau);
                break;
            case SORTIR:
                printf("\n\n ---------- FIN DE LA COMMUNICATION ---------- \n\n");
                break;
            default:
                printf("\n\n ---------- DEFAULT ---------- \n\n");
                break;
        }
    }

    closeConnection(sockTCP);

    return 0;
}
