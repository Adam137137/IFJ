#include "compiler.h"
FILE *file = NULL;
bool prvy_prechod;
DLList2 symtable_stack;
bool testovanie = false;
dynamic_buffer buffer1;
dynamic_buffer buffer2;
int instruction_per_line = 0;

void first_analysis_parser(){
    current_token = getNextToken();
    while (current_token.type != 0)
    {
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
    sprintf(buffer1.data, ".IFJcode23\n");


    file = stdin;
    DLL_InsertFirst2(&symtable_stack);
    first_analysis_parser();
    prvy_prechod = false;
    
    rewind(stdin);
    clearerr(stdin);
    parser();
    
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