#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"

extern struct Token current_token;
void parser(FILE* file);
bool sekvencia(FILE* file);
bool letnutie(FILE* file);

#endif
