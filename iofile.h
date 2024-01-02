#ifndef IOFILE_H
#define IOFILE_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define CHAR_SET_SIZE 256
#define A 2  // 4-0+2-5+2-1+1-9+8
#define K 32 // 4+0+2+5+2+1+1+9+8
char *encryptString(const char *plaintext)
{
    int length = strlen(plaintext);
    char *encryptedText = (char *)malloc(sizeof(char) * (length + 1));

    for (int i = 0; i < length; i++)
    {
        encryptedText[i] = ((A * plaintext[i] + K) % CHAR_SET_SIZE);
    }
    encryptedText[length] = '\0';
    return encryptedText;
}

char *decryptString(const char *encryptedText)
{
    int length = strlen(encryptedText);
    char *decryptedText = (char *)malloc(sizeof(char) * (length + 1));

    int aInverse = 0;
    while (((A * aInverse) % CHAR_SET_SIZE) != 1)
    {
        aInverse++;
    }

    for (int i = 0; i < length; i++)
    {
        decryptedText[i] = (aInverse * (encryptedText[i] - K + CHAR_SET_SIZE)) % CHAR_SET_SIZE;
    }
    decryptedText[length] = '\0';
    return decryptedText;
}

void saveStringToFile(const char *filename, const char *text)
{
    FILE *file = fopen(filename, "wb");
    if (file != NULL)
    {
        fwrite(text, sizeof(char), strlen(text), file);
        fclose(file);
    }
    else
    {
        printf("Error opening file '%s' for writing!\n", filename);
    }
}

char *readStringFromFile(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    char *text = NULL;
    size_t len = 0;

    if (file != NULL)
    {
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        rewind(file);

        text = (char *)malloc(sizeof(char) * (len + 1));
        if (text != NULL)
        {
            fread(text, sizeof(char), len, file);
            text[len] = '\0';
        }
        fclose(file);
    }
    else
    {
        printf("Error opening file '%s' for reading!\n", filename);
    }
    return text;
}
#endif