#include "../heads/users.h"


User users[MAX_USERS];
int USER_ID = 0;

void init_users()
{
    for (int i = 0; i < 10; i++)
    {
        users[i].id = -1;
        users[i].figure_id = -1;
        users[i].socket.fd = -1;

    }
    printf("[init_users] Users initialized\n");
}


User add_user(socket_t socket)
{
    for (int i = 0; i < 10; i++)
    {
        if (users[i].id == -1)
        {
            users[i].id = USER_ID;
            users[i].figure_id = 0;
            users[i].socket = socket;
            USER_ID ++;
            printf("[add_user] %d added\n", users[i].id);
            return users[i];
        }
    }
    return users[0];
}

User add_figure_to_user(int id)
{
    for (int i = 0; i < 10; i++)
    {
        if (users[i].id == id)
        {
            users[i].figure_id = users[i].figure_id + 1;
            printf("[add_figure_to_user] L'utilisateur %d est à la figure %d \n", users[i].id, users[i].figure_id);
            return users[i];
        }
    }
    return users[0];
}

void remove_user(User user)
{
    int id = user.id;
    for (int i = 0; i < 10; i++)
    {
        if (users[i].id == user.id)
        {
            users[i].id = -1;
            users[i].socket.fd = -1;
            users[i].figure_id = -1;
            printf("[remove_user] %d removed\n", id);
        }
    }
}



void display_users()
{
    printf("*******************************************\n");
    printf("[display_users] ID | Socket | Figure\n");
    for (int i = 0; i < 10; i++)
    {
        if (users[i].id != -1)
        {
            printf(" %d | %d | %d\n", users[i].id , users[i].socket.fd, users[i].figure_id );
        }
    }
    printf("*******************************************\n"); 

    return;
}


User get_user_by_socket(socket_t socket)
{
    for (int i = 0; i < 10; i++)
    {
        if (users[i].socket.fd == socket.fd)
        {
            return users[i];
        }
    }
    return users[0];
}


// get all users
User *get_users()
{
    return users;
}

bool user_exists(int id)
{
    for (int i = 0; i < 10; i++)
    {
        if (users[i].id == id && users[i].id != -1)
        {
            return true;
        }
    }
    return false;
}



