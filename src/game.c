#include "game.h"

int game_level            = 1;
int game_over             = 0;
int total_score           = 0;

int position_x            = 0;
int position_y            = 0;
unsigned int number_block = 0;

unsigned int *players_connected_pointer = NULL;
int players_shapes[7] = {0};

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

int process_heap_collisions(void)
{ // Проверка на столкновение: проход по высоте
    for (int l = 0; l < current_block.height; ++l)
    {
        // проход по ширине
        for (int i = position_x + current_block.width - 1; i >= position_x; --i)
        {
            // Условие столкновения фигуры с дном или с кучей
            if ((cup[position_y + current_block.height - l][i] != ' ') &&
                (current_block.data[current_block.height - 1 - l][i - position_x] != ' '))
            {
                // функция добавления фигуры в матрицу стакана
                add_shape_in_cup(cup);
                total_score += 5;
                // Проверка на заполнение линий
                line_is_full();

                return 1;
            }
        }
    }
    return 0;
}

void add_shape_in_cup(char cup_buf[CUPSIZE_Y][CUPSIZE_X])
{
    for (int i = 0; i < current_block.height; ++i)
    {
        for (int j = 0; j < current_block.width; ++j)
        {
            if (current_block.data[i][j] != ' ')
            {
                cup_buf[position_y + i][position_x + j] = current_block.data[i][j];
            }
        }
    }
}

void line_is_full(void)
{
    int counter = 0;

    for (int i = CUPSIZE_Y - 2; i > 0; --i)
    {
        for (int j = 1; j < CUPSIZE_X - 1; ++j)
        {
            if (cup[i][j] != ' ')
            {
                ++counter;
            }
        }

        if (counter == CUPSIZE_X - 2)
        {
            clear_line_from_cup(i);
            i = CUPSIZE_Y - 1;
        }

        counter = 0;
    }
}

void clear_line_from_cup(int number)
{
    int counter = 0;

    for (int i = number; i > 1; --i)
    {
        for (int j = 1; j < CUPSIZE_X - 1; ++j)
        {
            cup[i][j] = cup[i - 1][j];

            if (cup[i][j] == ' ')
            {
                ++counter;
            }
        }

        if (counter == CUPSIZE_X - 2)
        {
            total_score += 50;
            return;
        }

        counter = 0;
    }
}

void spawn_new_block(void)
{
    number_block = players_shapes[generate_random_number(0, *players_connected_pointer - 1)];
    current_block = blocks[number_block];

    position_x = (CUPSIZE_X / 2) - (current_block.width / 2);
    position_y = 0;

    if (process_heap_collisions())
    {
        game_over = 1;
    }

    if (total_score > game_level * 500 && game_level < 8)
    {
        game_level++;
    }
}

void block_drop(void)
{
    if (process_heap_collisions())
    {
        spawn_new_block();
    }

    position_y++;
}