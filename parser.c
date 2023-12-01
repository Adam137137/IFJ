/*
 * Projekt: Implementace překladače imperativního jazyka IFJ23
 *
 * autori suboru:
 * 247112 - Peter Gvozdjak
 * xvarsa01 - Adam Varsanyi
 * xperec00 - Mario Perecz
 * 
*/

#include "parser.h"
#include "compiler.h"

bool dvojbodka_typ_neni = false;
bool return_neni = false;
struct Token current_token, current_token2;

bool ladenie = 1;
void token_print(){             // ladenie zapnut ! ! !
    if (ladenie)
    {
        printf("%s\n", current_token.attribute);
    }
}

void unget_token(struct Token token){
    if (token.type == 0){
        ungetc(EOF, file);
    }
    else{
        for (int i = strlen(token.attribute) - 1; i >= 0; i--) {
            ungetc((unsigned char)token.attribute[i], file);
        }
    }
}
bool returnovanie(){
    if (current_token.type == 4 && strcmp(current_token.attribute,"return") == 0){
        current_token = getNextToken();             // prvy token do precedencnej
        return reduce_exp();
    }
    else if (current_token.type == 23){             //epsilon prechod
        unget_token(current_token);
        return true;
    }
    return false;
}
bool func_declar(){
    return_neni = true;
    current_token = getNextToken();
    char *name_of_node = string_dup(current_token.attribute);
    if (current_token.type != 1){               // id
        return false;
    }

    if (!prvy_prechod){
        DLL_InsertFirst2(&symtable_stack);
    }
    insert_func(&symtable_stack.firstElement->treeRoot, name_of_node, 1);


    current_token = getNextToken();
    if (current_token.type != 20){              // (
        return false;
    }
    if (parametre(name_of_node) == false){                  // parametre
            return false;
    }
    current_token = getNextToken();
    if (current_token.type != 21){              // )
            return false;
    }
    if (sipka_typ(name_of_node) == false){                  // ->typ
        return false;
    }

///////////////////////////////////////
    if (prvy_prechod)                       // end of first analysis
    {
        free(name_of_node);
        return true;
    }
    
///////////////////////////////////////
    current_token = getNextToken();
    if (current_token.type != 22){              // {
        return false;        
    }
    if (sekvencia() == false){                  // sekvencia
        return false;
    }
    current_token = getNextToken();
    if (returnovanie() == false){               // return
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){              // }
        return false;
    }
    free(name_of_node);
    DLL_DeleteFirst2(&symtable_stack);
    return true;
}

bool parametre(char *name_of_node){
    current_token = getNextToken();
    // token_print();
    if (current_token.type == 1 ||current_token.type == 15)
    {
        return parameter(name_of_node) && param_zostatok(name_of_node);
    }
    else if (current_token.type == 21)               // )
    {
        unget_token(current_token);
        return true;
    }
    return false;
}

bool parameter(char *name_of_node){
    // token uz je nacitany
    if (current_token.type == 1)                    // id
    {
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 1, current_token.attribute);
        }
        
        return zbytok_param(name_of_node);
    }
    else if (current_token.type == 15)              // _
    {
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 1, current_token.attribute);
        }
        
        current_token = getNextToken();
        if (current_token.type != 1)                // id
        {
            return false;
        }
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 2, current_token.attribute);
        }
        current_token = getNextToken();
        //token_print();
        if (current_token.type != 12)                // :
        {
            return false;
        }
        return typ_of_param(name_of_node);
    }
    return false;
}

bool param_zostatok(char *name_of_node){
    current_token = getNextToken();
    //token_print();
    if (current_token.type == 13)                   // ,
    {
        current_token = getNextToken();
        return parameter(name_of_node) && param_zostatok(name_of_node);
    }
    else if (current_token.type == 21)              // epsilon
    {
        unget_token(current_token);
        return true;
    }
    return false;
}

bool zbytok_param(char *name_of_node){
    current_token = getNextToken();
    if (current_token.type == 1 || current_token.type == 15)
    {
        // token_print();
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 2, current_token.attribute);
        }

        current_token = getNextToken();
        if (current_token.type == 12)           // :
        {
            return typ_of_param(name_of_node);
        }
        return false;
    }
    return false;
}

