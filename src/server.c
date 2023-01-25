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