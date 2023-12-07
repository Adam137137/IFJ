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
int frame_counter = 0;
int func_counter = 1;
int if_counter = 0;
int while_counter = 0;
int built_in_counter = 2;
int relation_counter = 0;

int main_jump_counter = 1;
char *name_of_function = NULL;
int anti_zanorenie = 0;
bool ladenie = 1;


bool built_in_write_one_param_print(){
    current_token = getNextToken();
    if (current_token.type == 1)
    {
        char c;
        btree_node *token_found = find_declaration(&symtable_stack, current_token.attribute,&c);
        if (token_found == NULL){
            handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
        }
        if (c == 'F'){
            bool found = false;
            for(int i =0; i < token_found->func_num_of_param ;i++){			// searching through function params, trying to find id
                if(strcmp(token_found->paramsArray[i].identif, current_token.attribute) == 0){
                    found = true;
                    char *push_var = unique_name(token_found->paramsArray[i].identif, frame_counter);
                    sprintf(buffer1.data, "%sWRITE LF@%s\n",buffer1.data, push_var);
                    break;
                }
            }
            if (!found){
                handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
            }
        }
        else{                                           //case V means variable
            if (token_found->inicialized == false)
            {
                handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
            }
            
            
            if (frame_counter - anti_zanorenie == 0){                // case for global scope
                char *push_var = unique_name(current_token.attribute, 0);
                sprintf(buffer1.data, "%sWRITE GF@%s\n", buffer1.data, push_var);
            }
            else if(frame_counter-anti_zanorenie > 0){              // case for local scope
                char *push_var = unique_name(current_token.attribute, frame_counter - anti_zanorenie);
                sprintf(buffer1.data, "%sWRITE LF@%s\n",buffer1.data, push_var);
            }
        }
        
    }
    else if (current_token.type == 2)
    {
        sprintf(buffer1.data, "%sWRITE int@%s\n", buffer1.data, current_token.attribute);
    }
    else if (current_token.type == 3)
    {
        sprintf(buffer1.data, "%sWRITE float@%s\n", buffer1.data, current_token.attribute);
    }
    else if (current_token.type == 7)
    {
        sprintf(buffer1.data, "%sWRITE string@%s\n", buffer1.data, current_token.attribute);
    }
    else{
        return false;
    }
    return true;
}

bool built_in_write(){
    current_token = getNextToken();
    if(current_token.type != 20){
        return false;
    }
    
    if (built_in_write_one_param_print() == false){
        return false;
    }

    current_token = getNextToken();
    if (current_token.type == 21){               // single param function
        return true;
    }
    
    do {    
        if (built_in_write_one_param_print() == false){
            return false;
        }
        current_token = getNextToken();
    } while (current_token.type == 13);         // while param is folowed by ',' read next param

    if(current_token.type != 21){
        return false;
    }
    return true;
}


void token_print(){             // debugger, bool ladenie has to be 1
    if (ladenie)
    {
        printf("%s\n", current_token.attribute);
    }
}

void unget_token(struct Token token, bool new_line){
    if (token.type == 0){
        ungetc(EOF, file);
    }
    else{
        for (int i = strlen(token.attribute) - 1; i >= 0; i--) {
            ungetc((unsigned char)token.attribute[i], file);
        }
        if (new_line){
            ungetc('\n', file);
        }
    }
}
bool returnovanie(char *name_of_node, bool *contains_return){
    if (current_token.type == 4 && strcmp(current_token.attribute,"return") == 0){
        char return_t;
        current_token = getNextToken();             // first token in precedence analysis
        if (reduce_exp(&return_t, name_of_node, false) == false){
            return false;
        }
        btree_node *temp = find_function_in_global(&symtable_stack, name_of_node);
        if (temp->return_type != return_t){
            printf("Zla navratova hodnota z funckie\n");
            handle_error(SEMANTIC_RETURN_STATEMENT);
        }
        *contains_return = true;
        return true;
    }
    else if (current_token.type == 23){             //epsilon transition
        unget_token(current_token, current_token.first_in_line);
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
        name_of_function = string_dup(current_token.attribute);     // saving name of function to global variable
        char *func_name = unique_name(current_token.attribute, func_counter);
        sprintf(buffer1.data, "%sLABEL %s\n", buffer1.data, func_name);
        sprintf(buffer1.data, "%sPUSHFRAME\n", buffer1.data);
        
        func_counter++;
        DLL_InsertFirst2(&symtable_stack);
        frame_counter++;
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
    if (sekvencia(true, false, false) == false){                  // sequence
        return false;
    }
    current_token = getNextToken();
    bool dead_code = false;
    if (returnovanie(name_of_node, &dead_code) == false){               // return
        return false;
    }
    current_token = getNextToken();
    if (dead_code){
        while (current_token.type != 23)
        {
            current_token = getNextToken();
        }
    }
    
    if (current_token.type != 23){              // }
        return false;
    }
    sprintf(buffer1.data, "%sPOPFRAME\n", buffer1.data);
    sprintf(buffer1.data, "%sRETURN\n", buffer1.data);
    free(name_of_node);
    free(name_of_function);
    DLL_DeleteFirst2(&symtable_stack);
    name_of_function = NULL;
    frame_counter--;
    func_counter--;
    return_neni = false;
    return true;
}

bool parametre(char *name_of_node){
    current_token = getNextToken();
    if (current_token.type == 1 ||current_token.type == 15)
    {
        return parameter(name_of_node) && param_zostatok(name_of_node);
    }
    else if (current_token.type == 21)               // )
    {
        unget_token(current_token, current_token.first_in_line);
        return true;
    }
    return false;
}

bool parameter(char *name_of_node){
    // token already loaded
    if (current_token.type == 1)                    // id
    {
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 1, current_token.attribute);
        }
        
        return zbytok_param(name_of_node);
    }
    else if (current_token.type == 15)              // '_'
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
        if (!prvy_prechod){
            char *push_params = unique_name(current_token.attribute, frame_counter);
            sprintf(buffer1.data, "%sDEFVAR LF@%s\n", buffer1.data, push_params);
            sprintf(buffer1.data, "%sMOVE LF@%s LF@&%s\n", buffer1.data, push_params, push_params);
        }
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 2, current_token.attribute);
        }
        current_token = getNextToken();
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
    if (current_token.type == 13)                   // ,
    {
        current_token = getNextToken();
        return parameter(name_of_node) && param_zostatok(name_of_node);
    }
    else if (current_token.type == 21)              // epsilon
    {
        unget_token(current_token, current_token.first_in_line);
        return true;
    }
    return false;
}

