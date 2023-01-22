#ifndef SERVER_H
#define SERVER_H

#define MAX_PLAYER_COUNT 7

struct Player
{
    const char *name;
    unsigned int shapeType;
    int socket;
};

Player players[MAX_PLAYER_COUNT];

static unsigned int playersConnected = 0;
static unsigned int currentPlayer = 0;

#endif //SERVER_H