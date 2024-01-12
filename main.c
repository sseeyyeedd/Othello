#include "iofile.h"
#include "game.h"
#include "disc.h"
#include "menus.h"
#include "score.h"
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
int main()
{
    char gameScoresFile[] = "gameScores.oth";
    char gamesFile[]= "games.oth";

    Game games[10];
    Disc Board[8][8] = {None};
    Coordinates legalMoves[30];
    int statusCheck = 0;
    int legalMoveCount = 0;
    int blackScores[32] = {0};
    int whiteScores[32] = {0};
    int p2m[2] = {0};
    Game currentGame = {.id = 0, .whitePlayer = (char *)malloc(50 * sizeof(char)), .blackPlayer = (char *)malloc(50 * sizeof(char)), .whiteRemainingTime = 0, .blackRemainingTime = 0, .timeLimit = 0, .isTimed = false, .whiteToPlay = false, .whiteScores = whiteScores, .whiteMoveCount = 0, .blackScores = blackScores, .blackMoveCount = 0, .whiteReturn = 2, .blackReturn = 2};
    copyBoard(currentGame.Board, Board);
    for (int i = 0; i < 2; i++)
    {
        currentGame.pastTwoMoveTimes[i] = p2m[i];
    }

    userMenu(&currentGame);
    system("cls");
    while (true)
    {
        int scoresCount=0;
        Score* Scores=deserializeScoreArray(readStringFromFile(gameScoresFile,"[]"),&scoresCount);
        int modeIndicator = -1;
        do
        {
            modeIndicator = gameSelector(games, 10, &currentGame,Scores,scoresCount);
            if (modeIndicator == -1)
            {
                printf("Invalid game Id!\n");
            }

        } while (modeIndicator == -1);
        system("cls");
        if (modeIndicator == 0)
        {
            isTimedGame(&currentGame);
        }
        int moveStatus = 0;
        while (true)
        {
            system("cls");
            legalMoveCount = GetLegalMoves(legalMoves, currentGame);
            if (legalMoveCount == 0)
            {
                currentGame.whiteToPlay = !currentGame.whiteToPlay;
                statusCheck++;
                if (statusCheck == 2)
                {
                    // Game Finished
                    break;
                }
                continue;
            }
            else
            {
                statusCheck = 0;
            }
            printCurrentGame(currentGame);

            moveStatus = Move(&currentGame, legalMoves, legalMoveCount, moveStatus);
            if (currentGame.isTimed)
            {

                if (currentGame.whiteToPlay)
                {

                    if (currentGame.blackRemainingTime <= 0)
                    {
                        Score s={.score=scoreSum(currentGame.whiteScores,currentGame.whiteMoveCount)};
                        strcpy(s.name,currentGame.whitePlayer);
                        AddScore(Scores,&scoresCount,s);
                        saveStringToFile(gameScoresFile,serializeScoreArray(Scores,scoresCount));
                        break;
                    }
                }
                else
                {

                    if (currentGame.whiteRemainingTime <= 0)
                    {
                        printf("FFFFFFFFF\n");
                         Score s={.score=scoreSum(currentGame.blackScores,currentGame.blackMoveCount)};
                        strcpy(s.name,currentGame.blackPlayer);
                        AddScore(Scores,&scoresCount,s);
                        saveStringToFile(gameScoresFile,serializeScoreArray(Scores,scoresCount));
                        break;
                    }
                }
            }

            if (moveStatus == -1)
            {
                // Save and go to main menu
                break;
            }
        }
    }

    return 0;
}