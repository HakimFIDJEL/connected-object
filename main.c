#include "./heads/data.h"
#include "./heads/session.h"
#include "./heads/users.h"
#include <pthread.h>

#define ADDR "0.0.0.0"
// #define ADDR "192.168.238.73"
#define PORT 5000
#define MODE SOCK_STREAM
#define MAXSIZE 1024

int nbrUser = 0;
bool gameStarted = false;
bool gameEnded = false;



void serveur();
void client();
void dialogueSrv(socket_t *sockEch, buffer_t buff, pFct serial);
void *server_thread(void * arg);
void *timer_thread(void * arg);
void messHandler(buffer_t buff, socket_t sock);
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
#ifdef SERVEUR
    serveur();
#else
    client();
#endif
    return 0;
}

socket_t sockEcoute;


void serveur()
{
    socket_t sockEch;
    buffer_t buff;
    pid_t pid;

    init_users();



    
    sockEcoute = creerSocketEcoute(ADDR, PORT);
    printf("Socket écoute créée : %d\n", sockEcoute.fd);


    while(1)
    {
        // Accepter une connexion
        sockEch = accepterClt(sockEcoute);
        printf("Socket dialogue créée : %d\n", sockEch.fd);

        if(nbrUser == 0)
        {
            nbrUser++;
            pthread_t timer;
            pthread_create(&timer, NULL, &timer_thread, NULL);

        }

        User user = add_user(sockEch);
        
        display_users();
        
        // On crée un thread pour chaque client
        pthread_t server;
        pthread_create(&server, NULL, &server_thread, &sockEch);




    }

}

void *timer_thread(void * arg)
{
    printf("[Timer_thread] La partie se lance dans 15 secondes\n");
    sleep(10);
    for(int i = 5; i > 0; i--)
    {
        printf("[Timer_thread]  %d s\n", i);
        sleep(1);
    }
    printf("[Timer_thread] Lancement de la partie\n");
    gameStarted = true;

    // On récupère les utilisateurs
    User *users = get_users();
    // On envoie à toutes les sockets d'utilisateurs le message de lancement de la partie
    for(int i = 0; i < 10; i++)
    {
        if(users[i].id != -1)
        {
            envoyer(&users[i].socket, "start\n", NULL);
        }
    }

    return NULL;
}


void *server_thread(void * arg)
{
    socket_t sockEch = *(socket_t *) arg;
    buffer_t buff;
    
    

    printf("[Server_thread] Nouvelle connexion depuis l'IP %s\n", inet_ntoa(sockEch.addrLoc.sin_addr));

    dialogueSrv(&sockEch, buff, NULL);

    remove_user(get_user_by_socket(sockEch));
    display_users();

    printf("[Server_thread] Femeture de la socket %d \n", sockEch.fd);


    fermerSocket(&sockEch);
    return NULL;
}


void dialogueSrv(socket_t *sockEch, buffer_t buff, pFct serial)
{
    User *users = get_users(); 

    while(!gameStarted)
    {
        sleep(1);
    }

    while(1)
    {
        // Recevoir
        recevoir(sockEch, buff, NULL);


        // Traiter
        messHandler(buff, *sockEch);


        
        // Traiter
        /*printf("[dialogueSrv] Message reçu : %s\n", buff);

        if(strcmp(buff, "exit\n") == 0)
        {
            return;
        }
        */

        // On vide le buffer
        buff[0] = '\0';
        
    }

    return;
}


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

    // pthread_create(&threadEnvoi, NULL, EnvoiClt, &sockConn);
    pthread_create(&threadReception, NULL, ReceptionClt, &sockConn);


    // On attend la fin des threads //

    // pthread_join(threadEnvoi, NULL);
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


void messHandler(buffer_t buff, socket_t sock)
{
    User user = get_user_by_socket(sock);
    // On récupère le premier caractère du buffer
    char c = buff[0];

    switch(c)
    {
        // Fin d'une figure
        case '0':
            add_figure_to_user(user.id);
            envoyer(&user.socket, "Figure finie !\n", NULL);
        break;
        // Fin de partie
        case '1':
            printf("L'utilisateur %d a terminé la partie\n", user.id);

            User *users = get_users();
            // Déconnecte tous les utilisateurs
            for(int i = 0; i < 10; i++)
            {
                if(users[i].id != -1)
                {
                    envoyer(&users[i].socket, "exit\n", NULL);
                }
            }

        break;
    }
}
