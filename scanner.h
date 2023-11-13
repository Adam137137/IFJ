#include "compiler.h"

#define key_words_length 11
#define built_in_functions_length 10
char *key_words[] = {"Double", "else", "func", "if", "Int", "let", "nil", "return", "String", "var", "while"};

char *built_in_functions[] = {"readString", "readInt", "readDouble", "write", "Int2Double", "Double2Int", "length", "substring", "ord", "chr"};



void string_reset(char*);
char getChar(int*);
void parser();