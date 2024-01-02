#ifndef USER_H
#define USER_H
#include "guid.h"
#include <stdlib.h>
typedef struct
{
    GUID id;
    char username[50];
} User;
typedef struct UserNode
{
    User user;
    struct UserNode *next;
} UserNode;
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
#endif