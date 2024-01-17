#ifndef GAME_H
#define GAME_H
#include "disc.h"
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"

typedef struct
{
    int id;
    char *whitePlayer;
    char *blackPlayer;
    Disc Board[8][8];
    Disc Board_lw[8][8];
    Disc Board_llw[8][8];
    Disc Board_lb[8][8];
    Disc Board_llb[8][8];
    int whiteRemainingTime;
    int blackRemainingTime;
    int timeLimit;
    bool isTimed;
    int pastTwoMoveTimes[2];
    bool whiteToPlay;
    int whiteScores[32];
    int whiteMoveCount;
    int blackScores[32];
    int blackMoveCount;
    int whiteReturn;
    int blackReturn;
    bool IsCompleated;
} Game;
typedef struct
{
    int x;
    int y;
} Coordinates;
bool isLegalMove(Coordinates current, Disc Board[][8], Disc self);
bool isInBoard(int x, int y);
bool isBoardEmpty(Disc Board[][8]);
bool CheckPlayers(Game game, char p1[], char p2[]);
char *BoardToString(Disc Board[][8])
{
    char *str=(char*)malloc(sizeof(char)*65);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            str[i * 8 + j] = 48+Board[i][j];
        }
    }
    str[64] = '\0';
    return str;
}
void StringToBoard(Disc Board[][8], char *str)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Board[i][j] = (Disc)(str[i * 8 + j] - '0');
        }
    }
}
char *p2mToString(int p2m[])
{
    char *str=(char*)malloc(sizeof(char)*9);

    sprintf(str, "%04d%04d", p2m[0], p2m[1]);
    return str;
}
void stringTop2m(int p2m[], char *str)
{
    sscanf(str,"%04d%04d", &p2m[0], &p2m[1]);
}
char *ScoresToString(int scores[], int len)
{
    char *str=(char*)malloc(sizeof(char)*len * 2 + 1);
    for (int i = 0; i < len; i++)
    {
        char s[3];
        sprintf(s, "%02d", scores[i]);
        strcat(str, s);
    }
    str[len * 2] = '\0';
    return str;
}
void StringToScore(int scores[], int len, char *str)
{
    for (int i = 0; i < len; i++)
    {
        char substr[3];
        strncpy(substr, str + i * 2, 2);
        substr[2] = '\0';
        scores[i] = atoi(substr);
    }
}
int scoreSum(int scores[], int count)
{
    int sum = 0;
    for (int i = 0; i < count; i++)
    {
        sum += scores[i];
    }
    return sum;
}
void copyBoard(Disc destination[][8], const Disc source[][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            destination[i][j] = source[i][j];
        }
    }
}
void newPM(int array[], int value)
{
    array[1] = array[0];
    array[0] = value;
}
bool isBoardEmpty(Disc Board[][8])
{
    if (Board[3][3] == None)
    {
        return true;
    }
    return false;
}
int CountDiscs(Disc Board[][8],Disc disc){
    int counter=0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (Board[i][j]==disc)
            {
                
                counter++;
            }
            
        }
        
    }
    return counter;
    
}
void turnDiscs(Game *game, Coordinates move)
{
    int score = 0;
    Disc opponent = game->whiteToPlay ? Black : White;
    Disc self = game->whiteToPlay ? White : Black;
    game->Board[move.x][move.y] = self;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < 8; i++)
    {
        bool selfExist = false;
        for (int j = 1; j < 8; j++)
        {
            if (!isInBoard(move.x + (dx[i] * j), move.y + (dy[i] * j)))
            {
                break;
            }
            else
            {
                if (game->Board[move.x + (dx[i] * j)][move.y + (dy[i] * j)] == None)
                {
                    break;
                }
                else if (game->Board[move.x + (dx[i] * j)][move.y + (dy[i] * j)] == self)
                {
                    selfExist = true;
                    break;
                }
            }
        }
        if (selfExist)
        {
            int newX = move.x + dx[i];
            int newY = move.y + dy[i];
            while (isInBoard(newX, newY) && game->Board[newX][newY] == opponent)
            {
                score++;
                game->Board[newX][newY] = self;
                newX += dx[i];
                newY += dy[i];
            }
        }
    }
    if (game->whiteToPlay)
    {
        game->whiteScores[game->whiteMoveCount] = score;
        game->whiteMoveCount++;
    }
    else
    {
        game->blackScores[game->blackMoveCount] = score;
        game->blackMoveCount++;
    }
}
void printCurrentGame(Game game)
{
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j < 35; j++)
        {
            if (i % 3 == 1 && j != 0 && j != 34 && j % 4 != 1)
            {
                printf("_");
            }
            else if (j % 4 == 1 && i > 1)
            {
                printf("|");
            }
            else if (j == 0 && i % 3 == 0)
            {
                if (i / 3 != 0)
                {
                    printf("%c", (i / 3) + 96);
                }
            }
            else if (i == 0 && j % 4 == 0)
            {
                printf("%d", (j / 4));
            }
            else if (j % 4 == 3 && i % 3 == 0 && j != 34 && i != 0)
            {

                if (game.Board[i / 3 - 1][j / 4] != None)
                {
                    printf("%c", game.Board[i / 3 - 1][j / 4] == Black ? 'O' : '#');
                }
                else
                {
                    printf(" ");
                }
            }
            else if (j == 34)
            {
                switch (i)
                {
                case 8:
                    printf(" %s(O) Discs:%d", game.blackPlayer, CountDiscs(game.Board,Black));
                    break;
                case 9:
                    printf(" %s(#) Discs:%d", game.whitePlayer, CountDiscs(game.Board,White));
                    break;
                case 10:
                    printf("-----------------");
                    break;
                case 11:
                    printf(" %s to play.", game.whiteToPlay ? game.whitePlayer : game.blackPlayer);
                    break;
                case 12:
                    printf("-----------------");
                    break;
                case 13:
                    if (game.isTimed)
                    {
                        int time = game.blackRemainingTime;
                        printf(" %s(O) remaining time: %dm %ds", game.blackPlayer, time / 60, time % 60);
                    }
                    break;
                case 14:
                    if (game.isTimed)
                    {
                        int time = game.whiteRemainingTime;
                        printf(" %s(#) remaining time: %dm %ds", game.whitePlayer, time / 60, time % 60);
                    }

                    break;
                case 15:
                    if (game.isTimed && game.blackReturn > 0)
                    {
                        printf(" %s(O) can enter O to return; %d returns left.", game.blackPlayer, game.blackReturn);
                    }
                    break;
                case 16:
                    if (game.isTimed && game.whiteReturn > 0)
                    {
                        printf(" %s(#) can enter # to return; %d returns left.", game.whitePlayer, game.whiteReturn);
                    }
                    break;
                case 17:
                    if (game.isTimed)
                    {
                        printf("-----------------");
                    }
                    break;
                default:
                    break;
                }
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}
int GetLegalMoves(Coordinates legals[], Game game)
{
    int output = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Coordinates current = {.x = i, .y = j};
            if (isLegalMove(current, game.Board, game.whiteToPlay ? White : Black))
            {
                legals[output] = current;
                output++;
            }
        }
    }
    return output;
}
bool isLegalMove(Coordinates current, Disc Board[][8], Disc self)
{
    Disc opponent = self == White ? Black : White;
    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    if (Board[current.x][current.y] != None)
    {
        return false;
    }

    for (int dir = 0; dir < 8; dir++)
    {
        int newX = current.x + dx[dir];
        int newY = current.y + dy[dir];
        if (isInBoard(newX, newY) && Board[newX][newY] == opponent)
        {
            newX += dx[dir];
            newY += dy[dir];
            while (isInBoard(newX, newY))
            {
                if (Board[newX][newY] == self)
                {
                    return true;
                }
                newX += dx[dir];
                newY += dy[dir];
            }
        }
    }
    return false;
}
bool isInBoard(int x, int y)
{
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        return true;
    }
    return false;
}
int Move(Game *game, Coordinates legalMoves[], int legalMoveCount, int moveStatus)
{

    time_t passedTime;
    if (game->isTimed)
    {
        passedTime = clock();
    }
    char input[3];
    do
    {
        scanf("%s", input);
        if (input[1] != '\0')
        {
            Coordinates mc;
            if (input[0] > '`')
            {
                mc.x = input[0] - 'a';
            }
            else
            {
                mc.x = input[0] - 'A';
            }
            sscanf(&input[1], "%d", &mc.y);
            mc.y--;
            for (int i = 0; i < legalMoveCount; i++)
            {
                if (legalMoves[i].x == mc.x && legalMoves[i].y == mc.y)
                {
                    if (game->whiteToPlay)
                    {
                        copyBoard(game->Board_llw, game->Board_lw);
                        copyBoard(game->Board_lw, game->Board);
                    }
                    else
                    {
                        copyBoard(game->Board_llb, game->Board_lb);
                        copyBoard(game->Board_lb, game->Board);
                    }

                    turnDiscs(game, mc);
                    if (game->isTimed)
                    {
                        passedTime = clock() - passedTime;

                        if (game->whiteToPlay)
                        {
                            game->whiteRemainingTime -= (int)passedTime / CLOCKS_PER_SEC;
                        }
                        else
                        {
                            game->blackRemainingTime -= (int)passedTime / CLOCKS_PER_SEC;
                        }
                        newPM(game->pastTwoMoveTimes, passedTime);
                    }
                    game->whiteToPlay = !game->whiteToPlay;

                    return 0;
                }
            }
            if (game->isTimed)
            {
                if (game->whiteToPlay)
                {
                    if (game->whiteRemainingTime-(int)(clock() - passedTime)/ CLOCKS_PER_SEC<=0)
                    {
                        return 0;
                    }
                    
                }else
                {
                     if (game->blackRemainingTime-(int)(clock() - passedTime)/ CLOCKS_PER_SEC<=0)
                    {
                        return 0;
                    }
                }
                
                
            }
            
            printf("That move is not legal!\n");
        }
        else
        {
            switch (input[0])
            {
            case '#':
                if (game->isTimed && !isBoardEmpty(game->Board_llw) && moveStatus != 1)
                {
                    if (game->whiteToPlay)
                    {
                        passedTime = clock() - passedTime;
                        game->whiteRemainingTime -= (int)passedTime / CLOCKS_PER_SEC;
                        newPM(game->pastTwoMoveTimes, (int)passedTime / CLOCKS_PER_SEC);
                    }
                    switch (game->whiteReturn)
                    {
                    case 2:
                        game->whiteRemainingTime -= 20;
                        break;
                    case 1:
                        game->whiteRemainingTime -= 40;
                        game->blackRemainingTime += !game->whiteToPlay ? (game->pastTwoMoveTimes[0] + game->pastTwoMoveTimes[1]) : game->pastTwoMoveTimes[0];
                        break;
                    }
                    game->blackMoveCount -= !game->whiteToPlay ? 2 : 1;
                    game->whiteToPlay = true;
                    copyBoard(game->Board, game->Board_lw);
                    copyBoard(game->Board_lw, game->Board_llw);
                    game->whiteReturn--;
                    // make ll empty if logical errors happend
                    return 1;
                }
                else
                {
                    printf("You can't return right now!\n");
                }

                break;
            case 'O':
                if (game->isTimed && !isBoardEmpty(game->Board_llb) && moveStatus != 1)
                {
                    if (!game->whiteToPlay)
                    {
                        passedTime = clock() - passedTime;
                        game->blackRemainingTime -= (int)passedTime / CLOCKS_PER_SEC;
                        newPM(game->pastTwoMoveTimes, (int)passedTime / CLOCKS_PER_SEC);
                    }
                    switch (game->blackReturn)
                    {
                    case 2:
                        game->blackRemainingTime -= 20;
                        break;
                    case 1:
                        game->blackRemainingTime -= 40;
                        game->whiteRemainingTime += game->whiteToPlay ? (game->pastTwoMoveTimes[0] + game->pastTwoMoveTimes[1]) : game->pastTwoMoveTimes[0];
                        break;
                    }
                    game->blackMoveCount -= game->whiteToPlay ? 2 : 1;
                    game->whiteToPlay = false;
                    copyBoard(game->Board, game->Board_lb);
                    copyBoard(game->Board_lb, game->Board_llb);
                    game->blackReturn--;
                    // make ll empty if logical errors happend
                    return 1;
                }
                else
                {
                    printf("You can't return right now!\n");
                }
                break;
            case 'X':
                printf("Enter X again to save and exit or anything else to continue:\n");
                char txt[2];
                scanf("%s", txt);
                if (txt[0] == 'X')
                {
                    if (game->isTimed)
                    {
                        passedTime = clock() - passedTime;

                        if (game->whiteToPlay)
                        {
                            game->whiteRemainingTime -= (int)passedTime / CLOCKS_PER_SEC;
                        }
                        else
                        {
                            game->blackRemainingTime -= (int)passedTime / CLOCKS_PER_SEC;
                        }
                        newPM(game->pastTwoMoveTimes, (int)passedTime / CLOCKS_PER_SEC);
                    }

                    return -1;
                }
                else
                {
                    printf("Game will be continued!\n");
                    break;
                }

                break;
            default:
                printf("Invalid input!\n");
                break;
            }
        }
    } while (true);
    return 0;
}

