/*
 * Projekt: Implementace překladače imperativního jazyka IFJ23
 *
 * autori suboru: 
 * xlusti03 - Tomas Lustik
 * 247112 - Peter Gvozdjak
 * xvarsa01 - Adam Varsanyi
 * xperec00 - Mario Perecz
 * 
*/
#include "precedence.h"
#include "compiler.h"
#include "dll.h"

//      +	-	*	/	(	)	i	$
// +	>	>	<	<	<	>	<	>
// -	>	>	<	<	<	>	<	>
// *	>	>	>	>	<	>	<	>
// /	>	>	>	>	<	>	<	>
// (	<	<	<	<	<	=	<	
// )	>	>	>	>		>		>
// i	>	>	>	>		>		>
// $	<	<	<	<	<		<	


//  1. E -> E+E
//  2. E -> E-E
//  3. E -> E*E
//  4. E -> E/E
//  5. E -> (E)
//  6. E -> i
char var;           // V stands for variable, F for function
// char *r, is return type of eval
bool int2double_int_double = false;
bool int2double_double_int = false;
int concat_counter = 0;
bool reduce_exp(char *r, char* name_of_func, bool *extra_paranthasis){
    DLList list;
    DLLElementPtr topTerminal;
    btree_node *token_found = NULL;              // my token
    char current_type = '\0';
    char return_type = '\0';
    DLL_Init(&list);
    DLL_InsertLast(&list, '$');
    char token_char;
    static int counter = 0;    //When token is '(' add 1, when ')' sub 1
    while (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8 ||current_token.type == 5 || current_token.type == 20 || current_token.type == 21)    // stale sme vo vyraze
    {
        token_char = (current_token.attribute)[0];
        if(token_char == '('){
            counter++;
        }
        else if(token_char == ')'){
            counter--;
        }
        if(current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8){
            if(current_token.type == 1){                // if we get identifier
                char *name_of_node = '\0';
                name_of_node = string_dup(current_token.attribute);
                token_found = find_declaration(&symtable_stack, name_of_node,&var);
                if (token_found == NULL || token_found->inicialized == false){
                    handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
                }
                
                if (var == 'F'){
                    int i = check_params(token_found, name_of_node);
                    current_type = token_found->paramsArray[i].type;
                }
                else{
                    current_type = token_found->data_type;
                }
            }

            else if (current_token.type == 2)
            {
                current_type = 'I';
            }
            else if (current_token.type == 3)
            {
                current_type = 'D';
            }
            else{
                current_type = 'S';
            }

            if (return_type == '\0'){                                           // setting result type according to variable
                return_type = current_type;
            }
            else{
                if ((return_type == 'S' && current_type != 'S') || (return_type != 'S' && current_type == 'S'))            // operation between string and no string
                {
                    handle_error(SEMANTIC_TYPE_COMPATIBILITY);
                }
                
                if (return_type == 'I' && current_type == 'D')              // int + double
                {
                    int2double_int_double = true;
                    return_type = 'D';
                }
                else if (return_type == 'D' && current_type == 'I')         // double + int
                {
                    int2double_double_int = true;
                    return_type = 'D';
                }        
            
            }
            
            token_char = 'i';
        }

        if((counter >= 0)){
            topTerminal =  DLL_TopTerminal(&list, true);
            if(token_char == '+' || token_char == '-'){
                if (token_char != '+' && return_type == 'S'){                                   //strings are not working with '-'
                    handle_error(SEMANTIC_TYPE_COMPATIBILITY);
                } 
                if(topTerminal->data == '(' || topTerminal->data == '$'){
                    pushLess(&list, token_char);
                }
                else{
                    reduce(&list, return_type);
                    unget_token(current_token, current_token.first_in_line);
                }
            }
            else if(token_char == '*' || token_char == '/'){                                    // Strings are not working with opration '*' or '/'
                if (return_type == 'S'){
                    handle_error(SEMANTIC_TYPE_COMPATIBILITY);
                } 
                if(topTerminal->data == '+' || topTerminal->data == '-' || topTerminal->data == '(' || topTerminal->data == '$'){
                    pushLess(&list, token_char);
                }
                else{
                    reduce(&list, return_type);
                    unget_token(current_token, current_token.first_in_line);
                }
            }
            else if(token_char == '('){
                if(topTerminal->data == ')' || topTerminal->data == 'i'){
                    DLL_Dispose(&list);
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    pushLess(&list, token_char);
                }
            }
            else if(token_char == ')'){
                if(topTerminal->data == '('){
                    if(DLL_GetLast(&list) == '('){
                        DLL_Dispose(&list);
                        handle_error(SYNTAX_ERROR);
                    }
                    DLL_InsertLast(&list, token_char);
                }
                else if(topTerminal->data == '$'){
                    DLL_Dispose(&list);
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    reduce(&list, return_type);
                    unget_token(current_token, current_token.first_in_line);
                    counter++;              // after putting ')' back to input, incrementing bracket counter, so after next token we have correct counting of brackets 
                }
            }
            else if(token_char == 'i'){
                if(topTerminal->data == ')' || topTerminal->data == 'i'){
                    DLL_Dispose(&list);
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    pushLess(&list, token_char);
                    DLL_InsertValueLast(&list, current_token.type, current_token.attribute);
                }
            }
            else if(token_char == '$'){
                if(topTerminal->data == '(' || topTerminal->data == '$'){
                    DLL_Dispose(&list);
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    reduce(&list, return_type);
                    unget_token(current_token, current_token.first_in_line);
                }  
            }
        }
        else{
            counter++;              // ")" doesnt belong to expresion, so we uncount it
            break;
        }
        current_token = getNextToken();
        if(token_char == 'i'){
            if (current_token.first_in_line == true){
                break;
            }
        }
        
    }

    while(counter != 0){                               // parentheses not closed properly or next token is relation operator
        if (current_token.type == 6){
            counter--;
            *extra_paranthasis = true;
            break;
        }
        
        DLL_Dispose(&list);
        handle_error(SYNTAX_ERROR);
        break;
    }

    unget_token(current_token, current_token.first_in_line);                    // unget token that doesnt belong to expression
    topTerminal = DLL_TopTerminal(&list, true);
        while(topTerminal->data != '$'){
            reduce(&list, return_type);
            topTerminal = DLL_TopTerminal(&list, true);
        }
    DLL_Dispose(&list);
    *r = return_type;
    return true;
}