bool sipka_typ(char *name_of_node){
    current_token = getNextToken();
    if (current_token.type == 11)
    {
        return typ(name_of_node);
    }
    else if(current_token.type == 22){          // {
        unget_token(current_token);
        return true;
    }
    return false;
}
/**
 * toto volanie nastane pri urceni return typu pri deklaracii funckie alebo pri letnuti / varnuti
 * 
 * pri letnuti / varnuti iba urci typ, ale nikam nezapisuje
*/
bool typ(char *name_of_node){
    current_token = getNextToken();
    //token_print();
    if (current_token.type == 4 && strcmp(current_token.attribute, "Int") == 0){
        if (!prvy_prechod){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, 'I');
        }
        else{
            insert_return_typ(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0]);
        }
        return true;
    }

    if (current_token.type == 4 && strcmp(current_token.attribute, "String") == 0){
        if (!prvy_prechod){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, 'S');
        }
        else{
            insert_return_typ(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0]);
        }
        return true;
    }

    if (current_token.type == 4 && strcmp(current_token.attribute, "Double") == 0){
        if (!prvy_prechod){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, 'D');
        }
        else{
            insert_return_typ(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0]);
        }
        return true;
    }

    if (current_token.type == 4 && strcmp(current_token.attribute, "Int?") == 0){
        if (!prvy_prechod){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, 'T');
        }                         //odzadu int konci t, znamena ze moze mat aj NIL
        else{
            insert_return_typ(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0]);
        }
        return true;
    }

    if (current_token.type == 4 && strcmp(current_token.attribute, "String?") == 0){
        if (!prvy_prechod){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, 'G');
        }
        else{
            insert_return_typ(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0]);
        }
        return true;
    }

    if (current_token.type == 4 && strcmp(current_token.attribute, "Double?") == 0){
        if (!prvy_prechod){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, 'E');
        }
        else{
            insert_return_typ(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0]);
        }
        return true;
    }
    return false;
}
/**
 * toto volanie nastane iba z deklaracie funkcie od samotnych parametrov
 * 
 * pri prvom prechode analyzy bude aktualizovat nodu stromu
*/
bool typ_of_param(char *name_of_node){
    current_token = getNextToken();
    if (current_token.type == 4 && ( 
        strcmp(current_token.attribute, "Int") == 0 ||
        strcmp(current_token.attribute, "String") == 0 ||
        strcmp(current_token.attribute, "Double") == 0 ||
        strcmp(current_token.attribute, "Int?") == 0 ||
        strcmp(current_token.attribute, "String?") == 0 ||
        strcmp(current_token.attribute, "Double?") == 0))
    {
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 3, current_token.attribute);
        }
        return true;
    }
    return false;
}

bool dvojbodka_typ(char *name_of_node){
    current_token = getNextToken();
    if (strcmp(current_token.attribute, ":") == 0 && current_token.type == 12){     //ked nacita : tak urcime typ
        return typ(name_of_node);
    }
    dvojbodka_typ_neni = true;                       //epsilon pravidlo - ked nacita hocico ine okrem dvojbodky, poznacime ze typ bol vynechany
    unget_token(current_token);
    return true;
}

bool priradenie_prave(){
    char *name_of_node = '\0';
    //token_print();
    current_token = getNextToken();
    current_token2 = getNextToken();
    if (current_token.type == 1 && current_token2.type == 20){              // id (
        name_of_node = string_dup(current_token.attribute);
        //printf("%s", name_of_node);
        btree_node *temp = find_declaration(&symtable_stack, name_of_node);
        if (temp == NULL)
        {
            puts("funkcia neexistuje\n");
            handle_error(SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE);
        }
        int num_of_params = 0;
        if (parametre_volania(temp, &num_of_params) == false){
            return false;
        }
        if (num_of_params < temp->func_num_of_param){
            printf("Pocet volanych parametrov je mensi ako pri deklaracii\n");
            handle_error(SEMANTIC_PARAMETER_MISMATCH);
        }
        free(name_of_node);
        current_token = getNextToken();
        return (current_token.type == 21);                   // )
    }
    else if (current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7 || current_token.type == 8 || current_token.type == 20){
        //printf("expression will be reduced:\n");
        // token_print();
        //printf("somtu");
        //printf("%d", reduce_exp());
        //printf("%s", current_token.attribute);
        unget_token(current_token2);                       //toto asi treba dat pred reduce_exp
        //printf("%d", reduce_exp());
        //printf("anoo");
        if (reduce_exp() == false){                         //tu uz su nacitane rovno prve dva tokeny
           return false;
        }
        //token_print();
        return true;
    }
    return false;
}

