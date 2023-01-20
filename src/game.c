#include "game.h"

<<<<<<< HEAD
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
=======
int process_cup_border_collisions(void)
{
    int X = 0;
    int Y = 0;
    struct tetris_block tmp = currentBlock;

    for (int i = 0; i < tmp.w; ++i)
    {
        
        for (int j = 0; j < tmp.h; ++j)
        {
            X = positionX + i;
            Y = positionY + j;
            
            if (X < 0 || X >= CUPSIZEX)
            {
                return 1;
            }
            
            if (Y >= CUPSIZEY)
            {
                return 1;
            }
        }
        
    }

    return 0;
}
>>>>>>> 4da3bc2 (добавлена функция обработки столкновений с границами стакана)
