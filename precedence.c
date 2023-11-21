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
    char token_char;
    int counter = 0;    //When token is '(' add 1, when ')' sub 1
    while (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 5 || current_token.type == 20 || current_token.type == 21)    // stale sme vo vyraze
    {
        token_char = (current_token.attribute)[0];
        token_print();
        if(token_char == '('){
            counter++;
        }
        else if(token_char == ')'){
            counter--;
        }
        if(current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7){
            token_char = 'i';
        }

        if(!(counter < 0)){
            topTerminal =  DLL_TopTerminal(&list, true);
            if(token_char == '+' || token_char == '-'){ 
                if(topTerminal->data == '(' || topTerminal->data == '$'){
                    pushLess(&list, token_char);
                    // DLL_PrintList(&list);
                }
                else{
                    // printf("Redukcia1\n");
                    // DLL_PrintList(&list);
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                }
            }
            else if(token_char == '*' || token_char == '/'){
                if(topTerminal->data == '+' || topTerminal->data == '-' || topTerminal->data == '(' || topTerminal->data == '$'){
                    pushLess(&list, token_char);
                    // DLL_PrintList(&list);
                }
                else{
                    // printf("Redukcia2\n");
                    // DLL_PrintList(&list);
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                }
            }
            else if(token_char == '('){
                if(topTerminal->data == ')' || topTerminal->data == 'i'){
                    DLL_Dispose(&list);
                    printf("test72\n");
                    handle_error(2);
                }
                else{
                    pushLess(&list, token_char);
                }
                // DLL_PrintList(&list);
            }
            else if(token_char == ')'){
                if(topTerminal->data == '('){
                    if(DLL_GetLast(&list) == '('){
                        DLL_Dispose(&list);
                        printf("test84\n");
                        handle_error(2);
                    }
                    DLL_InsertLast(&list, token_char);
                    // DLL_PrintList(&list);
                }
                else if(topTerminal->data == '$'){
                    //Free
                    DLL_Dispose(&list);
                    printf("test92\n");
                    handle_error(2);
                }
                else{
                    // printf("Redukcia3\n");
                    // DLL_PrintList(&list);
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                }
            }
            else if(token_char == 'i'){
                if(topTerminal->data == ')' || topTerminal->data == 'i'){
                    //Free
                    DLL_Dispose(&list);
                    printf("test105\n");
                    handle_error(2);
                }
                else{
                    pushLess(&list, token_char);
                    // DLL_PrintList(&list);
                }
            }
            else if(token_char == '$'){
                if(topTerminal->data == '(' || topTerminal->data == '$'){
                    //Free
                    DLL_Dispose(&list);
                    printf("test116\n");
                    handle_error(2);
                }
                else{
                    // printf("Redukcia4\n");
                    // DLL_PrintList(&list);
                    //printf("first: %c\n Last: \n", topTerminal->data);
                    reduce(&list);
                }
            }
        }
        current_token = getNextToken();
    }
    puts("posledny token:");
    token_print();
    unget_token(current_token);
    token_print();
    topTerminal = DLL_TopTerminal(&list, true);
        while(topTerminal->data != '$'){
        // printf("TOP: %c\n", topTerminal->data);
        // printf("koniec\n");
        // DLL_PrintList(&list);
        reduce(&list);
        topTerminal = DLL_TopTerminal(&list, true);
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
    char cache[4] = {'\0', '\0', '\0', '\0'};  //Temporary array to store items from "stack" before reducing
    for(int i = 0; list->lastElement->data != '<'; i++){
        cache[i] = DLL_DeleteLast(list);
        // printf("tmp = %d\n", cache[i]);
        // printf("preco = %d\n", i);

    }
    // printf("cache[1] = %c\n", cache[1]);
    DLL_DeleteLast(list);
    // for(int j = 0; cache[j] != '\0'; j++){
    //     // printf("test %d\n", j);
    //     // printf("redukuj = %d", cache[j]);
    //     // printf("\n");
    //     // printf("top = %c\n", list->lastElement->data);
    // }
    // printf("cache = \"%s\"\n", cache);
    if(strcmp(cache, "E+E") == 0){
        printf("Pravidlo 1\n");
    }
    else if(strcmp(cache, "E-E") == 0){
        printf("Pravidlo 2\n");
    }
    else if(strcmp(cache, "E*E") == 0){
        printf("Pravidlo 3\n");
    }
    else if(strcmp(cache, "E/E") == 0){
        printf("Pravidlo 4\n");
    }
    else if(strcmp(cache, "(E)") == 0){
        printf("Pravidlo 5\n");
    }
    else if(strcmp(cache, "i") == 0){
        printf("Pravidlo 6\n");
    }
    DLL_InsertLast(list, 'E');
    // printf("List po redukcii:");
    // DLL_PrintList(list);
    unget_token(current_token);
}