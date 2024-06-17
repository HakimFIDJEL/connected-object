#include "./heads/data.h"
#include "./heads/session.h"
#include "./heads/game.h"
#include "./heads/leds.h"
#include "./heads/buttons.h"
#include "./heads/buzzers.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

// #define ADDR "192.168.1.92"
#define ADDR "192.168.238.246"
#define PORT 5000
#define MODE SOCK_STREAM
#define MAXSIZE 1024
#define MAX_FIGURE 2





int figure[4][4];
int tempFigure[4][4];

int figureUpScale[8][8];
int tempFigureUpScale[8][8];

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

    printf("[Client] Initialisation du client\n");

    setupLeds();
    setupButtons();
    setupBuzzer();


    socket_t sockConn;
    buffer_t buff;

    // On affiche la socket
    sockConn = connecterClt2Srv(ADDR, PORT);
    printf("[Client] Socket dialogue créée : %d\n", sockConn.fd);

    ledWait();


    while(1)
    {
        printf("[Client] Attente du lancement de la partie\n");
        recevoir(&sockConn, buff, NULL);
        if(strcmp(buff, "start\n") == 0)
        {
            printf("[Client] Lancement de la partie dans 3 secondes\n");
            for(int i = 3; i > 0; i--)
            {
                ledCountDown(i);
                bcm2835_delay(1000);
            }
            break;
        }

    }

    emptyMatrice();

    

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

  
    

    Position button = {-1, -1};
    int nbrFigure = 0;

   

    while(nbrFigure < MAX_FIGURE && !gameEnded)
    {
        // On vide les tableux de figures
        emptyFigure(tempFigure);
        emptyFigure(figure);

        // On génère une figure aléatoire
        fillFigure();

        // On l'agrandit et on l'affiche sur la matrice de led 8x8
        upscaleFigure(figure, figureUpScale);
        displayFigure(figureUpScale);

        // On met un temps de 3s puis c'est au joueur de reproduire la figure
        printf("A vous de jouer\n");
        bcm2835_delay(3000); 
        emptyMatrice();

        // Tant que la figure n'est pas correcte, on laisse le joueur jouer
        while(!checkIfFigureIsCorrect())
        {
            button = readButtons();

            // Si le joueur presse un bouton
            if (button.row != -1 && button.col != -1) 
            {
                // Si la case est correcte on l'ajoute à la figure temporaire
                if(checkIfCaseIsCorrect(button.row, button.col))
                {
                    // Si la case a pas déjà été ajoutée
                    if(tempFigure[button.row][button.col] == 1)
                    {
                        printf("Case déjà ajoutée\n");
                        continue;
                    }
                    else 
                    {
                        printf("Case ajoutée\n");

                        emptyMatrice();

                        bcm2835_delay(100); 
                        // On affiche la figure temporaire
                        addCaseToFigure(button.row, button.col);
                        upscaleFigure(tempFigure, tempFigureUpScale);
                        displayFigure(tempFigureUpScale);
                    }
                }
                else 
                {
                    printf("Mauvaise case\n");
                    buzzerError();
                    ledError();
                    emptyFigure(tempFigure);
                    emptyMatrice();

                    bcm2835_delay(100); 

                    displayFigure(figureUpScale);
                    bcm2835_delay(3000); 
                    emptyMatrice();

                    if(gameEnded)
                    {
                        break;
                    }
                }
            } else {
                bcm2835_delay(100); 
            }
        }
        if(gameEnded)
        {
            break;
        }
        // On envoi dans le buffer "0" pour signifier la fin d'une figure

        nbrFigure++;
        /*
        if(nbrFigure == MAX_FIGURE )
        {
            envoyer(&sockConn, "1", NULL);
            // gameEnded = true;
        }
        else 
        {
            envoyer(&sockConn, "0", NULL);
        }
        */
        

        buzzerSuccess();
        ledSuccess();
    }

    if(gameEnded)
    {
        printf("Vous avez perdu\n");
        buzzerError();
        ledError();
    }
    else 
    {
        // On envoi dans le buffer "1" pour signifier la fin de la partie
        gameEnded = true;
        envoyer(&sockConn, "1", NULL);
        printf("Bravo vous avez réussi\n");
        buzzerSuccess();
        ledSuccess();
    }

    emptyMatrice();
    closeLeds();
    

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
        // printf("Game ended : %d\n", gameEnded);
        if(strcmp(buff, "exit\n") == 0)
        {
            gameEnded = true;
            printf("Vous avez perdue\n");
            buzzerError();
            ledError();
            emptyMatrice();
            closeLeds();
            // On ferme la socket de dialogue
            printf("[Client] Fermeture de la socket dialogue\n");
            fermerSocket(&sockConn);
            // On tue le programme
            exit(0);

            break;
        }



        // On vide le buffer
        buff[0] = '\0';
    }


    return NULL;
}

