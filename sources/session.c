/**
 *	\file		session.c
 *	\brief		Spécification de la couche Session
 *	\author		Fidjel Hakim
 *	\date		25 mars 2023
 *	\version	1.0
 */
/*
*****************************************************************************************
 *	\noop		I N C L U D E S   S P E C I F I Q U E S
 */
#include "../heads/session.h"
/*
*****************************************************************************************
 *	\noop		  F O N C T I O N S
 */
void adr2struct (struct sockaddr_in *addr, char *adrIP, short port){

	addr->sin_family = PF_INET;
	addr->sin_port = htons(port);
	addr->sin_addr.s_addr = inet_addr(adrIP);
    memset(addr->sin_zero, 0, 8);

}

socket_t creerSocket (int mode){
	socket_t sock;

	// Vérifie si le mode de socket est connu
	if (mode != SOCK_STREAM && mode != SOCK_DGRAM){
		fprintf(stderr, "Mode de socket inconnu\n");
		exit(1);
	}

	// Création de la socket en fonction du mode
	sock.mode = mode;
	CHECK(sock.fd = socket(PF_INET, mode, 0), "Can't create");

	return sock;

}

socket_t creerSocketAdr (int mode, char *adrIP, short port){

	socket_t sock;

	// Création de la socket
	sock = creerSocket(mode);

	// Création de l'adressage
	adr2struct(&sock.addrLoc, adrIP, port);

	// Association de l'adressage à la socket
	CHECK(bind(sock.fd, (struct sockaddr *)&sock.addrLoc, sizeof(sock.addrLoc)), "Can't bind");

	return sock;

}

socket_t creerSocketEcoute (char *adrIP, short port){

	socket_t sock;

	//Création de la sock avec adresse
	sock = creerSocketAdr(SOCK_STREAM,adrIP,port);

	// Mise en écoute de la socket, 5 ms pour éviter le DenIT Service 
	CHECK(listen(sock.fd, 5), "Can't listen");

	return sock;
}

socket_t accepterClt (const socket_t sockEcoute){
	socket_t sockDial;
	socklen_t cltLen;

	cltLen = sizeof(struct sockaddr_in);

	// On accept la demande de connexion
	CHECK(sockDial.fd=accept(sockEcoute.fd,(struct sockaddr *)&sockEcoute.addrDst,&cltLen),"Can't accept");
	sockDial.mode = sockEcoute.mode;
	sockDial.addrDst = sockEcoute.addrDst;
	sockDial.addrLoc = sockEcoute.addrLoc;

	return sockDial;
}

socket_t connecterClt2Srv (char *adrIP, short port){
	socket_t sock;

	// Création de la socket
	sock = creerSocket(SOCK_STREAM);

	// Création de l'adressage
	adr2struct(&sock.addrDst, adrIP, port);

	// Connexion au serveur
	CHECK(connect(sock.fd, (struct sockaddr *)&sock.addrDst, sizeof(sock.addrDst)), "Can't connect");

	// Recupérer l'addresse affectée à la socket
	socklen_t len = sizeof(sock.addrLoc);
	CHECK(getsockname(sock.fd, (struct sockaddr *)&sock.addrLoc, &len), "Can't getsockname");

	return sock;
}

// fermerSocket
void fermerSocket (socket_t *sock){
	close(sock->fd);
}