#include "../include/game.h"
#include<stdlib.h>

char *serializeGame(Game game)
{
    const char *whiteToPlayString = game.whiteToPlay ? "true" : "false";
    int requiredSize = snprintf(NULL, 0, "{\"id\":%s,\"startTime\":%lld,\"endTime\":%lld,\"whitePlayerId\":%s,\"blackPlayerId\":%s,\"timeControl\":%s,\"whiteToPlay\":\"%s\"}",
                                serializeGuid(game.id), game.startTime, game.endTime,
                                serializeGuid(game.whitePlayerId), serializeGuid(game.blackPlayerId),
                                serializeTimeControl(game.timeControl), whiteToPlayString);

    char *jsonString = (char *)malloc((requiredSize + 1) * sizeof(char));
    if (jsonString != NULL)
    {
        sprintf(jsonString, "{\"id\":%s,\"startTime\":%lld,\"endTime\":%lld,\"whitePlayerId\":%s,\"blackPlayerId\":%s,\"timeControl\":%s,\"whiteToPlay\":\"%s\"}",
                serializeGuid(game.id), game.startTime, game.endTime,
                serializeGuid(game.whitePlayerId), serializeGuid(game.blackPlayerId),
                serializeTimeControl(game.timeControl), whiteToPlayString);
    }
    return jsonString;
}
Game deserializeGame(char *json)
{
    Game game;
    char *gameid, *whiteid, *blackid, *tc;
    gameid = whiteid = blackid = (char *)malloc(sizeof(char) * 50);
    tc = (char *)malloc(sizeof(char) * 70);
    char whiteToPlayString[6]; // Assuming "true" or "false" will fit

    sscanf(json, "{\"id\":%s,\"startTime\":%lld,\"endTime\":%lld,\"whitePlayerId\":%s,\"blackPlayerId\":%s,\"timeControl\":%s,\"whiteToPlay\":\"%5s\"}",
           gameid, &game.startTime, &game.endTime,
           whiteid, blackid,
           tc, whiteToPlayString);

    game.id = deserializeGuid(gameid);
    game.whitePlayerId = deserializeGuid(whiteid);
    game.blackPlayerId = deserializeGuid(blackid);
    game.timeControl = deserializeTimeControl(tc);
    game.whiteToPlay = (strcmp(whiteToPlayString, "true") == 0) ? true : false;

    free(gameid);
    free(whiteid);
    free(blackid);
    free(tc);

    return game;
}

// Function to serialize a GameNode into a JSON string
char *serializeGameNode(GameNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    int nodeCount = 0;
    GameNode *current = node;

    // Count the number of nodes in the list
    while (current != NULL)
    {
        nodeCount++;
        current = current->next;
    }

    int totalSize = nodeCount * (/* estimated size of serialized game + comma and brackets */ 500) + 3;
    char *jsonString = (char *)malloc(totalSize * sizeof(char));
    if (jsonString != NULL)
    {
        strcpy(jsonString, "[");

        current = node;
        while (current != NULL)
        {
            char *serializedGame = serializeGame(current->game);
            strcat(jsonString, serializedGame);
            free(serializedGame);

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

// Function to deserialize a JSON array into a linked list of GameNodes
GameNode *deserializeGameNode(char *json)
{
    if (json == NULL || json[0] != '[')
    {
        return NULL;
    }

    GameNode *head = NULL;
    GameNode *current = NULL;

    // Move past the opening bracket
    json++;

    while (*json != '\0' && *json != ']')
    {
        // Find the start of a game object
        if (*json == '{')
        {
            char *end = strchr(json, '}');
            if (end == NULL)
            {
                // Invalid JSON format
                return NULL;
            }

            // Extract the game object
            int length = end - json + 1;
            char *gameJson = (char *)malloc((length + 1) * sizeof(char));
            strncpy(gameJson, json, length);
            gameJson[length] = '\0';

            Game game = deserializeGame(gameJson);
            free(gameJson);

            GameNode *newNode = (GameNode *)malloc(sizeof(GameNode));
            newNode->game = game;
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

            // Move to the next potential game object
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