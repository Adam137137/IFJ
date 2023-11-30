#include "compiler.h"
FILE *file = NULL;
btree_node *tree_main = NULL;
bool prvy_prechod;
DLList2 symtable_stack;
bool testovanie = false;



void first_analysis_parser(){
    current_token = getNextToken();
    while (current_token.type != 0)
    {
        // printf("Type: %d     ", current_token.type);
        // printf("Attribute: %s         ", current_token.attribute);
        // printf("prvy: %d\n", (int)current_token.first_in_line);
        
        if (strcmp(current_token.attribute, "func") == 0 && current_token.type == 4){            
            if (func_declar(true) == false){
                handle_error(SYNTAX_ERROR);
            }

        }
        current_token = getNextToken();
    }
}

int main(int argc, char *argv[]){
    (void) argc;
    (void) argv;
        
    prvy_prechod = true;

    if (testovanie)
    {
        file = fopen("test.txt", "r");
        parser();
        // printf("Compiler - testing script:\n\n");
        // for (int i = 0; i < 3; i++)
        // {
        //     tests_init(i);
        //     file = fopen("test.txt", "r");
        //     parser();
        // }
    }
    else{
        file = stdin;
        first_analysis_parser();
        puts("prve spustenie skoncilo");
        prvy_prechod = false;
        printtree(tree_main, 0);
        
        rewind(stdin);
        clearerr(stdin);
        parser();
        // puts("druhe spustenie skoncilo");
    }
    return 0;

    

    // strom testy
    // btree_node *root = NULL;
    // int x = 0;
    //char *found_name_of_symbol = NULL;
    //char *func_params = NULL;
    // insert(&root, "nieco", 1, false, "", false, 0, "", 0, "", 0, "");
    // insert(&root, "else", 1, false, "", false, 0, "", 0, "", 0, "");
    // insert(&root, "test", 1, false, "", false, 0, "", 0, "", 0, "");
    // int c = height_of_node(root);
    // printf("main koniec %d\n", c);
    // printtree(root, x);
    // insert(&root, 4, 6, "ugabugag", "", 0);
    // insert(&root, 4, 7, "lol", "", 0);
    // insert(&root, 4, 4, "XDDd", "", 0);
    // insert(&root, 4, 1, "uff", "", 0);
    // insert(&root, 4, 9, "memem", "", 0);
    // insert(&root, 4, 5, "memem", "", 0);
    // node_delete(&root, 4, 6);
    // printtree(root, x);
    // bool result = search(root, 4, 6, &found_name_of_symbol, &func_params, 0);
    // if (result) {
    //     printf("Found: %s\n", found_name_of_symbol);
    //     free(found_name_of_symbol);    
    // } else {
    //     printf("Not found.\n");
    // }
//    tree_dispose(&root);

    	// DLList list;
    	// DLL_Init(&list);
    	// DLL_InsertLast(&list, '$');
    	// DLL_InsertLast(&list, 'E');
    	// DLL_InsertLast(&list, '<');
    	// DLL_InsertLast(&list, 'F');
    	// DLL_InsertLast(&list, '>');
    	// DLLElementPtr top = DLL_TopTerminal(&list, true);
    	// printf("%c\n", top->data);
}