char *serializeGameArray(Game games[], int numGames)
{
    cJSON *root = cJSON_CreateArray();

    for (int i = 0; i < numGames; i++)
    {
        cJSON *gameObject = cJSON_CreateObject();

        cJSON_AddNumberToObject(gameObject, "id", games[i].id);
        cJSON_AddStringToObject(gameObject, "whitePlayer", games[i].whitePlayer);
        cJSON_AddStringToObject(gameObject, "blackPlayer", games[i].blackPlayer);
        cJSON_AddStringToObject(gameObject, "board", BoardToString(games[i].Board));
        cJSON_AddStringToObject(gameObject, "board_lb", BoardToString(games[i].Board_lb));
        cJSON_AddStringToObject(gameObject, "board_lw", BoardToString(games[i].Board_lw));
        cJSON_AddStringToObject(gameObject, "board_llb", BoardToString(games[i].Board_llb));
        cJSON_AddStringToObject(gameObject, "board_llw", BoardToString(games[i].Board_llw));
        cJSON_AddNumberToObject(gameObject, "whiteRemainingTime", games[i].whiteRemainingTime);
        cJSON_AddNumberToObject(gameObject, "blackRemainingTime", games[i].blackRemainingTime);
        cJSON_AddNumberToObject(gameObject, "timeLimit", games[i].timeLimit);
        cJSON_AddBoolToObject(gameObject, "isTimed", games[i].isTimed);
        cJSON_AddBoolToObject(gameObject, "IsCompleated", games[i].IsCompleated);
        cJSON_AddStringToObject(gameObject, "pastTwoMoveTimes", p2mToString(games[i].pastTwoMoveTimes));
        cJSON_AddBoolToObject(gameObject, "whiteToPlay", games[i].whiteToPlay);
        cJSON_AddStringToObject(gameObject, "whiteScores", ScoresToString(games[i].whiteScores, games[i].whiteMoveCount));
        cJSON_AddStringToObject(gameObject, "blackScores", ScoresToString(games[i].blackScores, games[i].blackMoveCount));
        cJSON_AddNumberToObject(gameObject, "whiteMoveCount", games[i].whiteMoveCount);
        cJSON_AddNumberToObject(gameObject, "blackMoveCount", games[i].blackMoveCount);
        cJSON_AddNumberToObject(gameObject, "whiteReturn", games[i].whiteReturn);
        cJSON_AddNumberToObject(gameObject, "blackReturn", games[i].blackReturn);
        cJSON_AddItemToArray(root, gameObject);
    }
    char *jsonString = cJSON_Print(root);
    cJSON_Delete(root);
    return jsonString;
}
Game *deserializeGameArray(char *jsonString, int *numGames)
{
    cJSON *root = cJSON_Parse(jsonString);

    if (root == NULL)
    {
        return NULL;
    }

    *numGames = cJSON_GetArraySize(root);
    Game *games = (Game *)malloc(((*numGames) + 1) * sizeof(Game));
    for (int i = 0; i < *numGames; i++)
    {
        cJSON *gameObject = cJSON_GetArrayItem(root, i);
        games[i].whitePlayer =(char*) malloc(sizeof(char) * 50);
        games[i].blackPlayer =(char*) malloc(sizeof(char) * 50);
        games[i].id = cJSON_GetObjectItem(gameObject, "id")->valueint;
        strcpy(games[i].whitePlayer, cJSON_GetObjectItem(gameObject, "whitePlayer")->valuestring);
        strcpy(games[i].blackPlayer, cJSON_GetObjectItem(gameObject, "blackPlayer")->valuestring);
        StringToBoard(games[i].Board, cJSON_GetObjectItem(gameObject, "board")->valuestring);
        StringToBoard(games[i].Board_lw, cJSON_GetObjectItem(gameObject, "Board_lw")->valuestring);
        StringToBoard(games[i].Board_llw, cJSON_GetObjectItem(gameObject, "Board_llw")->valuestring);
        StringToBoard(games[i].Board_lb, cJSON_GetObjectItem(gameObject, "Board_lb")->valuestring);
        StringToBoard(games[i].Board_llb, cJSON_GetObjectItem(gameObject, "Board_llb")->valuestring);
        games[i].whiteRemainingTime = cJSON_GetObjectItem(gameObject, "whiteRemainingTime")->valueint;
        games[i].blackRemainingTime = cJSON_GetObjectItem(gameObject, "blackRemainingTime")->valueint;
        games[i].timeLimit = cJSON_GetObjectItem(gameObject, "timeLimit")->valueint;
        games[i].isTimed = cJSON_GetObjectItem(gameObject, "isTimed")->valueint;
        games[i].IsCompleated = cJSON_GetObjectItem(gameObject, "IsCompleated")->valueint;
        stringTop2m(games[i].pastTwoMoveTimes, cJSON_GetObjectItem(gameObject, "pastTwoMoveTimes")->valuestring);
        games[i].whiteToPlay = cJSON_GetObjectItem(gameObject, "whiteToPlay")->valueint;
        games[i].whiteMoveCount = cJSON_GetObjectItem(gameObject, "whiteMoveCount")->valueint;
        games[i].blackMoveCount = cJSON_GetObjectItem(gameObject, "blackMoveCount")->valueint;
        StringToScore(games[i].whiteScores, games[i].whiteMoveCount, cJSON_GetObjectItem(gameObject, "whiteScores")->valuestring);
        StringToScore(games[i].blackScores, games[i].blackMoveCount, cJSON_GetObjectItem(gameObject, "blackScores")->valuestring);
        games[i].whiteReturn = cJSON_GetObjectItem(gameObject, "whiteReturn")->valueint;
        games[i].blackReturn = cJSON_GetObjectItem(gameObject, "blackReturn")->valueint;
    }

    cJSON_Delete(root);
    return games;
}
void copyGame(Game *dest, const Game *src)
{
    dest->id = src->id;
    dest->whitePlayer =(char*) malloc(sizeof(char) * 50);
    dest->blackPlayer =(char*) malloc(sizeof(char) * 50);
    strcpy(dest->whitePlayer, src->whitePlayer);
    strcpy(dest->blackPlayer, src->blackPlayer);
    copyBoard(dest->Board, src->Board);
    copyBoard(dest->Board_lw, src->Board_lw);
    copyBoard(dest->Board_llw, src->Board_llw);
    copyBoard(dest->Board_lb, src->Board_lb);
    copyBoard(dest->Board_llb, src->Board_llb);
    dest->whiteRemainingTime = src->whiteRemainingTime;
    dest->blackRemainingTime = src->blackRemainingTime;
    dest->timeLimit = src->timeLimit;
    dest->isTimed = src->isTimed;
    memcpy(dest->pastTwoMoveTimes, src->pastTwoMoveTimes, sizeof(src->pastTwoMoveTimes));
    dest->whiteToPlay = src->whiteToPlay;
    memcpy(dest->whiteScores, src->whiteScores, sizeof(src->whiteScores));
    dest->whiteMoveCount = src->whiteMoveCount;
    memcpy(dest->blackScores, src->blackScores, sizeof(src->blackScores));
    dest->blackMoveCount = src->blackMoveCount;
    dest->whiteReturn = src->whiteReturn;
    dest->blackReturn = src->blackReturn;
    dest->isCompleated=src->isCompleated
}
bool CheckPlayers(Game game, char p1[], char p2[])
{
    if ((strcmp(game.whitePlayer, p1) == 0 || strcmp(game.whitePlayer, p2) == 0) && (strcmp(game.blackPlayer, p1) == 0 || strcmp(game.blackPlayer, p2) == 0))
    {
        return true;
    }
    return false;
}
Game *FindGameById(Game games[], int count, int id)
{
    for (int i = 0; i < count; i++)
    {
        if (games[i].id == id)
        {
            return &games[i];
        }
    }
    return NULL;
}
int newId(Game games[], int count)
{
    int max = 0;
    for (int i = 0; i < count; i++)
    {
        if (games[i].id > max)
        {
            max = games[i].id;
        }
    }
    return max + 1;
}
void AddGame(Game *game, Game games[], int *count)
{
    if (game->id == 0)
    {
        game->id = newId(games, *count);
        copyGame(&games[*count], game);
        (*count) += 1;
    }
    else
    {
        Game *g =(Game*) malloc(sizeof(Game));
        g = FindGameById(games, *count, game->id);
        copyGame(g, game);
    }
}
#endif