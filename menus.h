#ifndef MENUS_H
#define MENUS_H
#include "game.h"
#include "score.h"
#include <stdio.h>
#include <string.h>
void userMenu(Game *game)
{
    char fp[50];
    char sp[50];
    printf("Enter first player's name:\n");
    scanf("%s", fp);
    printf("Enter secound player's name:\n");
    scanf("%s", sp);
    strcpy(game->blackPlayer,fp);
    strcpy(game->whitePlayer,sp);
     
}
int gameSelector(Game games[], int gamesCount, Game *game,Score *scores,int scoresCount)
{
    while (true)
    {
        int input;
    printf("Unfinished games:\n");
    // Unfinished games
    printf("Enter 0 to start new game,-1 to see scores or enter game id to continue:\n");
    scanf("%d", &input);
    if (input == 0)
    {
        return 0;
    }
    else if(input==-1)
    {
        PrintScores(scores,scoresCount);
    }
    else
    {
        for (int i = 0; i < gamesCount; i++)
        {
            if (games[i].id == input)
            {
                *game = games[i];
                return 1;
            }
        }
        return -1;
    }
    }
    
    
}
void PrintScores(Score* scores,int scoresCount){
    if (scoresCount<1)
    {
        printf("No scores found!\n");
    }
    
    for (int i = 0; i < scoresCount; i++)
    {
        printf("%s : %d\n",scores[i].name,scores[i].score);
    }
    
}
void isTimedGame(Game *game)
{
    game->Board[3][3]=White;
    game->Board[4][4]=White;
    game->Board[3][4]=Black;
    game->Board[4][3]=Black;
    
    do
    {
        printf("Enter 0 to play Normally\n Or enter a number between 1 to 30(for timed game time limit in minutes):\n");
        int input;
        scanf("%d", &input);
        if (input == 0)
        {
            game->isTimed = false;
            return;
        }
        else if (input > 0 && input <= 30)
        {
            game->isTimed=true;
            game->timeLimit=input*60;
            game->blackRemainingTime=input*60;
            game->whiteRemainingTime=input*60;
            return;
        }
        else
        {
            printf("Invalid input!!");
        }
    } while (true);
}
#endif