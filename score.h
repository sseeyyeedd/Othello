#ifndef SCORE_H
#define SCORE_H
#include"cJSON.h"
#include <stdlib.h>
#include<string.h>
typedef struct
{
    int score;
    char name[50];
} Score;
char* serializeScoreArray(const Score *array, int size) {
    cJSON *root = cJSON_CreateArray();
    for (int i = 0; i < size; ++i) {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "name", array[i].name);
        cJSON_AddNumberToObject(item, "score", array[i].score);
        cJSON_AddItemToArray(root, item);
    }

    char *jsonStr = cJSON_Print(root);
    cJSON_Delete(root);

    return jsonStr;
}
Score *deserializeScoreArray(const char *jsonStr, int *size) {
    cJSON *root = cJSON_Parse(jsonStr);
    if (root == NULL) {
        return NULL; 
    }

    *size = cJSON_GetArraySize(root);
    Score *array = (Score *)malloc(((*size)+2) * sizeof(Score));

    for (int i = 0; i < *size; ++i) {
        cJSON *item = cJSON_GetArrayItem(root, i);
        strcpy(array[i].name, cJSON_GetObjectItem(item, "name")->valuestring);
        array[i].score = cJSON_GetObjectItem(item, "score")->valueint;
    }
    cJSON_Delete(root);
    return array;
}
void AddScore(Score *scores,int *scoresCount,Score newScore){
    for (int i = 0; i < *scoresCount; i++)
    {
        if (strcmp(scores[i].name,newScore.name)==0)
        {
            scores[i].score+=newScore.score;
            return;
        }
    }
    scores[*scoresCount].score=newScore.score;
    strcpy(scores[*scoresCount].name,newScore.name); 
    (*scoresCount)+=1;
}
#endif