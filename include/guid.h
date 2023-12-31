#ifndef GUID_H
#define GUID_H
#include <stdbool.h>
typedef struct
{
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
} GUID;
GUID generateGUID();
bool compareGUIDs(const GUID *guid1, const GUID *guid2);
char *serializeGuid(GUID guid);
GUID deserializeGuid(const char *json);
#endif