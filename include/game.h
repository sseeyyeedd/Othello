#ifndef GAME_H
#define GAME_H
#include "guid.h"
#include "disc.h"
#include "timeControl.h"
#include <time.h>
#include<stdbool.h>
typedef struct
{
    GUID id;
    time_t startTime;
    time_t endTime;
    GUID whitePlayerId;
    GUID blackPlayerId;
    Disc Board[8][8];
    TimeControl timeControl;
    bool whiteToPlay;
    int whiteScores[32];
    int whiteMoveCount;
    int blackScores[32];
    int blackMoveCount;
} Game;
typedef struct GameNode
{
    Game game;
    struct GameNode *next;
} GameNode;
char *serializeGame(Game game);
Game deserializeGame(char *json);
char *serializeGameNode(GameNode *node);
GameNode *deserializeGameNode(char *json);
#endif