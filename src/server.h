#ifndef SERVER_H
#define SERVER_H

#include <stdbool.h>
#include <signal.h>

//  Code to ask client to send their name back
#define CODE_TO_REPLY_WITH_NAME     133
#define MAX_PLAYER_COUNT            7
#define NAME_BUFFER_SIZE            32

struct Player
{
    char            name[NAME_BUFFER_SIZE];
    unsigned int    shape_type;
    int             socket;
};

struct Player players[MAX_PLAYER_COUNT];

int listener_socket;

unsigned int players_connected              = 0;
unsigned int current_active_player          = 0;
bool         should_stop_accepting_clients  = false;

void interrupt_handler(int sigint);
void stop_accepting_clients_handler(int sigusr);

void initialize_server(void);
void accept_clients(bool* stop_accepting);

//  Returns an int value which represents the key pressed by currently active player.
//  (In accordance to keycode chart)
unsigned int 	receive_data_from_clients(void);
void            send_data_to_clients(void* buffer, unsigned int buff_size);

void cleanup();

int main(void);

#endif //   SERVER_H