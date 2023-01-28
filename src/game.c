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

void process_rotations(void)
{
    struct tetris_block temp_block_a = current_block;
    struct tetris_block temp_block_b = temp_block_a;

    int temp_pos_x;
    int temp_pos_y;
    temp_block_a.width = temp_block_b.height;
    temp_block_a.height = temp_block_b.width;

    for (int i = 0; i < temp_block_b.width; ++i)
    {
        for (int j = 0; j < temp_block_b.height; ++j)
        {
            temp_block_a.data[i][j] = temp_block_b.data[temp_block_b.height - j - 1][i];
        }
    }

    temp_pos_x = position_x;
    temp_pos_y = position_y;

    position_x -= (temp_block_a.width - temp_block_b.width) / 2;
    position_y -= (temp_block_a.height - temp_block_b.height) / 2;
    current_block = temp_block_a;

    if (process_cup_border_collisions())
    {
        current_block = temp_block_b;
        position_x = temp_pos_x;
        position_y = temp_pos_y;
    }
}

int process_cup_border_collisions(void)
{
    for (int i = 0; i < current_block.height; ++i)
    {
        for (int j = 0; j < current_block.width; ++j)
        {
            if (current_block.data[i][j] != ' ' && cup[position_y + i][position_x + j] != ' ')
            {
                return 1;
            }
        }
    }

    for (int i = 0; i < current_block.height; ++i)
    {
        for (int j = 0; j < current_block.width; ++j)
        {
            if (current_block.data[i][j] != ' ' && cup[position_y + i][position_x + j] != ' ')
            {
                return 1;
            }
        }
    }

    return 0;
}