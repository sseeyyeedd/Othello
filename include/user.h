#ifndef USER_H
#define USER_H
#include "guid.h"
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
char *serializeUser(User user);
User deserializeUser(char *json);
char *serializeUserNode(UserNode *node);
UserNode *deserializeUserNode(char *json);
#endif