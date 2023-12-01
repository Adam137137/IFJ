#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "precedence.h"

extern struct Token current_token;
void parser();
void token_print();
void unget_token(struct Token);

bool sekvencia();
bool func_declar();
bool parametre(char *name_of_node);
bool parameter(char *name_of_node);
bool param_zostatok(char *name_of_node);
bool zbytok_param(char *name_of_node);
bool sipka_typ(char *name_of_node);
bool typ(char *name_of_node);
bool typ_of_param(char *name_of_node);
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
bool dvojbodka_typ(char *name_of_node);
bool typ(char *name_of_node);

#endif
