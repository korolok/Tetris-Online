#include "game.h"

int game_level            = 1;
int game_over             = 0;
int total_score           = 0;

int position_x            = 0;
int position_y            = 0;
unsigned int number_block = 0;

struct tetris_block
{
    char data[5][5];
    int width;
    int height;
};

char cup[CUPSIZE_Y][CUPSIZE_X] = {
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|          |",
    "|----------|"};

struct tetris_block blocks[] = {
    {{" T ",
      "TTT"},
     3,
     2},
    {{"OO",
      "OO"},
     2,
     2},
    {{"IIII"},
     4,
     1},
    {{"JJ",
      "J ",
      "J "},
     2,
     3},
    {{"LL",
      " L",
      " L"},
     2,
     3},
    {{"ZZ ",
      " ZZ"},
     3,
     2},
    {{" SS",
      "SS "},
     3,
     2}};

struct tetris_block current_block = {0};
