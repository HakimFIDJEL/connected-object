#include "./heads/data.h"
#include "./heads/session.h"
#include "./heads/users.h"
#include <pthread.h>
// #include <time.h>

#define ADDR "0.0.0.0"
// #define ADDR "192.168.238.73"
#define PORT 5000
#define MODE SOCK_STREAM
#define MAXSIZE 1024

int nbrUser = 0;
bool gameStarted = false;
bool gameEnded = false;


void serveur();
void dialogueSrv(socket_t *sockEch, buffer_t buff, pFct serial);
void *server_thread(void * arg);
void *timer_thread(void * arg);
int messHandler(buffer_t buff, socket_t sock);




int main () 
{
    system("clear");
    serveur();
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
        if(messHandler(buff, *sockEch) == 1)
        {
            return;
        }


        
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


int messHandler(buffer_t buff, socket_t sock)
{
    User user = get_user_by_socket(sock);
    User *users = get_users();
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

            display_users();

            
            // Déconnecte tous les utilisateurs
            for(int i = 0; i < 10; i++)
            {
                if(users[i].id != -1 && users[i].id != user.id)
                {
                    printf("Envoi de la déconnexion à l'utilisateur %d\n", users[i].id);
                    // On ferme la connexion
                    envoyer(&users[i].socket, "exit\n", NULL);
                    remove_user(users[i]);

                }
            }
            return 1;

        break;
    }
    return 0;
}