bool zbytok_param(char *name_of_node){
    current_token = getNextToken();
    if (current_token.type == 1 || current_token.type == 15)
    {
        if (prvy_prechod)
        {
            insert_params(&symtable_stack.firstElement->treeRoot, name_of_node, 2, current_token.attribute);
        }
        if (!prvy_prechod){
            char *push_params = unique_name(current_token.attribute, frame_counter);
            sprintf(buffer1.data, "%sDEFVAR LF@%s\n", buffer1.data, push_params);
            sprintf(buffer1.data, "%sMOVE LF@%s LF@&%s\n", buffer1.data, push_params, push_params);
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
        unget_token(current_token, current_token.first_in_line);
        return true;
    }
    return false;
}
/**
 * this call of function occurs, when we 
 * toto volanie nastane pri urceni return typu pri deklaracii funckie alebo pri letnuti / varnuti
 * 
 * in let and var, defining type
 * pri letnuti / varnuti iba urci typ, ale nikam nezapisuje
*/
bool typ(char *name_of_node){
    current_token = getNextToken();
    if (current_token.type == 4 && (
        strcmp(current_token.attribute, "Int") == 0 ||
        strcmp(current_token.attribute, "String") == 0 ||
        strcmp(current_token.attribute, "Double") == 0 ||
        strcmp(current_token.attribute, "Int?") == 0 ||
        strcmp(current_token.attribute, "String?") == 0 ||
        strcmp(current_token.attribute, "Double?") == 0))
    {
        if ((current_token.type == 4 && (strcmp(current_token.attribute, "Int") == 0 || strcmp(current_token.attribute, "String") == 0 || strcmp(current_token.attribute, "Double") == 0))  && !(prvy_prechod)){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0], false);
        }
        else if ((current_token.type == 4 && (strcmp(current_token.attribute, "Int?") == 0 || strcmp(current_token.attribute, "String?") == 0 || strcmp(current_token.attribute, "Double?") == 0))  && !(prvy_prechod)){
            insert_data_type(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0], true);
        }
        
        insert_return_typ(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.attribute[0]);
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
    dvojbodka_typ_neni = true;                       //epsilon rule - when reads anything else than ':', means that type was not assigned
    unget_token(current_token, current_token.first_in_line);
    return true;
}

