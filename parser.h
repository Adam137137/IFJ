#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "precedence.h"

extern struct Token current_token;
void parser();
void token_print();
void unget_token(struct Token);

bool sekvencia();
bool func_declar(bool);
bool parametre();
bool parameter();
bool param_zostatok();
bool zbytok_param();
bool sipka_typ();
bool typ();
bool ifnutie();
bool podmienka();
bool relacia();

bool parametre_volania();
bool letnutie();
bool varnutie();
bool whilnutie();
bool parameter_volania();
bool param_vol_zost();
bool priradenie_prave();
bool returnovanie();
bool idnutie();
bool priradenie_zost();
bool rovna_sa__priradenie();
bool dvojbodka_typ();
bool typ();

#endif
