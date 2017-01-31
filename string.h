#ifndef _STRING_
#define _STRING_
#include <stdint.h>

//up to 10 args
#define ROWS 10
//int16 numbers read in (+-~32500) all commands less than 5 chars
#define COLS 15
//macro to translate the value of a number char to an int
#define charToInt(c) ((int)c-48)

int8_t strcomp(const char*, const char*);

uint16_t LengthOfString(const char*);

void strtok(const char*, char[ROWS][COLS], const char*);

int8_t firstIndex(const char* str, const char* delim);

int16_t strToInt(const char*);

char* itoa(int , char* , int , int);

#endif //String