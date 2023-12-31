#include "../include/timeControl.h"
#include <stdlib.h>
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