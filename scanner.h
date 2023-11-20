#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


#define key_words_length 14
#define built_in_functions_length 10
extern char *built_in_functions[];
extern char *key_words[];

struct Token
{
    int type;
    char *attribute;
    bool first_in_line;
};

struct Token getNextToken();
void string_reset(char*);
char getChar(int*);

#endif