bool priradenie_prave(char *name_of_node){
    current_token = getNextToken();
    current_token2 = getNextToken();

    if(current_token.type == 16 && current_token2.type == 20){           // check for built in funcs
        built_in_counter++;
        if(strcmp(current_token.attribute, "Int2Double") == 0){
            current_token = getNextToken();
            
            if(!(current_token.type == 1 || current_token.type == 2)){      // variable or int
                printf("u built in zly parameter\n");
                handle_error(SEMANTIC_PARAMETER_MISMATCH);
            }
            char return_t;
            if(reduce_exp(&return_t, name_of_node, false) == false){        // reduce expr inside func call
                return false;
            }
            
            char c;
            btree_node *assign_to_variable = find_declaration(&symtable_stack, name_of_node, &c);
            
            if(assign_to_variable->data_type != 'D'){
                handle_error(SEMANTIC_TYPE_COMPATIBILITY);
            }

            current_token = getNextToken();
            if(current_token.type != 21){      // )
                return false;
            }
            
            sprintf(buffer1.data, "%sINT2FLOATS\n", buffer1.data);
            
            if(frame_counter-anti_zanorenie == 0){                                     // assignment after checks
                char *variable_name = unique_name(name_of_node, 0);
                sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
            }
            else if(frame_counter-anti_zanorenie > 0){
                char *variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
            }
            assign_to_variable->inicialized = true;
            return true;

        }
        else if(strcmp(current_token.attribute, "Double2Int") == 0){
            current_token = getNextToken();
            
            if(!(current_token.type == 1 || current_token.type == 3)){      // variable or double
                printf("u built in zly parameter\n");
                handle_error(SEMANTIC_PARAMETER_MISMATCH);
            }
            char return_t;
            if(reduce_exp(&return_t, name_of_node, false) == false){        // reduce expr inside func call
                return false;
            }
            
            char c;
            btree_node *assign_to_variable = find_declaration(&symtable_stack, name_of_node, &c);
            
            if(assign_to_variable->data_type != 'I'){
                handle_error(SEMANTIC_TYPE_COMPATIBILITY);
            }

            current_token = getNextToken();
            if(current_token.type != 21){      // )
                return false;
            }
            
            sprintf(buffer1.data, "%sFLOAT2INTS\n", buffer1.data);
            
            if(frame_counter-anti_zanorenie == 0){                                     // assignment after checks
                char *variable_name = unique_name(name_of_node, 0);
                sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
            }
            else if(frame_counter-anti_zanorenie > 0){
                char *variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
            }
            assign_to_variable->inicialized = true;
            return true;
        
        }
        else if(strcmp(current_token.attribute, "length") == 0){
            current_token = getNextToken();
            
            if(!(current_token.type == 1 || current_token.type == 7 || current_token.type == 8)){      // variable or string
                printf("u built in zly parameter\n");
                handle_error(SEMANTIC_PARAMETER_MISMATCH);
            }
            char return_t;
            if(reduce_exp(&return_t, name_of_node, false) == false){        // reduce expr inside func call
                return false;
            }

            char c;
            btree_node *assign_to_variable = find_declaration(&symtable_stack, name_of_node, &c);
            
            if(assign_to_variable->data_type != 'I'){
                handle_error(SEMANTIC_TYPE_COMPATIBILITY);
            }

            current_token = getNextToken();
            if(current_token.type != 21){      // )
                return false;
            }
            
            char *variable_name;
            if(frame_counter-anti_zanorenie == 0){                                  // assignment after checks
                variable_name = unique_name(name_of_node, 0);
                sprintf(buffer1.data, "%sDEFVAR GF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sPOPS GF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sSTRLEN GF@%s GF@TEMP%d\n", buffer1.data, variable_name, built_in_counter + func_counter);    
            }
            else if(frame_counter-anti_zanorenie > 0){                             // assignment after checks
                variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                printf(buffer1.data, "%sDEFVAR LF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sPOPS LF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sSTRLEN LF@%s LF@TEMP%d\n", buffer1.data, variable_name, built_in_counter + func_counter);
            }
            assign_to_variable->inicialized = true;
            return true;
        
        }
        else if(strcmp(current_token.attribute, "substring") == 0){
            printf("not implemented\n");
        }
        else if(strcmp(current_token.attribute, "ord") == 0){
            current_token = getNextToken();
            
            if(!(current_token.type == 1 || current_token.type == 7 || current_token.type == 8)){      // variable or string
                printf("u built in zly parameter\n");
                handle_error(SEMANTIC_PARAMETER_MISMATCH);
            }
            char return_t;
            if(reduce_exp(&return_t, name_of_node, false) == false){        // reduce expr inside func call
                return false;
            }
            
            char c;
            btree_node *assign_to_variable = find_declaration(&symtable_stack, name_of_node, &c);
            
            if(assign_to_variable->data_type != 'I'){
                handle_error(SEMANTIC_TYPE_COMPATIBILITY);
            }

            current_token = getNextToken();
            if(current_token.type != 21){      // )
                return false;
            }
            char *variable_name;
            if(frame_counter-anti_zanorenie == 0){                                  // assignment after checks
                variable_name = unique_name(name_of_node, 0);
                sprintf(buffer1.data, "%sDEFVAR GF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sPOPS GF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sSTRI2INT GF@%s GF@TEMP%d int@0\n", buffer1.data, variable_name, built_in_counter + func_counter);
            }
            else if(frame_counter-anti_zanorenie > 0){                             // assignment after checks
                variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                printf(buffer1.data, "%sDEFVAR LF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sPOPS LF@TEMP%d\n", buffer1.data, built_in_counter + func_counter);
                sprintf(buffer1.data, "%sSTRI2INT LF@%s LF@TEMP%d int@0\n", buffer1.data, variable_name, built_in_counter + func_counter);
            }
            assign_to_variable->inicialized = true;
            return true;

        }
        else if(strcmp(current_token.attribute, "chr") == 0){
            current_token = getNextToken();
            char *param = string_dup(current_token.attribute);
            
            if(!(current_token.type == 1 || current_token.type == 2)){      // variable or int
                printf("u built in zly parameter\n");
                handle_error(SEMANTIC_PARAMETER_MISMATCH);
            }
            char return_t;
            if(reduce_exp(&return_t, name_of_node, false) == false){        // reduce expr inside func call
                return false;
            }
            
            char c;
            btree_node *assign_to_variable = find_declaration(&symtable_stack, name_of_node, &c);
            
            if(assign_to_variable->data_type != 'S'){
                handle_error(SEMANTIC_TYPE_COMPATIBILITY);
            }

            char *variable_name;
            if(frame_counter-anti_zanorenie == 0){                                  // assignment after checks
                variable_name = unique_name(name_of_node, 0);
                sprintf(buffer1.data, "%sINT2CHAR GF@%s int@%s\n", buffer1.data, variable_name, param);
            }
            else if(frame_counter-anti_zanorenie > 0){                             // assignment after checks
                variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                sprintf(buffer1.data, "%sINT2CHAR LF@%s int@%s\n", buffer1.data, variable_name, param);
                
            }
            
            current_token = getNextToken();
            if(current_token.type != 21){       // )
                return false;
            }
            assign_to_variable->inicialized = true;
            return true;

        }
    }

    if (current_token.type == 1 && current_token2.type == 20){              // id (
        char *name_of_func = string_dup(current_token.attribute);
        btree_node *temp = find_function_in_global(&symtable_stack, name_of_func);
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
        char *func_call = unique_name(name_of_func, func_counter);
        sprintf(buffer1.data, "%sCALL %s\n", buffer1.data, func_call);

        char c;
        btree_node *node = find_declaration(&symtable_stack, name_of_node, &c);             // finding variable on left side of expression
        if(frame_counter-anti_zanorenie == 0 && c == 'V'){
            char *variable_name = unique_name(name_of_node, 0);
            sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
        }
        else if(frame_counter-anti_zanorenie > 0){
            char *variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
            sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
        }
        else if(c == 'F'){
            char *push_var = unique_name(name_of_node,  1);
            sprintf(buffer1.data, "%sPUSHS LF@%s\n",buffer1.data, push_var);
        }
        
        if (node->data_type == '\0'){
            node->data_type = temp->return_type;            // type is deduced from function return
        }
        if (temp->return_type != node->data_type){      //comparison of returned data types
            // function has wrong returning value, which has to be assigned to variable
            handle_error(SEMANTIC_TYPE_COMPATIBILITY);
        }
        node->inicialized = true;
        free(name_of_func);
        current_token = getNextToken();
        return (current_token.type == 21);                   // )
    }
    

    else if (current_token.type == 1 && current_token2.type == 9){
        unget_token(current_token2, current_token2.first_in_line);

        btree_node *token_found = NULL;
        char var;
        char *name_of_node2 = '\0';
        name_of_node2 = string_dup(current_token.attribute);
        token_found = find_declaration(&symtable_stack, name_of_node2,&var);

        current_token = getNextToken();         // ??
        current_token = getNextToken();
        
        if (!(current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8)){
            return false;
        }

        char *typ_na_pushnutie = NULL;
        switch (current_token.type){
        case 2:
            typ_na_pushnutie = "int";
            break;
        case 3:
            typ_na_pushnutie = "float";
            break;
        default:
            typ_na_pushnutie = "string";
            break;
        }


        if (token_found->nilnuta){
            char *variable_name = unique_name(name_of_node, 0);
            if (frame_counter - anti_zanorenie == 0){
                sprintf(buffer1.data, "%sPUSHS %s@%s\n", buffer1.data, typ_na_pushnutie, current_token.attribute);
                sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
            }
            else if(frame_counter-anti_zanorenie > 0){
                sprintf(buffer1.data, "%sPUSHS %s@%s\n", buffer1.data, typ_na_pushnutie, current_token.attribute);
                sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
            }
        }
        else{
            char *variable_name = unique_name(name_of_node, 0);
            if (frame_counter - anti_zanorenie == 0){
                char *push_var = unique_name(token_found->name_of_symbol, 0);
                sprintf(buffer1.data, "%sPUSHS GF@%s\n", buffer1.data, push_var);
                sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
            }
            else if(frame_counter-anti_zanorenie > 0){
                char *push_var = unique_name(token_found->name_of_symbol, frame_counter - anti_zanorenie);
                sprintf(buffer1.data, "%sPUSHS LF@%s\n",buffer1.data, push_var);
                sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
            }       
        }
        return true;
    }
    

    else if (current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || (current_token.type == 4 && strcmp(current_token.attribute, "nil")==0)|| current_token.type == 7 || current_token.type == 8 || current_token.type == 20){
        unget_token(current_token2, current_token2.first_in_line);
        
        char c;                                                                         // it is F for function or V for variable
        btree_node *tmp = find_declaration(&symtable_stack, name_of_node,&c);
        
        if (current_token.type == 4){                      
            if (tmp->data_type == '\0'){                    //  // assigning nil to unknown return type
                handle_error(SEMANTIC_TYPE_INFERENCE);
            }
            
            if (tmp->nil == false){
                handle_error(SEMANTIC_TYPE_COMPATIBILITY);
            }

            if(frame_counter-anti_zanorenie == 0){
                char *variable_name = unique_name(name_of_node, 0);
                sprintf(buffer1.data, "%sPUSHS nil@nil\n", buffer1.data);
                sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
            }
            else if(frame_counter-anti_zanorenie > 0){
                char *variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                sprintf(buffer1.data, "%sPUSHS nil@nil\n", buffer1.data);
                sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
            }
            tmp->nilnuta = true;
            return true;
        }
        
        char return_t;
        tmp->inicialized = true;    
        if (reduce_exp(&return_t, name_of_node, false) == false){
            return false;
        }
        if (c == 'F'){
            // printf("Do parametru sa neda priradit hodnota\n");
            handle_error(OTHER_SEMANTIC_ERROR);
        }
        else if(c == 'V'){
            if (tmp->data_type != return_t && tmp->data_type != '\0'){                 // return type may not be defined
                if (tmp->data_type == 'D' && return_t == 'I'){                              // assigning to declaration of double, int
                    tmp->data_type = 'I';
                    if(frame_counter-anti_zanorenie == 0){
                        char *variable_name = unique_name(name_of_node, 0);
                        sprintf(buffer1.data, "%sINT2FLOATS\n", buffer1.data);
                        sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
                    }
                    else if(frame_counter-anti_zanorenie > 0){
                        char *variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                        sprintf(buffer1.data, "%sINT2FLOATS\n", buffer1.data);
                        sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
                    }
                    return true;
                }
                else{                                               // case where type not defined, nothing to be changed
                    handle_error(SEMANTIC_TYPE_COMPATIBILITY);
                }
            }
            else{
                tmp->data_type = return_t;                       //type not defined, will be defined from result of reducing expression
                if(frame_counter-anti_zanorenie == 0){
                    char *variable_name = unique_name(name_of_node, 0);
                    sprintf(buffer1.data, "%sPOPS GF@%s\n", buffer1.data, variable_name);
                }
                else if(frame_counter-anti_zanorenie > 0){
                    char *variable_name = unique_name(name_of_node, frame_counter-anti_zanorenie);
                    sprintf(buffer1.data, "%sPOPS LF@%s\n", buffer1.data, variable_name);
                }
            }
        }             
        return true;        
    }

    return false;
}

