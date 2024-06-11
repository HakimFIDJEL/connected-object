#include "./heads/data.h"
#include "./heads/session.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define ADDR "192.168.1.92"
// #define ADDR "192.168.238.73"
#define PORT 5000
#define MODE SOCK_STREAM
#define MAXSIZE 1024

bool gameStarted = false;
bool gameEnded = false;



void client();
/**
 * @brief Fonction qui gère l'envoi des messages du client
 * @fn void *EnvoiClt(void *arg)
 * @param arg : la socket de dialogue
 * @return void *
 */
void *EnvoiClt(void *arg);

/**
 * @brief Fonction qui gère la réception des messages du client
 * @fn void *ReceptionClt(void *arg)
 * @param arg : la socket de dialogue
 * @return void *
 */
void *ReceptionClt(void *arg);



int main () 
{
    system("clear");
    client();
    return 0;
}

socket_t sockEcoute;



void client()
{
    socket_t sockConn;
    buffer_t buff;

    // On affiche la socket
    sockConn = connecterClt2Srv(ADDR, PORT);
    printf("[Client] Socket dialogue créée : %d\n", sockConn.fd);


    while(1)
    {
        printf("[Client] Attente du lancement de la partie\n");
        recevoir(&sockConn, buff, NULL);
        if(strcmp(buff, "start\n") == 0)
        {
            printf("[Client] Lancement de la partie\n");
            break;
        }

    }


    // On fait deux threads pour gérer l'envoi et la réception
    pthread_t threadEnvoi, threadReception;

    // On lance les threads //

    pthread_create(&threadEnvoi, NULL, EnvoiClt, &sockConn);
    pthread_create(&threadReception, NULL, ReceptionClt, &sockConn);


    // On attend la fin des threads //

    pthread_join(threadEnvoi, NULL);
    pthread_join(threadReception, NULL);

    
    // On ferme la socket de dialogue
    printf("[Client] Fermeture de la socket dialogue\n");
    fermerSocket(&sockConn);
}


void *EnvoiClt(void *arg)
{
    socket_t sockConn = *(socket_t *) arg;
    buffer_t buff;

    while(!gameEnded)
    {
        // On attend un message
        printf("Entrez un message : ");
        fgets(buff, MAXSIZE, stdin);

        // On envoie le message
        envoyer(&sockConn, buff, NULL);
        if(strcmp(buff, "exit\n") == 0)
        {
            gameEnded = true;

            break;
        }

        // On vide le buffer
        buff[0] = '\0';
    }

    printf("[Client] Fin de l'envoi\n");

    return NULL;
}

void *ReceptionClt(void *arg)
{
    socket_t sockConn = *(socket_t *) arg;
    buffer_t buff;

    while(!gameEnded)
    {
        // On reçoit un message
        recevoir(&sockConn, buff, NULL);
        printf("[Client] Message reçu : %s\n", buff);
        if(strcmp(buff, "exit\n") == 0)
        {
            gameEnded = true;

            break;
        }

        // On vide le buffer
        buff[0] = '\0';
    }


    return NULL;
}

