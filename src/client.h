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

int size_win_y = 0;
int size_win_x = 0;

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

void start_game_menu(char *path_pointer);
int print_game_menu(void);
void *start_server(char *path_pointer);

void exit_handler(void);

void reply_with_name(void);
void send_data_to_server(int input_code);
bool receive_data_from_server(void);

int procces_input(int key_code);
void draw_cup(void);

void setup_terminal(void);
void nc_init(void);
void nc_setup_colors(void);
void nc_cleanup(void);
void resize_win();

#endif // CLIENT_H