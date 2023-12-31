#ifndef TIMECONTROL_H
#define TIMECONTROL_H
typedef struct
{
    int whitePassedTime;
    int blackPassedTime;
    int timeLimit;
    bool isTimed;
} TimeControl;
char *serializeTimeControl(TimeControl tc);
TimeControl deserializeTimeControl(const char *json);
#endif