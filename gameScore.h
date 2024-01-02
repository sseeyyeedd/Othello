#ifndef GAMESCORE_H
#define GAMESCORE_H
#include"guid.h"
#include<stdlib.h>
#include<string.h>
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
char *serializeGameScore(GameScore gameScore)
{
    int requiredSize = snprintf(NULL, 0, "{\"userId\":%s,\"gameId\":%s,\"score\":%d}",
                                serializeGuid(gameScore.userId), serializeGuid(gameScore.gameId), gameScore.score);

    char *jsonString = (char *)malloc((requiredSize + 1) * sizeof(char));
    if (jsonString != NULL)
    {
        sprintf(jsonString, "{\"userId\":%s,\"gameId\":%s,\"score\":%d}",
                serializeGuid(gameScore.userId), serializeGuid(gameScore.gameId), gameScore.score);
    }
    return jsonString;
}

GameScore deserializeGameScore(char *json)
{
    GameScore gameScore;
    char *userid, *gameid;
    gameid = userid = (char *)malloc(sizeof(char) * 80);
    sscanf(json, "{\"userId\":%s,\"gameId\":%s,\"score\":%d}",
           userid, gameid, &gameScore.score);
    gameScore.userId = deserializeGuid(userid);
    gameScore.gameId = deserializeGuid(gameid);
    return gameScore;
}

char *serializeGameScoreNode(GameScoreNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    int nodeCount = 0;
    GameScoreNode *current = node;

    // Count the number of nodes in the list
    while (current != NULL)
    {
        nodeCount++;
        current = current->next;
    }

    int totalSize = nodeCount * (/* estimated size of serialized game score + comma and brackets */ 100) + 3;
    char *jsonString = (char *)malloc(totalSize * sizeof(char));
    if (jsonString != NULL)
    {
        strcpy(jsonString, "[");

        current = node;
        while (current != NULL)
        {
            char *serializedGameScore = serializeGameScore(current->gameScore);
            strcat(jsonString, serializedGameScore);
            free(serializedGameScore);

            current = current->next;
            if (current != NULL)
            {
                strcat(jsonString, ",");
            }
        }

        strcat(jsonString, "]");
    }

    return jsonString;
}

GameScoreNode *deserializeGameScoreNode(char *json)
{
    if (json == NULL || json[0] != '[')
    {
        return NULL;
    }

    GameScoreNode *head = NULL;
    GameScoreNode *current = NULL;

    // Move past the opening bracket
    json++;

    while (*json != '\0' && *json != ']')
    {
        // Find the start of a game score object
        if (*json == '{')
        {
            char *end = strchr(json, '}');
            if (end == NULL)
            {
                // Invalid JSON format
                return NULL;
            }

            // Extract the game score object
            int length = end - json + 1;
            char *gameScoreJson = (char *)malloc((length + 1) * sizeof(char));
            strncpy(gameScoreJson, json, length);
            gameScoreJson[length] = '\0';

            GameScore gameScore = deserializeGameScore(gameScoreJson);
            free(gameScoreJson);

            GameScoreNode *newNode = (GameScoreNode *)malloc(sizeof(GameScoreNode));
            newNode->gameScore = gameScore;
            newNode->next = NULL;

            if (head == NULL)
            {
                head = newNode;
                current = head;
            }
            else
            {
                current->next = newNode;
                current = current->next;
            }

            // Move to the next potential game score object
            json = end + 1;
        }
        else
        {
            // Move to the next character
            json++;
        }
    }

    return head;
}
#endif