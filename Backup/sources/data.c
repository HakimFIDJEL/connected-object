/**
 *	\file		data.c
 *	\brief		Spécification de la couche Data Representation
 *	\author		Fidjel Hakim
 *	\date		25 mars 2023
 *	\version	1.0
 */
/*
*****************************************************************************************
 *	\noop		I N C L U D E S   S P E C I F I Q U E S
 */
#include "../heads/data.h"
/*
*****************************************************************************************
 *	\noop		  F O N C T I O N S
 */
void envoyerMessSTREAM(socket_t *sockEch, buffer_t buffer){

    //Envoi du message
    CHECK(write(sockEch->fd, buffer, strlen(buffer)+1), "Can't send");

}

void envoyerMessDGRAM(socket_t *sockEch, buffer_t buffer, char *ip, int port){

    //Création de l'adressage de destination 
    adr2struct(&sockEch->addrDst, ip, port);
    //Envoi du message à l'adresse de destination 
    CHECK(sendto(sockEch->fd, buffer, strlen(buffer)+1, 0, (struct sockaddr *)&sockEch->addrDst, sizeof(sockEch->addrDst)), "Can't send");
    //Fermeture du socket
    close(sockEch->fd);

}

void envoyer(socket_t *sockEch, generic quoi, pFct serial, ...){
    buffer_t buffer;

    //Serialiser dans buff la requête/réponse à envoyer
    if (serial != NULL) serial(quoi, buffer);
    else strcpy(buffer, (char *)quoi);

    //Envoi : appel de la fonction adéquate selon le mode
    if (sockEch->mode == SOCK_STREAM) envoyerMessSTREAM(sockEch, buffer);
    else{
        va_list pArg;
        va_start(pArg, serial);

        char * addrIP = va_arg(pArg, char *);
        int port = va_arg(pArg, int);
        envoyerMessDGRAM(sockEch, buffer, addrIP,port);
        
        va_end(pArg);
    }    
}

void recevoirMessSTREAM(socket_t *sockEch, buffer_t buffer){

    //Réception du message
    CHECK(read(sockEch->fd, buffer, MAX_BUFFER-1), "Can't receive");

        buffer[strlen(buffer)]='\0';


}

void recevoirMessDGRAM(socket_t *sockEch, buffer_t buffer){
    

    struct sockaddr_in clt;
    socklen_t clt_len = sizeof(clt);

    //Réception du message
    CHECK(recvfrom(sockEch->fd, buffer, sizeof(buffer_t), 0, (struct sockaddr *)&clt, &clt_len), "Can't receive");

    buffer[strlen(buffer)]='\0';


}


void recevoir(socket_t *sockEch, generic quoi, pFct deSerial){
    buffer_t buffer;

    //Réception : appel de la fonction adéquate selon le mode
    if (sockEch->mode == SOCK_STREAM) recevoirMessSTREAM(sockEch, buffer);
    else recevoirMessDGRAM(sockEch, buffer);

    //Désérialiser dans quoi le message reçu
    if (deSerial != NULL) deSerial(quoi, buffer);
    else strcpy((char *)quoi, buffer);

}
