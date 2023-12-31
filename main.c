#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CHAR_SET_SIZE 256
#define A 2  // 4-0+2-5+2-1+1-9+8
#define K 32 // 4+0+2+5+2+1+1+9+8
typedef enum
{
    false,
    true
} bool;

// Enums
typedef enum
{
    None,
    White,
    Black
} Disc;
// EndEnums
// Structs
typedef struct
{
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
} GUID;
typedef struct
{
    int whitePassedTime;
    int blackPassedTime;
    int timeLimit;
    bool isTimed;
} TimeControl;
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
typedef struct
{
    GUID userId;
    GUID gameId;
    int score;
} GameScore;
typedef struct
{
    GUID id;
    char username[50];
} User;
// Nodes
typedef struct GameNode
{
    Game game;
    struct GameNode *next;
} GameNode;
typedef struct UserNode
{
    User user;
    struct UserNode *next;
} UserNode;
typedef struct GameScoreNode
{
    GameScore gameScore;
    struct GameScoreNode *next;
} GameScoreNode;
// EndNodes
// EndStructs
// Prototypes
GUID generateGUID();
bool compareGUIDs(const GUID *guid1, const GUID *guid2);
char *serializeGuid(GUID guid);
GUID deserializeGuid(const char *json);
char *encryptString(const char *plaintext);
char *decryptString(const char *encryptedText);
void saveStringToFile(const char *filename, const char *text);
char *readStringFromFile(const char *filename);
char *serializeTimeControl(TimeControl tc);
TimeControl deserializeTimeControl(const char *json);
char *serializeGame(Game game);
Game deserializeGame(char *json);
char *serializeGameNode(GameNode *node);
GameNode *deserializeGameNode(char *json);
char *serializeUser(User user);
User deserializeUser(char *json);
char *serializeUserNode(UserNode *node);
UserNode *deserializeUserNode(char *json);
char *serializeGameScore(GameScore gameScore);
GameScore deserializeGameScore(char *json);
char *serializeGameScoreNode(GameScoreNode *node);
GameScoreNode *deserializeGameScoreNode(char *json);
// EndPrototypes

int main()
{
    return 0;
}

// Functions
GUID generateGUID()
{
    GUID guid;

    srand(time(NULL));

    guid.data1 = rand();
    guid.data2 = rand() % 65536;
    guid.data3 = rand() % 65536;

    for (int i = 0; i < 8; ++i)
    {
        guid.data4[i] = rand() % 256;
    }

    return guid;
}

bool compareGUIDs(const GUID *guid1, const GUID *guid2)
{
    if (guid1->data1 != guid2->data1)
        return false;
    if (guid1->data2 != guid2->data2)
        return false;
    if (guid1->data3 != guid2->data3)
        return false;
    for (int i = 0; i < 8; i++)
    {
        if (guid1->data4[i] != guid2->data4[i])
            return false;
    }
    return true;
}
char *serializeGuid(GUID guid)
{
    int requiredSize = snprintf(NULL, 0, "{\"data1\":%lu,\"data2\":%hu,\"data3\":%hu,\"data4\":[%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu]}",
                                guid.data1, guid.data2, guid.data3,
                                guid.data4[0], guid.data4[1], guid.data4[2], guid.data4[3],
                                guid.data4[4], guid.data4[5], guid.data4[6], guid.data4[7]);

    char *jsonString = (char *)malloc((requiredSize + 1) * sizeof(char));
    if (jsonString != NULL)
    {
        sprintf(jsonString, "{\"data1\":%lu,\"data2\":%hu,\"data3\":%hu,\"data4\":[%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu]}",
                guid.data1, guid.data2, guid.data3,
                guid.data4[0], guid.data4[1], guid.data4[2], guid.data4[3],
                guid.data4[4], guid.data4[5], guid.data4[6], guid.data4[7]);
    }
    return jsonString;
}

GUID deserializeGuid(const char *json)
{
    GUID guid;
    sscanf(json, "{\"data1\":%lu,\"data2\":%hu,\"data3\":%hu,\"data4\":[%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu]}",
           &guid.data1, &guid.data2, &guid.data3,
           &guid.data4[0], &guid.data4[1], &guid.data4[2], &guid.data4[3],
           &guid.data4[4], &guid.data4[5], &guid.data4[6], &guid.data4[7]);
    return guid;
}
char *encryptString(const char *plaintext)
{
    int length = strlen(plaintext);
    char *encryptedText = (char *)malloc(sizeof(char) * (length + 1));

    for (int i = 0; i < length; i++)
    {
        encryptedText[i] = ((A * plaintext[i] + K) % CHAR_SET_SIZE);
    }
    encryptedText[length] = '\0';
    return encryptedText;
}

char *decryptString(const char *encryptedText)
{
    int length = strlen(encryptedText);
    char *decryptedText = (char *)malloc(sizeof(char) * (length + 1));

    int aInverse = 0;
    while (((A * aInverse) % CHAR_SET_SIZE) != 1)
    {
        aInverse++;
    }

    for (int i = 0; i < length; i++)
    {
        decryptedText[i] = (aInverse * (encryptedText[i] - K + CHAR_SET_SIZE)) % CHAR_SET_SIZE;
    }
    decryptedText[length] = '\0';
    return decryptedText;
}

