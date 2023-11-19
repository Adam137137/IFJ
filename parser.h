#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "precedence.h"

extern struct Token current_token;
void parser();

void unget_token(struct Token);

bool sekvencia();
bool func_declar();
bool parametre();
bool parameter();
bool param_zostatok();
bool zbytok_param();
bool sipka_typ();
bool typ();

bool letnutie();
bool parameter_volania();
bool param_vol_zost();
bool priradenie_prave();

#endif
