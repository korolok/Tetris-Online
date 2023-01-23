#ifndef SERVER_H
#define SERVER_H

#define MAX_PLAYER_COUNT 7

struct Player
{
    const char *name;
    unsigned int shapeType;
    int socket;
};

struct Player players[MAX_PLAYER_COUNT];

static unsigned int players_connected = 0;
static unsigned int current_player = 0;

#endif //SERVER_H