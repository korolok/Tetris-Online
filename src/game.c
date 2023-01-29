#include "game.h"

int game_level            = 1;
int game_over             = 0;
int total_score           = 0;

int position_x            = 0;
int position_y            = 0;
unsigned int shape_number = 0;

unsigned int *players_connected_pointer = NULL;
int players_shapes[7] = {0};

struct Shape
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

char cup_to_send[CUPSIZE_Y * CUPSIZE_X + CUPSIZE_Y] = {0};

struct Shape shapes[] = {
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

struct Shape current_shape = {0};

void process_rotations(void)
{
    struct Shape temp_shape_a = current_shape;
    struct Shape temp_shape_b = temp_shape_a;

    int temp_pos_x;
    int temp_pos_y;
    temp_shape_a.width = temp_shape_b.height;
    temp_shape_a.height = temp_shape_b.width;

    for (int i = 0; i < temp_shape_b.width; ++i)
    {
        for (int j = 0; j < temp_shape_b.height; ++j)
        {
            temp_shape_a.data[i][j] = temp_shape_b.data[temp_shape_b.height - j - 1][i];
        }
    }

    temp_pos_x = position_x;
    temp_pos_y = position_y;

    position_x -= (temp_shape_a.width - temp_shape_b.width) / 2;
    position_y -= (temp_shape_a.height - temp_shape_b.height) / 2;
    current_shape = temp_shape_a;

    if (process_cup_border_collisions())
    {
        current_shape = temp_shape_b;
        position_x = temp_pos_x;
        position_y = temp_pos_y;
    }
}

int process_cup_border_collisions(void)
{
    for (int i = 0; i < current_shape.height; ++i)
    {
        for (int j = 0; j < current_shape.width; ++j)
        {
            if (current_shape.data[i][j] != ' ' && cup[position_y + i][position_x + j] != ' ')
            {
                return 1;
            }
        }
    }

    for (int i = 0; i < current_shape.height; ++i)
    {
        for (int j = 0; j < current_shape.width; ++j)
        {
            if (current_shape.data[i][j] != ' ' && cup[position_y + i][position_x + j] != ' ')
            {
                return 1;
            }
        }
    }

    return 0;
}

int process_heap_collisions(void)
{ // Проверка на столкновение: проход по высоте
    for (int l = 0; l < current_shape.height; ++l)
    {
        // проход по ширине
        for (int i = position_x + current_shape.width - 1; i >= position_x; --i)
        {
            // Условие столкновения фигуры с дном или с кучей
            if ((cup[position_y + current_shape.height - l][i] != ' ') &&
                (current_shape.data[current_shape.height - 1 - l][i - position_x] != ' '))
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
    for (int i = 0; i < current_shape.height; ++i)
    {
        for (int j = 0; j < current_shape.width; ++j)
        {
            if (current_shape.data[i][j] != ' ')
            {
                cup_buf[position_y + i][position_x + j] = current_shape.data[i][j];
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

void spawn_new_shape(void)
{
    shape_number = players_shapes[generate_random_number(0, *players_connected_pointer - 1)];
    current_shape = shapes[shape_number];

    position_x = (CUPSIZE_X / 2) - (current_shape.width / 2);
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

void shape_drop(void)
{
    if (process_heap_collisions())
    {
        spawn_new_shape();
    }

    position_y++;
}

void form_cup(void)
{
    char cup_buf[CUPSIZE_Y][CUPSIZE_X];

    for (int i = 0; i < CUPSIZE_Y; ++i)
    {
        for (int j = 0; j < CUPSIZE_X; ++j)
        {
            cup_buf[i][j] = cup[i][j];
        }
    }

    add_shape_in_cup(cup_buf);

    int counter = 0;

    for (int i = 0; i < CUPSIZE_Y; ++i)
    {
        for (int j = 0; j < CUPSIZE_X; ++j)
        {
            cup_to_send[counter] = cup_buf[i][j];
            counter++;
        }

        cup_to_send[counter] = '\n';
        counter++;
    }
}

void shape_control(int input)
{
    switch (input)
    {

    case KEY_ARROW_UP:
        process_rotations();
        form_cup();
        break;

    case KEY_ARROW_DOWN:
        shape_drop();
        form_cup();
        break;

    case KEY_ARROW_LEFT:
        position_x--;
        if (process_cup_border_collisions())
        {
            position_x++;
        }

        form_cup();
        break;

    case KEY_ARROW_RIGHT:
        position_x++;
        if (process_cup_border_collisions())
        {
            position_x--;
        }

        form_cup();
        break;
    }
}

char *get_cup_to_send(void)
{
    return (char *)&cup_to_send;
}

int get_cup_to_send_size(void)
{
    return CUPSIZE_Y * CUPSIZE_X + CUPSIZE_Y;
}

int get_game_over(void)
{
    return game_over;
}

void set_user_count_pointer(unsigned int *user_count)
{
    players_connected_pointer = user_count;

    int shapes_present_in_game[7] = {7, 7, 7, 7, 7, 7, 7};
    int flag = 0;

    for (int i = 0; i < (int)*players_connected_pointer;)
    {
        int number_shape = generate_random_number(0, 6);

        for (int j = 0; j < 7; ++j)
        {
            if (shapes_present_in_game[j] == number_shape)
            {
                flag = 1;
            }
        }
        if (flag == 0)
        {
            players_shapes[i] = number_shape;
            shapes_present_in_game[i] = number_shape;
            ++i;
        }
    }
}

unsigned int get_id_current_shape(void)
{
    return shape_number;
}

int get_shapes_type(int player_number)
{
    return players_shapes[player_number];
}

int get_total_score(void)
{
    return total_score;
}

int get_game_level(void)
{
    return game_level;
}