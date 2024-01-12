#ifndef GAME_H
#define GAME_H
#include "disc.h"
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>

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
} Game;
typedef struct
{
    int x;
    int y;
} Coordinates;
bool isLegalMove(Coordinates current, Disc Board[][8], Disc self);
bool isInBoard(int x, int y);
bool isBoardEmpty(Disc Board[][8]);
char *BoardToString(Disc Board[][8])
{
    char str[65];
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            str[i * 8 + j] = (char)Board[i][j];
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
            Board[i][j] = (Disc)[i * 8 + j] - '0';
        }
    }
}
char *p2mToString(int p2m[])
{
    char str1[5];
    char str2[5];
    sprintf(str1, "%04d", p2m[0]);
    sprintf(str2, "%04d", p2m[1]);
    return strcat(str1, str2);
}
void stringTop2m(int p2m[], char *str)
{
    sscanf("%04d%04d", &p2m[0], &p2m[1]);
}
char *ScoresToString(int scores[], int len)
{
    char str[len * 2 + 1];
    for (int i = 0; i < len; i++)
    {
        char s[3];
        sprintf(s, "%02d", scores[i]);
        strcat(str, s);
    }
    str[len * 2]='\0';
    return str;
}
void StringToScore(int scores[], int len, char* str) {
    for (int i = 0; i < len; i++) {
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
                    printf(" %s(O) Discs:%d", game.blackPlayer, 10);
                    break;
                case 9:
                    printf(" %s(#) Discs:%d", game.whitePlayer, 10);
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
            if (input[0] > 'a')
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
                    printf("You can't return right now!");
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
                    printf("You can't return right now!");
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
#endif