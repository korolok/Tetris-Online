#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <termios.h>
#include <fcntl.h>
#include "util.h"
#include "keycodes.h"

#define CUPSIZE_X 12
#define CUPSIZE_Y 21

void process_rotations(void);

int process_cup_border_collisions(void);

int process_heap_collisions(void);
void add_shape_in_cup(char cup_buf[CUPSIZE_Y][CUPSIZE_X]);
void line_is_full(void);
void clear_line_from_cup(int number);
void shape_drop(void);
void spawn_new_shape(void);
void set_user_count_pointer(unsigned int *user_count);
void shape_control(int input);

void form_cup(void);
char* get_cup_to_send(void);

int get_cup_to_send_size(void);
int get_game_over(void);
unsigned int get_id_current_shape(void);
int get_shapes_type(int player_number);
int get_total_score(void);
int get_game_level(void);

#endif // GAME_H