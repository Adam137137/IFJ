#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"

extern struct Token current_token;
void parser();
bool sekvencia();
bool letnutie();
bool parameter_volania();
bool param_vol_zost();
bool priradenie_prave();

#endif
