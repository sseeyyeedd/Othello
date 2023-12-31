#include "../include/guid.h"
#include <stdlib.h>

GUID generateGUID()
{
    GUID guid;

    srand(time(NULL));

    guid.data1 = rand();
    guid.data2 = rand() % 65536;
    guid.data3 = rand() % 65536;

    for (int i = 0; i < 8; ++i)
    {
        guid.data4[i] = rand() % 256;
    }

    return guid;
}

bool compareGUIDs(const GUID *guid1, const GUID *guid2)
{
    if (guid1->data1 != guid2->data1)
        return false;
    if (guid1->data2 != guid2->data2)
        return false;
    if (guid1->data3 != guid2->data3)
        return false;
    for (int i = 0; i < 8; i++)
    {
        if (guid1->data4[i] != guid2->data4[i])
            return false;
    }
    return true;
}
char *serializeGuid(GUID guid)
{
    int requiredSize = snprintf(NULL, 0, "{\"data1\":%lu,\"data2\":%hu,\"data3\":%hu,\"data4\":[%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu]}",
                                guid.data1, guid.data2, guid.data3,
                                guid.data4[0], guid.data4[1], guid.data4[2], guid.data4[3],
                                guid.data4[4], guid.data4[5], guid.data4[6], guid.data4[7]);

    char *jsonString = (char *)malloc((requiredSize + 1) * sizeof(char));
    if (jsonString != NULL)
    {
        sprintf(jsonString, "{\"data1\":%lu,\"data2\":%hu,\"data3\":%hu,\"data4\":[%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu]}",
                guid.data1, guid.data2, guid.data3,
                guid.data4[0], guid.data4[1], guid.data4[2], guid.data4[3],
                guid.data4[4], guid.data4[5], guid.data4[6], guid.data4[7]);
    }
    return jsonString;
}

GUID deserializeGuid(const char *json)
{
    GUID guid;
    sscanf(json, "{\"data1\":%lu,\"data2\":%hu,\"data3\":%hu,\"data4\":[%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu,%hhu]}",
           &guid.data1, &guid.data2, &guid.data3,
           &guid.data4[0], &guid.data4[1], &guid.data4[2], &guid.data4[3],
           &guid.data4[4], &guid.data4[5], &guid.data4[6], &guid.data4[7]);
    return guid;
}

