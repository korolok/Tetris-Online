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

#define CUPSIZE_X 12
#define CUPSIZE_Y 21

void process_rotations(void);

int process_cup_border_collisions(void);

int process_heap_collisions(void);
void add_shape_in_cup(char cup_buf[CUPSIZE_Y][CUPSIZE_X]);
void line_is_full(void);
void clear_line_from_cup(int number);
void block_drop(void);
void spawn_new_block(void);

#endif //GAME_H