bool rovna_sa__priradenie(){
    current_token = getNextToken();                                     // =
    if (current_token.type == 10 && strcmp(current_token.attribute, "=") == 0){
        return priradenie_prave();
    }
    else if(dvojbodka_typ_neni){              //ked bola vypustena deklaracia typu nemozeme vypustit priradenie
        return false;
    }
    else{
        unget_token(current_token);          //epsilon prechod vratime nacitany token  
        return true;
    }

}
bool letnutie(){
    current_token = getNextToken();
    char *name_of_node = string_dup(current_token.attribute);
    if (current_token.type == 1){
        insert_variable(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.type, true, '\0', true);
        if (dvojbodka_typ(name_of_node) == false){
            return false;
        }
        if (rovna_sa__priradenie() == false){
            return false;
        }
        free(name_of_node);
        return true;
    }
    else{
        return false;
    }
}

bool varnutie(){
    current_token = getNextToken();
    char *name_of_node = string_dup(current_token.attribute);
    if (current_token.type == 1){
        insert_variable(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.type, true, '\0', false);
        if (dvojbodka_typ(name_of_node) == false){
            return false;
        }
        if (rovna_sa__priradenie() == false){
            return false;
        }
        free(name_of_node);
        return true;
    }
    else{
        return false;
    }
}

bool relacia(){
    //printf("relacia -> precedencna\n");
    if (reduce_exp() == false){
        return false;
    }
    current_token = getNextToken();
    //token_print();
    if (current_token.type != 6)
    {
        return false;
    }
    current_token = getNextToken();
    if (reduce_exp() == false){
        return false;
    }
    return true;
}

bool podmienka(){
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4)
    {
        return letnutie();
    }
    else if(current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7 || current_token.type == 20)
    {
        return relacia();
    }
    return false;
}

bool whilnutie(){
    current_token = getNextToken();
    if (podmienka()== false){
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 22){              // {
        return false;        
    }
    current_token = getNextToken();

    if (sekvencia() == false){
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){              // }
        return false;
    }
    current_token = getNextToken();
    return true;
}

bool ifnutie(){
    current_token = getNextToken();
    if (podmienka()== false){
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 22){                                                  // {
        return false;        
    }
    //current_token = getNextToken();
    if (sekvencia() == false){                                                      // sekvencia
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){                                                  // }
        return false;
    }
    current_token = getNextToken();
    if (strcmp(current_token.attribute, "else") != 0 || current_token.type != 4){   // else
        return false;        
    }
    current_token = getNextToken();
    if (current_token.type != 22){                                                  // {
        return false;        
    }
    //current_token = getNextToken();
    if (sekvencia() == false){                                                      // sekvencia
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){                                                  // }
        return false;
    }
    return true;
}

bool param_vol_zost(btree_node *temp, int* num_of_params){
    current_token = getNextToken();
    if (current_token.type == 13){                  // ,
        current_token = getNextToken();             // id,string,int...(proste dalsi param)
        //(*num_of_params)++;
        return parameter_volania(temp, num_of_params) && param_vol_zost(temp, num_of_params);
    }
    else if (current_token.type == 21){
        unget_token(current_token);                 // epsilon prechod musime vratit nacitany token naspat
        return true;
    }
    return false;
}

bool parameter_volania(btree_node *temp, int* num_of_params){           // temp je ukazatel na uzol kde je nazov nasej funkcie
    (*num_of_params)++;                                                    //conuter parametrov
    //printf("Kolkaty parameter: %d\n", (*num_of_params));
    if (*num_of_params > temp->func_num_of_param){
        printf("Pocet parametrov vo volani je vacsi ako v deklaracii\n");
        handle_error(SEMANTIC_PARAMETER_MISMATCH);
    }
    current_token2 = getNextToken();    //: ak je dvojbodka je to volanie f(with: sth)

    if (current_token2.type == 12 && (strcmp(temp->paramsArray[(*num_of_params)-1].name, "_") != 0)){              // id : a v deklaracii nemoze byt _                  
        //printf("precedencna\n");
        
        if (strcmp(temp->paramsArray[(*num_of_params)-1].name,current_token.attribute) != 0){     //ked je identifikator ale nezhodouje sa  
            printf("Mena parametrov sa nezhoduju\n");
            handle_error(SEMANTIC_PARAMETER_MISMATCH);
        }
        
        current_token = getNextToken();                   // token do precedencnej
        if (reduce_exp() == false){
            return false;
        }
        return true;
    }
    else if (current_token2.type == 12 && strcmp(temp->paramsArray[*(num_of_params)-1].name, "_") == 0){          //ked v zavolani fce nie je identifikator_param a v deklaracii fce je
        printf("Meno parametru nema byt ale je\n");
        handle_error(SEMANTIC_PARAMETER_MISMATCH);
    } 
    else if (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8 || current_token.type == 20){    //ked nacita vyraz string,double,int,(...
        unget_token(current_token2);            //vratime token a zacneme precedencnu analyzu vyrazu
        //printf("precedencnaa\n");
        if (reduce_exp() == false){
            return false;
        }
        return true;
    }
    return false;
}
bool parametre_volania(btree_node *temp, int* num_of_params){
    current_token = getNextToken();
    //printf("%s\n", current_token.attribute);
    if (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8){    //ked nacita vyraz string,double,int,(...
        return parameter_volania(temp, num_of_params) && param_vol_zost(temp,num_of_params);
    }
    else if (current_token.type == 21){         // epsilon )
        // printf("somtu");
        
        // if (temp->func_num_of_param != 0)
        // {
        //     handle_error(SEMANTIC_PARAMETER_MISMATCH);
        // }

        unget_token(current_token);
        return true;
    }
    return false;
}