void pushLess(DLList *list, char c){
    DLL_InsertAfter(list, '<');
    DLL_InsertLast(list, c);
}

void pushEqual(DLList *list, char c){
    DLL_InsertLast(list, c);
}

void reduce(DLList *list, char return_type){
    DLLElementPtr temp = list->lastElement;
    char cache[4] = {'\0', '\0', '\0', '\0'};  //Temporary array to store items from "stack" before reducing
    for(int i = 0; list->lastElement->data != '<'; i++){
        cache[i] = DLL_DeleteLast(list);
    }
    DLL_DeleteLast(list);
    if(strcmp(cache, "E+E") == 0){
        if(return_type == 'S'){
            concat_counter++;
            if (frame_counter == 0){
                sprintf(buffer1.data, "%sDEFVAR GF@TMP1$%d\n", buffer1.data, concat_counter);
                sprintf(buffer1.data, "%sDEFVAR GF@TMP2$%d\n", buffer1.data, concat_counter);
                sprintf(buffer1.data, "%sDEFVAR GF@RES$%d\n", buffer1.data,concat_counter);
                sprintf(buffer1.data, "%sPOPS GF@TMP2$%d\n", buffer1.data,concat_counter);
                sprintf(buffer1.data, "%sPOPS GF@TMP1$%d\n", buffer1.data,concat_counter);
                sprintf(buffer1.data, "%sCONCAT GF@RES$%d GF@TMP1$%d GF@TMP2$%d\n",buffer1.data,concat_counter, concat_counter, concat_counter);
                sprintf(buffer1.data,"%sPUSHS GF@RES$%d\n",buffer1.data,concat_counter);

            }
            else{
                sprintf(buffer1.data, "%sDEFVAR LF@TMP1$%d\n", buffer1.data, concat_counter);
                sprintf(buffer1.data, "%sDEFVAR LF@TMP2$%d\n", buffer1.data, concat_counter);
                sprintf(buffer1.data, "%sDEFVAR LF@RES$%d\n", buffer1.data,concat_counter);
                sprintf(buffer1.data, "%sPOPS LF@TMP2$%d\n", buffer1.data,concat_counter);
                sprintf(buffer1.data, "%sPOPS LF@TMP1$%d\n", buffer1.data,concat_counter);
                sprintf(buffer1.data, "%sCONCAT LF@RES$%d LF@TMP1$%d LF@TMP2$%d\n",buffer1.data,concat_counter, concat_counter, concat_counter);
                sprintf(buffer1.data,"%sPUSHS LF@RES$%d\n",buffer1.data,concat_counter);
            }
        }
        else{
            sprintf(buffer1.data, "%sADDS\n",buffer1.data);
        }
    }
    else if(strcmp(cache, "E-E") == 0){
        sprintf(buffer1.data, "%sSUBS\n",buffer1.data);
    }
    else if(strcmp(cache, "E*E") == 0){
        sprintf(buffer1.data, "%sMULS\n",buffer1.data);
    }
    else if(strcmp(cache, "E/E") == 0){
        if(return_type == 'D'){
            sprintf(buffer1.data, "%sDIVS\n",buffer1.data);
        }
        else{
            sprintf(buffer1.data, "%sIDIVS\n",buffer1.data);
        }
    }
    else if(strcmp(cache, "(E)") == 0){

    }
    else if(strcmp(cache, "i") == 0){
        if(temp->type == 'I'){
            sprintf(buffer1.data, "%sPUSHS int@%d\n",buffer1.data, temp->valueI);
            if (int2double_double_int)
            {
                sprintf(buffer1.data, "%sINT2FLOATS\n", buffer1.data);
                int2double_double_int = false;
            }
            
        }
        else if(temp->type == 'D'){
            if (int2double_int_double)
            {
                sprintf(buffer1.data, "%sINT2FLOATS\n", buffer1.data);
                int2double_int_double = false;
            }            
            sprintf(buffer1.data, "%sPUSHS float@%a\n",buffer1.data, temp->valueD);
        }
        else if (temp->type == 'S'){
            sprintf(buffer1.data, "%sPUSHS string@%s\n", buffer1.data, temp->string);
        }
        else if (temp->type == 'V'){
            if (frame_counter - anti_zanorenie == 0 && var != 'F'){                // in global scope
                char *push_var = unique_name(temp->string, 0);
                sprintf(buffer1.data, "%sPUSHS GF@%s\n", buffer1.data, push_var);
            }
            else if(frame_counter-anti_zanorenie > 0){
                char *push_var = unique_name(temp->string, frame_counter - anti_zanorenie);
                sprintf(buffer1.data, "%sPUSHS LF@%s\n",buffer1.data, push_var);
            }
            else if(var == 'F'){
                char *push_var = unique_name(temp->string,  1);
                sprintf(buffer1.data, "%sPUSHS LF@%s\n",buffer1.data, push_var);
            }
        }
    }
    DLL_InsertLast(list, 'E');
}