bool rovna_sa__priradenie(char* name_of_node){
    current_token = getNextToken();                                     // =
    
    if (current_token.type == 10){
        return priradenie_prave(name_of_node);
    }
    else if(dvojbodka_typ_neni){              // was not declared => can not skip assignment
        return false;
    }
    else{
        unget_token(current_token, current_token.first_in_line);            //epsilon transition returning token   
        return true;
    }

}
bool letnutie(bool from_if){            
    current_token = getNextToken();
    char *name_of_node = string_dup(current_token.attribute);
    if (current_token.type == 1){
        if (from_if){                                                       // let rule type:  ': if let id{}'    
            btree_node *temp = find_function_in_global(&symtable_stack, name_of_node);
            if (!temp->nil){
                free(name_of_node);
                handle_error(OTHER_SEMANTIC_ERROR);
            }
            else{
                if (temp->inicialized == false){
                    sprintf(buffer1.data, "%sPUSHS bool@false\n", buffer1.data);
                }
                else{
                    sprintf(buffer1.data, "%sPUSHS bool@true\n", buffer1.data);
                }
            }
            
            free(name_of_node);
            return true;
        }


        insert_variable(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.type, true);
        
        char *variable_name = unique_name(current_token.attribute, frame_counter);
        if(frame_counter == 0){
            sprintf(buffer1.data, "%sDEFVAR GF@%s\n", buffer1.data, variable_name);
        }
        else{
            sprintf(buffer1.data, "%sDEFVAR LF@%s\n", buffer1.data, variable_name);
        }

        if (dvojbodka_typ(name_of_node) == false){
            free(name_of_node);
            return false;
        }
        
        if (rovna_sa__priradenie(name_of_node) == false){
            free(name_of_node);
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
        insert_variable(&symtable_stack.firstElement->treeRoot, name_of_node, current_token.type, false);
        
        char *variable_name = unique_name(current_token.attribute, frame_counter);
        if(frame_counter == 0){
            sprintf(buffer1.data, "%sDEFVAR GF@%s\n", buffer1.data, variable_name);
        }
        else{
            sprintf(buffer1.data, "%sDEFVAR LF@%s\n", buffer1.data, variable_name);
        }

        if (dvojbodka_typ(name_of_node) == false){
            return false;
        }
        if (rovna_sa__priradenie(name_of_node) == false){
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
    char return_t, return_t_2;
    bool extra_par = false;

    if (reduce_exp(&return_t,NULL, &extra_par) == false){
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 6){
        handle_error(SEMANTIC_TYPE_COMPATIBILITY);
    }
    char* operator = current_token.attribute;
    current_token = getNextToken();
    if (reduce_exp(&return_t_2,NULL, &extra_par) == false){
        return false;
    }
    if (return_t != return_t_2){                                            // comparison of relation operators
        if ( !((return_t == 'I' && return_t_2 == 'D')|| (return_t == 'D' && return_t_2 == 'I'))){
            handle_error(SEMANTIC_TYPE_COMPATIBILITY);
        }   
    }

    if (strcmp(operator, "<") == 0){
        sprintf(buffer1.data, "%sLTS\n", buffer1.data);
    }
    else if (strcmp(operator, ">") == 0)    {
        sprintf(buffer1.data, "%sGTS\n", buffer1.data);

    }
    else if(strcmp(operator, "==") == 0){
        sprintf(buffer1.data, "%sEQS\n", buffer1.data);
    }
    else if(strcmp(operator, "!=") == 0){
        sprintf(buffer1.data, "%sEQS\n", buffer1.data);
        sprintf(buffer1.data, "%sNOTS\n", buffer1.data);
    }
    else if(strcmp(operator, ">=") == 0){
        relation_counter++;
        if(frame_counter-anti_zanorenie == 0){                                  // assignment after checks
            sprintf(buffer1.data, "%sDEFVAR GF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sDEFVAR GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS GF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sPUSHS GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS GF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sGTS\n", buffer1.data);
            
            sprintf(buffer1.data, "%sPUSHS GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS GF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sEQS\n", buffer1.data);
            sprintf(buffer1.data, "%sORS\n", buffer1.data);
        }
        else{                             // assignment after checks
            sprintf(buffer1.data, "%sDEFVAR LF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sDEFVAR LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS LF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sPUSHS LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS LF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sGTS\n", buffer1.data);
            
            sprintf(buffer1.data, "%sPUSHS LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS LF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sEQS\n", buffer1.data);
            sprintf(buffer1.data, "%sORS\n", buffer1.data);
        }
    }
    else if(strcmp(operator, "<=") == 0){
        relation_counter++;
        if(frame_counter-anti_zanorenie == 0){                                  // assignment after checks
            sprintf(buffer1.data, "%sDEFVAR GF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sDEFVAR GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS GF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sPUSHS GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS GF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sGTS\n", buffer1.data);
            
            sprintf(buffer1.data, "%sPUSHS GF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS GF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sEQS\n", buffer1.data);
            sprintf(buffer1.data, "%sORS\n", buffer1.data);
        }
        else{                             // assignment after checks
            sprintf(buffer1.data, "%sDEFVAR LF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sDEFVAR LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPOPS LF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sPUSHS LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS LF@TMP1$%d\n", buffer1.data, relation_counter);

            sprintf(buffer1.data, "%sLTS\n", buffer1.data);
            
            sprintf(buffer1.data, "%sPUSHS LF@TMP2$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sPUSHS LF@TMP1$%d\n", buffer1.data, relation_counter);
            sprintf(buffer1.data, "%sEQS\n", buffer1.data);
            sprintf(buffer1.data, "%sORS\n", buffer1.data);
        }
    }

    if (extra_par){                  // deletion of extra parenthesis, when condition was in parentheses
        current_token =getNextToken();
    }
    return true;
}

bool podmienka(){
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4)
    {
        return letnutie(true);
    }
    else if(current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7 || current_token.type == 20)
    {
        return relacia();
    }
    return false;
}

bool whilnutie(bool in_func, bool in_while, bool in_if){
    current_token = getNextToken();

    while_counter++;
    char *while_start = unique_name("while_start", while_counter);
    sprintf(buffer1.data, "%sLABEL %s\n", buffer1.data, while_start);

    if (podmienka()== false){
        return false;
    }
    
    char *while_end = unique_name("while_end", while_counter);
    sprintf(buffer1.data, "%sPUSHS bool@true\n", buffer1.data);
    sprintf(buffer1.data, "%sJUMPIFNEQS %s\n", buffer1.data, while_end);
    if(!(in_func || in_while ||in_if)){
        sprintf(buffer1.data, "%sCREATEFRAME\n", buffer1.data);
        sprintf(buffer1.data, "%sPUSHFRAME\n", buffer1.data);
    }
    current_token = getNextToken();

    if (current_token.type != 22){              // {
        return false;
    }

    if (sekvencia(false, true, false) == false){
        return false;
    }

    sprintf(buffer1.data, "%sJUMP %s\n", buffer1.data, while_start);
    current_token = getNextToken();
    if (current_token.type != 23){              // }
        return false;
    }
    sprintf(buffer1.data, "%sLABEL %s\n", buffer1.data, while_end);
    return true;
}

bool ifnutie(bool in_func, bool in_while, bool in_if){
    current_token = getNextToken();

    if (podmienka()== false){
        return false;
    }
    
    if_counter++;
    char *true_end = unique_name("true_end", if_counter);
    char *else_end = unique_name("else_end", if_counter);
    
    sprintf(buffer1.data, "%sPUSHS bool@true\n", buffer1.data);
    sprintf(buffer1.data, "%sJUMPIFNEQS %s\n", buffer1.data, true_end);
    if(!(in_func || in_while ||in_if)){
        sprintf(buffer1.data, "%sCREATEFRAME\n", buffer1.data);
        sprintf(buffer1.data, "%sPUSHFRAME\n", buffer1.data);
    }
    
    current_token = getNextToken();

    if (current_token.type != 22){                                                  // {
        return false;        
    }
    if (sekvencia(false, false, true) == false){                                    // sequence
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){                                                  // }
        return false;
    }
    sprintf(buffer1.data, "%sJUMP %s\n", buffer1.data, else_end);
    current_token = getNextToken();
    if (strcmp(current_token.attribute, "else") != 0 || current_token.type != 4){   // else
        return false;        
    }
    current_token = getNextToken();
    if (current_token.type != 22){                                                  // {
        return false;        
    }
    sprintf(buffer1.data, "%sLABEL %s\n", buffer1.data, true_end);
    
    if (sekvencia(false, false, true) == false){                                    // sequence
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){                                                  // }
        return false;
    }
    sprintf(buffer1.data, "%sLABEL %s\n", buffer1.data, else_end);
    return true;
}

bool param_vol_zost(btree_node *temp, int* num_of_params){
    current_token = getNextToken();
    if (current_token.type == 13){                  // ,
        current_token = getNextToken();             // id,string,int => next param
        return parameter_volania(temp, num_of_params) && param_vol_zost(temp, num_of_params);
    }
    else if (current_token.type == 21){
        unget_token(current_token, current_token.first_in_line);                 // epsilon transition returning token back
        return true;
    }
    return false;
}

bool parameter_volania(btree_node *temp, int* num_of_params){           // temp is pointer to node, where is name of function
    (*num_of_params)++;
    if (*num_of_params > temp->func_num_of_param){                  // More params in calling than in declaration      
        handle_error(SEMANTIC_PARAMETER_MISMATCH);
    }
    current_token2 = getNextToken();    //if ':' type of calling => f(with: sth)
    if (current_token2.type == 12){
        if((strcmp(temp->paramsArray[(*num_of_params)-1].name, current_token.attribute) != 0) || (strcmp(temp->paramsArray[(*num_of_params)-1].name, "_") == 0)){
            handle_error(SEMANTIC_PARAMETER_MISMATCH);      // has name of param, but it should not have
        }
        current_token = getNextToken();                   // token goes to precedence analysis
        if (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8 || current_token.type == 20){
            
            char *push_params = unique_name(temp->paramsArray[(*num_of_params)-1].identif, func_counter);
            sprintf(buffer1.data, "%sDEFVAR TF@&%s\n", buffer1.data, push_params);
            char return_t;
            if (reduce_exp(&return_t, temp->name_of_symbol, false) == false){
                return false;
            }
            sprintf(buffer1.data, "%sPOPS TF@&%s\n", buffer1.data, push_params);
            
            if (temp->paramsArray[(*num_of_params)-1].type != return_t){  //function has different returning types
                handle_error(SEMANTIC_PARAMETER_MISMATCH);
            }
            return true;
        }
    }
    else if (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8 || current_token.type == 20){    //ked nacita vyraz string,double,int,(...
        unget_token(current_token2, current_token2.first_in_line);            // putting token back start of precedence analysis
        if ((strcmp(temp->paramsArray[(*num_of_params)-1].name, "_") != 0)){    // during call of func, we have more params than expected
            handle_error(SEMANTIC_PARAMETER_MISMATCH);
        }
        char *push_params = unique_name(temp->paramsArray[(*num_of_params)-1].identif, func_counter);
        sprintf(buffer1.data, "%sDEFVAR TF@&%s\n", buffer1.data, push_params);
        char return_t;
        if (reduce_exp(&return_t, temp->name_of_symbol, false) == false){
            return false;
        }
        sprintf(buffer1.data, "%sPOPS TF@&%s\n", buffer1.data, push_params);

        if (temp->paramsArray[(*num_of_params)-1].type != return_t){        // function has different types of params
            handle_error(SEMANTIC_PARAMETER_MISMATCH);
        }
        return true;
    }
    return false;
}
bool parametre_volania(btree_node *temp, int* num_of_params){
    current_token = getNextToken();
    sprintf(buffer1.data, "%sCREATEFRAME\n", buffer1.data);
    if (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8){
        return parameter_volania(temp, num_of_params) && param_vol_zost(temp,num_of_params);
    }
    else if (current_token.type == 21){         // epsilon )
        unget_token(current_token, current_token.first_in_line);
        return true;
    }
    return false;
}

bool priradenie_zost(){
    char *name_of_node = string_dup(current_token.attribute);
    current_token = getNextToken();
    if (current_token.type == 20){                               // ( paramter
        btree_node *temp = find_function_in_global(&symtable_stack, name_of_node);
        if (temp == NULL)
        {
            handle_error(SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE);
        }
        int num_of_params = 0;
        if (parametre_volania(temp, &num_of_params) == false)
        {
            return false;
        }
        
        if (num_of_params < temp->func_num_of_param){           // lower number of params in call, than in declaration
            handle_error(SEMANTIC_PARAMETER_MISMATCH);
        }
        char *func_call = unique_name(name_of_node, func_counter);
        sprintf(buffer1.data, "%sCALL %s\n", buffer1.data, func_call);

        current_token = getNextToken();
        return (current_token.type == 21);                      // )
    
    }
    else if (current_token.type == 10){                         // =
        char c;
        btree_node *temp = find_declaration(&symtable_stack, name_of_node,&c);
        if (temp == NULL){
            printf("ID nebolo deklarovane nikde\n");
            handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
        }
        if (c == 'F'){              // F stands for not finging varuabke, we have to search function params
            check_params(temp, name_of_node);
        }
        else if(c == 'V'){           // V stands for variable. that will be initialized
            if (temp->let == true && temp->inicialized == true){        // trying to overwrite constant let        
                handle_error(OTHER_SEMANTIC_ERROR);
            }
        }
        if( priradenie_prave(name_of_node) == false){
            return false;
        }
        return true;
    }
    return false;
}

bool idnutie(){
    return priradenie_zost();
}

bool sekvencia(bool in_func, bool in_while, bool in_if){
    current_token = getNextToken();
    dvojbodka_typ_neni = false;
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4){
        if (++instruction_per_line > 1)
        {
            return false;
        }
        if (letnutie(false) == false){
            return false;
        }
    }
    else if (strcmp(current_token.attribute, "var") == 0 && current_token.type == 4){
        if (++instruction_per_line > 1)
        {
            return false;
        }
        
        if (varnutie() == false){
            return false;
        }
    }
    else if (strcmp(current_token.attribute, "while") == 0 && current_token.type == 4){
        DLL_InsertFirst2(&symtable_stack);
        frame_counter++;
        char *name_of_node = string_dup(current_token.attribute);
        insert_func(&symtable_stack.firstElement->treeRoot, name_of_node, 4);
        if (whilnutie(in_func, in_while, in_if) == false){
            return false;
        }
        DLL_DeleteFirst2(&symtable_stack);
        frame_counter--;
    }
    else if (strcmp(current_token.attribute, "if") == 0 && current_token.type == 4){
        DLL_InsertFirst2(&symtable_stack);
        frame_counter++;
        char *name_of_node = string_dup(current_token.attribute);
        insert_func(&symtable_stack.firstElement->treeRoot, name_of_node, 4);
        
        if (ifnutie(in_func, in_while, in_if)== false){
            return false;   
        }
        DLL_DeleteFirst2(&symtable_stack);
        frame_counter--;
    }
    else if (strcmp(current_token.attribute, "func") == 0 && current_token.type == 4){
        char *main_jump = unique_name("main", main_jump_counter);
        sprintf(buffer1.data, "%sJUMP %s\n", buffer1.data, main_jump);
        if (func_declar() == false){
            return false;
        }
        char *main_label = unique_name("main", main_jump_counter);
        sprintf(buffer1.data, "%sLABEL %s\n", buffer1.data, main_label);
        main_jump_counter++;
    }
    else if(current_token.type == 1){
        if (idnutie() == false){
            return false;
        }
    }
    else if (current_token.type == 16)              // built in functions
    {
        if (strcmp(current_token.attribute, "write") == 0)
        {
            if(built_in_write() == false){  
                return false;
            }
        }

    }
    else if (current_token.type == 23 || (current_token.type == 4 && strcmp(current_token.attribute, "return") == 0 )){                 //epsion transition
        unget_token(current_token, current_token.first_in_line);
        if (current_token.type == 4 && return_neni == false){
            return false;
        }
        return true;
    }
    else if(current_token.type == 0){
        return true;
    }
    else{
        return false;
    }
    return sekvencia(in_func, in_while, in_if);
}

void parser(){
    if (sekvencia(false, false, false) == true){
        return;
    }
    else{
        handle_error(SYNTAX_ERROR);
    }
}
    