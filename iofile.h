#ifndef IOFILE_H
#define IOFILE_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define CHAR_SET_SIZE 256
#define A 3  // (4+0+2+5+2+1+1+9+8)/10
#define K 32 // 4+0+2+5+2+1+1+9+8
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; 
}
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

    int aInverse = modInverse(A, CHAR_SET_SIZE);
    if (aInverse == -1) {
        printf("Multiplicative inverse does not exist for given A and character set size.\n");
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        decryptedText[i] = (aInverse * (encryptedText[i] - K + CHAR_SET_SIZE)) % CHAR_SET_SIZE;
    }
    decryptedText[length] = '\0';
    return decryptedText;
}

void saveStringToFile(const char *filename, const char *text)
{
    FILE *file = fopen(filename, "wb");
    fwrite(encryptString(text), sizeof(char), strlen(text), file);
    fclose(file);
}

char *readStringFromFile(const char *filename,char* defaultText)
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
        
        saveStringToFile(filename,strcat(defaultText,"\0"));
        return defaultText;
    }
    return decryptString(text);
}
#endif