void saveStringToFile(const char *filename, const char *text)
{
    FILE *file = fopen(filename, "wb");
    if (file != NULL)
    {
        fwrite(text, sizeof(char), strlen(text), file);
        fclose(file);
    }
    else
    {
        printf("Error opening file '%s' for writing!\n", filename);
    }
}

char *readStringFromFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    char *text = NULL;
    size_t len = 0;

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        rewind(file);

        text = (char *)malloc(sizeof(char) * (len + 1));
        if (text != NULL)
        {
            fread(text, sizeof(char), len, file);
            text[len] = '\0';
        }
        fclose(file);
    }
    else
    {
        printf("Error opening file '%s' for reading!\n", filename);
    }
    return text;
}

void transferFileContent(const char *sourceFilename, const char *destinationFilename)
{
    char *content = readStringFromFile(sourceFilename);
    if (content != NULL)
    {
        char *encryptedContent = encryptString(content);
        saveStringToFile(destinationFilename, encryptedContent);

        free(content);
        free(encryptedContent);
    }
    else
    {
        printf("Transfer failed: Unable to read content from '%s'\n", sourceFilename);
    }
}
char *serializeTimeControl(TimeControl tc)
{
    int requiredSize = snprintf(NULL, 0, "{ \"whitePassedTime\": %d, \"blackPassedTime\": %d, \"timeLimit\": %d, \"isTimed\": %s }",
                                tc.whitePassedTime, tc.blackPassedTime, tc.timeLimit, tc.isTimed ? "true" : "false");

    char *jsonString = (char *)malloc((requiredSize + 1) * sizeof(char));
    if (jsonString != NULL)
    {
        sprintf(jsonString, "{ \"whitePassedTime\": %d, \"blackPassedTime\": %d, \"timeLimit\": %d, \"isTimed\": %s }",
                tc.whitePassedTime, tc.blackPassedTime, tc.timeLimit, tc.isTimed ? "true" : "false");
    }
    return jsonString;
}
TimeControl deserializeTimeControl(const char *json)
{
    TimeControl tc;

    const char *delimiter = ":,";
    const char *token = strtok((char *)json, delimiter);

    while (token != NULL)
    {
        if (strstr(token, "whitePassedTime"))
        {
            token = strtok(NULL, delimiter);
            tc.whitePassedTime = atoi(token);
        }
        else if (strstr(token, "blackPassedTime"))
        {
            token = strtok(NULL, delimiter);
            tc.blackPassedTime = atoi(token);
        }
        else if (strstr(token, "timeLimit"))
        {
            token = strtok(NULL, delimiter);
            tc.timeLimit = atoi(token);
        }
        else if (strstr(token, "isTimed"))
        {
            token = strtok(NULL, delimiter);
            tc.isTimed = strcmp(token, "true") == 0;
        }

        token = strtok(NULL, delimiter);
    }

    return tc;
}
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
char *serializeUser(User user)
{
    int requiredSize = snprintf(NULL, 0, "{\"id\":%s,\"username\":\"%s\"}",
                                serializeGuid(user.id), user.username);

    char *jsonString = (char *)malloc((requiredSize + 1) * sizeof(char));
    if (jsonString != NULL)
    {
        sprintf(jsonString, "{\"id\":%s,\"username\":\"%s\"}",
                serializeGuid(user.id), user.username);
    }
    return jsonString;
}

User deserializeUser(char *json)
{
    User user;
    char *userid = (char *)malloc(sizeof(char) * 50);
    sscanf(json, "{\"id\":%s,\"username\":\"%s\"}",
           userid, user.username);
    user.id = deserializeGuid(userid);
    return user;
}
char *serializeUserNode(UserNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }

    int nodeCount = 0;
    UserNode *current = node;

    // Count the number of nodes in the list
    while (current != NULL)
    {
        nodeCount++;
        current = current->next;
    }

    int totalSize = nodeCount * (/* estimated size of serialized user + comma and brackets */ 100) + 3;
    char *jsonString = (char *)malloc(totalSize * sizeof(char));
    if (jsonString != NULL)
    {
        strcpy(jsonString, "[");

        current = node;
        while (current != NULL)
        {
            char *serializedUser = serializeUser(current->user);
            strcat(jsonString, serializedUser);
            free(serializedUser);

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

UserNode *deserializeUserNode(char *json)
{
    if (json == NULL || json[0] != '[')
    {
        return NULL;
    }

    UserNode *head = NULL;
    UserNode *current = NULL;

    // Move past the opening bracket
    json++;

    while (*json != '\0' && *json != ']')
    {
        // Find the start of a user object
        if (*json == '{')
        {
            char *end = strchr(json, '}');
            if (end == NULL)
            {
                // Invalid JSON format
                return NULL;
            }

            // Extract the user object
            int length = end - json + 1;
            char *userJson = (char *)malloc((length + 1) * sizeof(char));
            strncpy(userJson, json, length);
            userJson[length] = '\0';

            User user = deserializeUser(userJson);
            free(userJson);

            UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));
            newNode->user = user;
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

            // Move to the next potential user object
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

// EndFunctions