bool priradenie_zost(){
    char *name_of_node = string_dup(current_token.attribute);
    current_token = getNextToken();                             

    if (current_token.type == 20){                               // ( paramter
        btree_node *temp = find_declaration(&symtable_stack, name_of_node);
        free(name_of_node);
        if (temp == NULL)
        {
            puts("funkcia neexistuje");
            handle_error(SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE);
        }
        int num_of_params = 0;
        if (parametre_volania(temp, &num_of_params) == false)
        {
            return false;
        }
        if (num_of_params < temp->func_num_of_param){
            printf("Pocet volanych parametrov je mensi ako pri deklaracii\n");
            handle_error(SEMANTIC_PARAMETER_MISMATCH);
        }
        current_token = getNextToken();
        return (current_token.type == 21);                      // )
    
    }
    else if (current_token.type == 10){                         // =
        free(name_of_node);
        return priradenie_prave();
    }
    return false;
}

bool idnutie(){
    return priradenie_zost();
}

bool sekvencia(){
    current_token = getNextToken();
    // token_print();
    dvojbodka_typ_neni = false;
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4){
        if (letnutie() == false){
            return false;
        }
    }
    else if (strcmp(current_token.attribute, "var") == 0 && current_token.type == 4){
        if (varnutie() == false){
            return false;
        }
    }
    else if (strcmp(current_token.attribute, "while") == 0 && current_token.type == 4){
        if (whilnutie() == false){
            return false;
        }
    }
    else if (strcmp(current_token.attribute, "if") == 0 && current_token.type == 4){
        if (ifnutie()== false){
            return false;
        }
    }
    else if (strcmp(current_token.attribute, "func") == 0 && current_token.type == 4){
        if (func_declar(false) == false){
            return false;
        }
    }
    else if(current_token.type == 1 || current_token.type == 16){
        // if (find_declaration(&symtable_stack, current_token.attribute) == NULL){        //nenajdeme id false
        //         printf("Nenasli sme premennu pri idnuti\n");
        //         handle_error(SEMANTIC_UNDEFINED_VARIABLE);
        //     }
        if (idnutie() == false){
            return false;
        }
    }
    //else if(current_token.type == 2 || current_token.type == 3){
        //return reduce_exp();
    //}
    else if (current_token.type == 23 || (current_token.type == 4 && strcmp(current_token.attribute, "return") == 0 )){                 //epsion prechod
        unget_token(current_token);
        if (current_token.type == 4)                        //precitali sme return
        {
            return_neni = false;
        }
        return true;
    }
    else if(current_token.type == 0){
        return true;
    }
    else{
        return false;
    }
    return sekvencia();
    //TODO dalsie mozne neterminaly zo sekvencie
}

void parser(){
    if (sekvencia() == true){
        puts("OK");
        return;
    }
    else{
        handle_error(SYNTAX_ERROR);
    }
    // while (1){
    //     current_token = getNextToken();
    //     //printf("%d\n", current_token.type);
    //     //printf("%s\n", current_token.attribute);
    //     if (current_token.type == 0 && strcmp(current_token.attribute, "END") == 0){
    //         printf("ok\n\n");
    //         break;
    //     }
    //     else if (sekvencia() == false){
    //         printf("Syntax Error\n\n");
    //         break;
    //     }
    // }
    // if (sekvencia() == true){
    //     printf("ok");
    // }
    // else{
    //     printf("Syntax Error");
    // }


    // current_token = getNextToken();
    // while (current_token.type != 0)
    // {
    //     printf("Type: %d     ", current_token.type);
    //     printf("Attribute: %s         ", current_token.attribute);
    //     printf("prvy: %d\n", (int)current_token.first_in_line);
    //     current_token = getNextToken();
    // }
}
    