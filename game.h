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

#define CUPSIZEX 23
#define CUPSIZEY 31

static int gameLevel = 1;
static int gameOver = 0;
static int totalScore = 0;
static int positionX = 0;
static int positionY = 0;
static int numberBlock = 0;

static long long currentTime = 0;
static long long previousTime = 0;
static long long deltaTime = 0;
static long long dropDownTime = 0;

struct tetris_block
{
    char data[5][5];
    int w;
    int h;
};
static char cup[CUPSIZEY][CUPSIZEX] = {
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|                     |",
"|---------------------|"
};
struct tetris_block blocks[] = {
{{"##",
  "##"},
2,
2},
{{" X ",
  "XXX"},
 3,
 2},
{{"@@@@"},
 4,
 1},
{{"OO",
  "O ",
  "O "},
 2,
 3},
{{"&&",
  " &",
  " &"},
 2,
 3},
{{"ZZ ",
  " ZZ"},
 3,
 2},
{{" ZZ",
  "ZZ "},
 3,
 2}
};


#endif //GAME_H