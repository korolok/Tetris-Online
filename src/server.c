#include "server.h"
#include "net.h"

void interrupt_handler(int sigint)
{
#ifdef DEBUG
    printf("Signal accepted SIGINT: %d\n", sigint);
#endif
    should_stop_accepting_clients = true;
    cleanup();
}

void stop_accepting_clients_handler(int sigusr)
{
#ifdef DEBUG
    printf("Signal accepted SIGUSR1: %d stopped accepting clients\n", sigusr);
#endif
    should_stop_accepting_clients = true;
}

void initialize_server(void)
{
    //  Setup server
    listener_socket = create_listener_socket();
    listen_for_new_connections(listener_socket);

    //  Setup signals
    signal(SIGUSR1, stop_accepting_clients_handler);
    signal(SIGINT, interrupt_handler);
}

void accept_clients(bool* stop_accepting)
{
    const unsigned int  CODE = CODE_TO_REPLY_WITH_NAME;
    char                name_buffer[NAME_BUFFER_SIZE] = {0};
    int                 bytes_received = 0;

    while (players_connected != MAX_PLAYER_COUNT && *stop_accepting == false)
    {   
        players[players_connected].socket = SOCKET_NULL;

        // Accept client
        while (*stop_accepting == false)
        {
            players[players_connected].socket = accept_client(listener_socket);

            if (players[players_connected].socket != SOCKET_NULL)
            {
                break;
            }
        }

        if (*stop_accepting)
        {
            break;
        }
		
        // Ask client for their name
        memset(&name_buffer, 0, NAME_BUFFER_SIZE);
        send_data(players[players_connected].socket, (void*)&CODE, sizeof(CODE));
        receive_data(players[players_connected].socket, &name_buffer, NAME_BUFFER_SIZE, &bytes_received);

        if (bytes_received == 0)
        {
            printf("Failed to receive player's name\n");
            exit(EXIT_FAILURE);
        }
        
        players[players_connected].name = name_buffer;
        ++players_connected;
	}

#ifdef DEBUG
    printf("Connected clients - %d\n", players_connected);
    for (unsigned int i = 0; i < players_connected; ++i)
    {
        printf("Player #%d name: %s | Socket: %d\n", i, players[i].name, players[i].socket);
    }
#endif
    
    if (players_connected == 0) 
    {
        printf("Failed to accept clients\n");
        exit(EXIT_FAILURE);
    }
}

unsigned int receive_data_from_clients(void)
{   
    unsigned int    keycode         = 0;
    int             bytes_received  = 0;

    receive_data(players[current_active_player].socket, &keycode, sizeof(keycode), &bytes_received);

#ifdef DEBUG
    if (bytes_received == 0)
    {
        printf("Failed to receive data from client\n");
    }
#endif

    return keycode;
}

void send_data_to_clients(void* buffer, unsigned int buff_size)
{
    for (unsigned int i = 0; i < players_connected; ++i)
    {
        send_data(players[i].socket, buffer, buff_size);
    }
}

void cleanup()
{
    close_socket(listener_socket);

    if (players_connected != 0)
    {
        for (unsigned int i = 0; i < players_connected; ++i)
        {
            close_socket(players[i].socket);
        }
    }
}

int main(void)
{
    initialize_server();
    accept_clients(&should_stop_accepting_clients);

    cleanup();
    
    exit(EXIT_SUCCESS);
    return 0;
}