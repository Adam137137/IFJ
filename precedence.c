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

bool reduce_exp(){
    // uz je nacitany token vyrazu -> pojde do while
    DLList list;
    DLLElementPtr topTerminal;
    DLL_Init(&list);
    DLL_InsertLast(&list, '$');
    while (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 5 || current_token.type == 20 || current_token.type == 21)    // stale sme vo vyraze
    {
        char token_char = (current_token.attribute)[0];
        if(current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7){
            token_char = 'i';
        }
        printf("%c\n", token_char);
        // TODO: nacitat dalsie tokeny a zistit ci ide o validny vyraz
        topTerminal =  DLL_TopTerminal(&list, true);
        if(token_char == '+' || token_char == '-'){ 
            if(topTerminal->data == '(' || topTerminal->data == '$'){
                pushLess(&list, token_char);
            }
            else{
                reduce(&list);
            }
        }
        else if(token_char == '*' || token_char == '/'){
            if(topTerminal->data == '+' || topTerminal->data == '-' || topTerminal->data == '(' || topTerminal->data == '$'){
                pushLess(&list, token_char);
            }
            else{
                reduce(&list);
            }
        }
        else if(token_char == '('){
            pushLess(&list, token_char);
        }
        else if(token_char == ')'){
            if(topTerminal->data == '('){
                DLL_InsertLast(&list, token_char);
            }
            else if(topTerminal->data == '$'){
                //Free
                DLL_Dispose(&list);
                handle_error(2);
            }
        }
        else if(token_char == 'i'){
            if(topTerminal->data == ')' || topTerminal->data == 'i'){
                //Free
                DLL_Dispose(&list);
                handle_error(2);
            }
            else{
                pushLess(&list, token_char);
            }
        }
        else if(token_char == '$'){
            if(topTerminal->data == '(' || topTerminal->data == '$'){
                //Free
                DLL_Dispose(&list);
                handle_error(2);
            }
            else{
                pushLess(&list, token_char);
            }
        }
        
        // ak sa neda urobit vyraz return false
        current_token = getNextToken();
    }
    
    // nacitany token uz nie je vyraz ale nieco ine, treba ho asi vratit
    DLL_Dispose(&list);
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
    char cache[4];  //Temporary array to store items from "stack" before reducing
    for(int i = 0; list->lastElement->data != '<'; i++){
        cache[i] = DLL_DeleteLast(list);
    }
    for(int j = 0; cache[j] != '\0'; j++){
        printf("%c", cache[j]);
        printf("\n");
    }
}