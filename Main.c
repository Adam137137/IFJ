#include "compiler.h"
FILE *file = NULL;
bool prvy_prechod;
DLList2 symtable_stack;
bool testovanie = false;
dynamic_buffer buffer1;
dynamic_buffer buffer2;

void first_analysis_parser(){
    current_token = getNextToken();
    while (current_token.type != 0)
    {
        // printf("Type: %d     ", current_token.type);
        // printf("Attribute: %s         ", current_token.attribute);
        // printf("prvy: %d\n", (int)current_token.first_in_line);
        
        if (strcmp(current_token.attribute, "func") == 0 && current_token.type == 4){            
            if (func_declar() == false){
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
    initDynamicArray(&buffer1);                 // for code gen
    initDynamicArray(&buffer2);                 // for other things
    sprintf(buffer1.data, ".IFJcode23\n");
    // printf("%d",buffer1.size);

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
        DLL_InsertFirst2(&symtable_stack);
        first_analysis_parser();
        prvy_prechod = false;
        
        rewind(stdin);
        clearerr(stdin);
        //insert_variable(&symtable_stack.firstElement->treeRoot, "anoo", current_token.type, true, "", true);
        // printtree(symtable_stack.firstElement->treeRoot,0);
        //insert_data_type(&symtable_stack.firstElement->treeRoot, "anoo", 'I');
        parser();
    }
    
    file = fopen("IFJcode23.txt", "w");
    fprintf(stdout, "%s", buffer1.data);
    fprintf(file, "%s", buffer1.data);


    // char *new_name = unique_name("auto", 4);
    // printf("nove meno: %s\n", new_name);
    // btree_node *root = NULL;
    // // int x = 0;
    // insert_func(&root, "automat",4);
    // insert_func(&root, "auto",4);
    // insert_func(&root, "lol",4);
    // insert_func(&root, "ahojt",4);
    // insert_params(&root, "auto", 1, "string");
    // // printtree(root, x);
    // tree_dispose(&root);
    return 0;
}