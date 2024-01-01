#ifndef GAMESCORE_H
#define GAMESCORE_H
#include"guid.h"
typedef struct
{
    GUID userId;
    GUID gameId;
    int score;
} GameScore;
typedef struct GameScoreNode
{
    GameScore gameScore;
    struct GameScoreNode *next;
} GameScoreNode;
char *serializeGameScore(GameScore gameScore);
GameScore deserializeGameScore(char *json);
char *serializeGameScoreNode(GameScoreNode *node);
GameScoreNode *deserializeGameScoreNode(char *json);
#endif