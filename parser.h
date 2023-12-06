#ifndef PARSER_H
#define PARSER_H
#include "scanner.h"
#include "precedence.h"

extern struct Token current_token;

void token_print();
void unget_token(struct Token, bool new_line);
bool returnovanie(char *name_of_node, bool *contains_return);
bool func_declar();
bool parametre(char *name_of_node);
bool parameter(char *name_of_node);
bool param_zostatok(char *name_of_node);
bool zbytok_param(char *name_of_node);
bool sipka_typ(char *name_of_node);
bool typ(char *name_of_node);
bool typ_of_param(char *name_of_node);
bool dvojbodka_typ(char *name_of_node);
bool priradenie_prave(char *name_of_node);
bool rovna_sa__priradenie(char* name_of_node);
bool letnutie();
bool varnutie();
bool relacia();
bool podmienka();
bool whilnutie(bool in_func, bool in_while, bool in_if);
bool ifnutie(bool in_func, bool in_while, bool in_if);
bool param_vol_zost(btree_node *temp, int* num_of_params);
bool parameter_volania(btree_node *temp, int* num_of_params);
bool parametre_volania(btree_node *temp, int* num_of_params);
bool priradenie_zost();
bool idnutie();
bool sekvencia(bool in_func, bool in_while, bool in_if);
void parser();
#endif
