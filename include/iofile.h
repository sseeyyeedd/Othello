#ifndef IOFILE_H
#define IOFILE_H
char *encryptString(const char *plaintext);
char *decryptString(const char *encryptedText);
void saveStringToFile(const char *filename, const char *text);
char *readStringFromFile(const char *filename);
#endif