#ifndef CLIENT_H
#define CLIENT_H

#include <termios.h>
#include <ncurses.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include "util.h"

#define BUFFER_SIZE             2000
#define CUP_SIZE                273
#define CUP_SIZE_X              23
#define CUP_SIZE_Y              31
#define CLIENT_SHAPE_INFO_SIZE  1024
#define CLIENT_NAME_SIZE        32

struct termios save;

int sock               = 0;
bool setup_as_a_server = false;

char cup[CUP_SIZE] = {0};
int score           = 0;

char client_name[CLIENT_NAME_SIZE]             = {0};
char client_shape_info[CLIENT_SHAPE_INFO_SIZE] = {0};
char server_reply[BUFFER_SIZE]                 = {0};
int bytes_received = 0;

pthread_t server_thread = 0;
int execution_result    = 0;

void initialize(void);
void exit_handler(void);
void start_game_menu(void);
int print_game_menu(void);
void setup_terminal(void);
void *start_server();
void reply_with_name(void);
void nc_cleanup(void);
void nc_init(void);
void send_data_to_server(int input_code);
bool receive_data_from_server();
int procces_input(int key_code);
void nc_setup_colors(void);
void draw_cup(void);

#endif // CLIENT_H