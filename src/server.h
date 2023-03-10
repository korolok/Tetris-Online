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

int          current_key                    = 0;
unsigned int players_connected              = 0;
unsigned int current_active_player          = 0;
bool         should_stop_accepting_clients  = false;

long long current_game_time     = 0;
long long previous_game_time    = 0;
long long shape_drop_down_time  = 0;

void interrupt_handler(int sigint);
void stop_accepting_clients_handler(int sigusr);

void initialize_server(int *argc, char *path_pointer);
void accept_clients(bool* stop_accepting);
void set_clients_non_blocking(void);

//  Returns an int value which represents the key pressed by currently active player.
//  (In accordance to keycode chart)
unsigned int 	receive_data_from_clients(void);
bool            send_data_to_clients(void* buffer, unsigned int buff_size);

void calculate_drop_down_time(void);
void update_server_time(long long* send_data_time);

void process_data(void);
void start_game(void);

void cleanup(void);

int main(int argc, char* argv[]);

#endif // SERVER_H