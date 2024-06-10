#include "./heads/data.h"
#include "./heads/session.h"
#include "./heads/users.h"
#include <pthread.h>

#define ADDR "0.0.0.0"
// #define ADDR "192.168.238.73"
#define PORT 5000
#define MODE SOCK_STREAM
#define MAXSIZE 1024



void serveur();
void client();
void dialogueSrv(socket_t *sockEch, buffer_t buff, pFct serial);
void dialogueClt(socket_t *sockConn, buffer_t buff, pFct serial);
void *server_thread(void * arg);




int main () 
{
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

        User user = add_user(sockEch);
        
        display_users();
        
        // On crée un thread pour chaque client
        pthread_t server;
        pthread_create(&server, NULL, &server_thread, &sockEch);

        // On ferme la socket de dialogue
        pthread_join(server, NULL);
    }

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

    while(1)
    {
        // Recevoir
        recevoir(sockEch, buff, NULL);

        // Traiter
        printf("[dialogueSrv] Message reçu : %s\n", buff);

        if(strcmp(buff, "exit\n") == 0)
        {
            return;
        }

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


    // Dialogue client
    dialogueClt(&sockConn, buff, NULL); 

    
    // On ferme la socket de dialogue
    printf("[Client] Fermeture de la socket dialogue\n");
    fermerSocket(&sockConn);
}

void dialogueClt(socket_t *sockConn, buffer_t buff, pFct serial)
{

    while(1)
    {

        // On attend un message
        printf("Entrez un message : ");
        fgets(buff, MAXSIZE, stdin);

        // on enlève le \n du buff
        



        // On envoie le message
        envoyer(sockConn, buff, NULL);

        

        if(strcmp(buff, "exit\n") == 0)
        {
            break;
        }

        // On vide le buffer
        buff[0] = '\0';
    }
    return;
}