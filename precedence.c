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

// char *r, is return type of eval
bool reduce_exp(char *r, char* name_of_func, bool after_relation_operator){                                   // first token is already loaded
    // puts("nova exp \n \n");
    DLList list;
    DLLElementPtr topTerminal;
    btree_node *token_found = NULL;              // my token
    char current_type = '\0';
    char return_type = '\0';
    DLL_Init(&list);
    DLL_InsertLast(&list, '$');
    char token_char;
    static int counter = 0;    //When token is '(' add 1, when ')' sub 1
    printf("count: %d \n", counter);
    while (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 5 || current_token.type == 20 || current_token.type == 21)    // stale sme vo vyraze
    {
        token_char = (current_token.attribute)[0];
        // printf("nacitanie : %c \n", token_char);
        // token_print();
        if(token_char == '('){
            counter++;
        }
        else if(token_char == ')'){
            counter--;
        }
        if(current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7){
            if(current_token.type == 1){                // if we get identifier
                char *name_of_node = '\0';
                name_of_node = string_dup(current_token.attribute);
                //printf("Noda: %s\n", name_of_node);
                //printf("%s", name_of_function);
                char c;
                token_found = find_declaration(&symtable_stack, name_of_node,&c);
                //printf("CO tot je %c\n",c);
                //printf("Co som nasiel %s\n", token_found->name_of_symbol);
                // btree_node *tmp = NULL;
                // if(token_found == NULL){            // kukneme globalny ramec
                //     tmp =find_function_in_global(&symtable_stack,name_of_func);
                //     if (tmp == NULL){
                //         //puts("nedeklarovana premenna alebo nedefinovana premenna\n");
                //         handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
                //     }
                //     bool found = false;
                //     for(int i =0; i < tmp->func_num_of_param;i++){
                //         if(strcmp(tmp->paramsArray[i].identif, name_of_node) == 0){
                //             found = true;
                //             break;
                //         }
                //     }
                //     if (!found){
                //         //puts("nedeklarovana premenna alebo nedefinovana premenna\n");
                //         handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
                //     }
                    
                //}
                //token_found_param = find_declaration(&symtable_stack, symtable_stack.firstElement->treeRoot->name_of_symbol);
                
                //printf("Prvy param %s\n", token_found->name_of_symbol);
                // if(token_found == NULL || token_found->inicialized == false){
                //     puts("nedeklarovana premenna alebo nedefinovana premenna\n");
                //     handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
                // }
                if (c == 'F'){
                    bool found = false;
                    for(int i =0; i < token_found->func_num_of_param ;i++){			// prehladanie funkcnych parametrov na najdenie id
                        if(strcmp(token_found->paramsArray[i].identif, name_of_node) == 0){
                            current_type = token_found->paramsArray[i].type;
                            found = true;
                            break;
                        }
                    }
                        if (!found){
                            printf("ID nebolo deklarovane nikde-precedencna\n");
                            handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
                    }
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

            if (return_type == '\0'){                                           // nastavenie vysledneho typu podla prvej premennej
                return_type = current_type;
            }
            else{
                if ((return_type == 'S' && current_type != 'S') || (return_type != 'S' && current_type == 'S'))            // String a nieco ine
                {
                    //puts("v precedencnej");
                    handle_error(SEMANTIC_TYPE_COMPATIBILITY);
                }
                if (!(return_type == 'D'))                                      // vysledny double nemoze byt zmeneny na int
                {
                    return_type = current_type;
                }
            }
            
            token_char = 'i';
        }

        if((counter >= 0)){
            topTerminal =  DLL_TopTerminal(&list, true);
            if(token_char == '+' || token_char == '-'){
                if (token_char != '+' && return_type == 'S'){                                   // Stringy nepracuju s operaciou '-'
                    //puts("v precedencnej");
                    handle_error(SEMANTIC_TYPE_COMPATIBILITY);
                } 
                if(topTerminal->data == '(' || topTerminal->data == '$'){
                    pushLess(&list, token_char);
                    //DLL_PrintList(&list);
                }
                else{
                    // printf("Redukcia1\n");
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                    // DLL_PrintList(&list);
                    unget_token(current_token, current_token.first_in_line);
                }
            }
            else if(token_char == '*' || token_char == '/'){                                    // Stringy nepracuju s operaciou '*' ani '/'
                if (return_type == 'S'){
                    //puts("v precedencnej");
                    handle_error(SEMANTIC_TYPE_COMPATIBILITY);
                } 
                if(topTerminal->data == '+' || topTerminal->data == '-' || topTerminal->data == '(' || topTerminal->data == '$'){
                    pushLess(&list, token_char);
                    //DLL_PrintList(&list);
                }
                else{
                    // printf("Redukcia2\n");
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                    // DLL_PrintList(&list);
                    unget_token(current_token, current_token.first_in_line);
                }
            }
            else if(token_char == '('){
                if(topTerminal->data == ')' || topTerminal->data == 'i'){
                    DLL_Dispose(&list);
                    // printf("test72\n");
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    pushLess(&list, token_char);
                }
                //DLL_PrintList(&list);
            }
            else if(token_char == ')'){
                //printf("%c \n", topTerminal->data);
                if(topTerminal->data == '('){
                    if(DLL_GetLast(&list) == '('){
                        DLL_Dispose(&list);
                        // printf("test84\n");
                        handle_error(SYNTAX_ERROR);
                    }
                    DLL_InsertLast(&list, token_char);
                    //DLL_PrintList(&list);
                }
                else if(topTerminal->data == '$'){
                    //Free
                    DLL_Dispose(&list);
                    // printf("test92\n");
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    //printf("Redukcia3\n");
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                    // DLL_PrintList(&list);
                    unget_token(current_token, current_token.first_in_line);      // ... 2hours of debugging
                    counter++;                      // after putting ')' back to input, incrementing bracket counter, so after next token we have correct counting of brackets 
                }
            }
            else if(token_char == 'i'){
                if(topTerminal->data == ')' || topTerminal->data == 'i'){
                    //Free
                    DLL_Dispose(&list);
                    //  printf("test105\n");
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    pushLess(&list, token_char);
                    DLL_InsertValueLast(&list, current_token.type, current_token.attribute);
                    //DLL_PrintList(&list);
                }
            }
            else if(token_char == '$'){
                if(topTerminal->data == '(' || topTerminal->data == '$'){
                    //Free
                    DLL_Dispose(&list);
                    //printf("test234\n");
                    handle_error(SYNTAX_ERROR);
                }
                else{
                    // printf("Redukcia4\n");
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                    // DLL_PrintList(&list);
                    unget_token(current_token, current_token.first_in_line);
                }  
            }
        }
        else{
            counter++;                                                          // ")" doesnt belong to expresion, so we uncount it
            break;
        }
        current_token = getNextToken();
        if(token_char == 'i'){
            if (current_token.first_in_line == true){
                break;
            }
        }
        
    }

    while(counter != 0){                               // parenthesis not closed properly or next token is relation operator
        
        if (current_token.type == 6)
        {
            counter--;
            break;
        }
        
        DLL_Dispose(&list);
        handle_error(SYNTAX_ERROR);
        break;
    }

    unget_token(current_token, current_token.first_in_line);                    // unget token that doesnt belong to expression
    topTerminal = DLL_TopTerminal(&list, true);
        while(topTerminal->data != '$'){
            // printf("TOP: %c\n", topTerminal->data);
            // printf("koniec\n");
            reduce(&list);
            // DLL_PrintList(&list);
            topTerminal = DLL_TopTerminal(&list, true);
        }
    DLL_Dispose(&list);
    *r = return_type;             //zapisanie do parametru
    // puts("relacia presla");
    return true;
}

void pushLess(DLList *list, char c){
    DLL_InsertAfter(list, '<');
    DLL_InsertLast(list, c);
}

void pushEqual(DLList *list, char c){
    DLL_InsertLast(list, c);
}

void reduce(DLList *list){
    DLLElementPtr temp = list->lastElement;
    char cache[4] = {'\0', '\0', '\0', '\0'};  //Temporary array to store items from "stack" before reducing, warning beacuse of unfinished code
    for(int i = 0; list->lastElement->data != '<'; i++){
        cache[i] = DLL_DeleteLast(list);
        // printf("tmp = %d\n", cache[i]);
        // printf("preco = %d\n", i);

    }
    // printf("cache[1] = %c\n", cache[1]);
    DLL_DeleteLast(list);
    // printf("cache = \"%s\"\n", cache);
    if(strcmp(cache, "E+E") == 0){
        sprintf(buffer1.data, "%sADDS\n",buffer1.data);
        //printf("Pravidlo 1\n");
    }
    else if(strcmp(cache, "E-E") == 0){
        sprintf(buffer1.data, "%sSUBS\n",buffer1.data);
        //printf("Pravidlo 2\n");
    }
    else if(strcmp(cache, "E*E") == 0){
        sprintf(buffer1.data, "%sMULS\n",buffer1.data);
        //printf("Pravidlo 3\n");
    }
    else if(strcmp(cache, "E/E") == 0){
        sprintf(buffer1.data, "%sDIVS\n",buffer1.data);
        //printf("Pravidlo 4\n");
    }
    else if(strcmp(cache, "(E)") == 0){
        //printf("Pravidlo 5\n");
    }
    else if(strcmp(cache, "i") == 0){
        //printf("typ %c\n", temp->type);
        if(temp->type == 'I'){
            sprintf(buffer1.data, "%sPUSHS int@%d\n",buffer1.data, temp->valueI);
        }
        else if(temp->type == 'D'){
            sprintf(buffer1.data, "%sPUSHS float@%a\n",buffer1.data, temp->valueD);
        }
        else if(temp->type == 'S'){
            sprintf(buffer1.data, "%sPUSHS string@%s\n",buffer1.data, temp->string);
        }
        else if (temp->type == 'V'){
            //char *push_var = unique_name(temp->string, frame_counter);
            if (frame_counter - anti_zanorenie == 0){                // sme v globalnom ramci
                char *push_var = unique_name(temp->string, 0);
                sprintf(buffer1.data, "%sPUSHS GF@%s\n", buffer1.data, push_var);
            }
            else if(frame_counter-anti_zanorenie > 0){
                char *push_var = unique_name(temp->string, frame_counter - anti_zanorenie);
                sprintf(buffer1.data, "%sPUSHS LF@%s\n",buffer1.data, push_var);
            }
        }
        //printf("Pravidlo 6\n");
    }
    // printf("List po redukcii:");
    // DLL_PrintList(list);
    DLL_InsertLast(list, 'E');
}