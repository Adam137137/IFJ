#include "compiler.h"
FILE *file = NULL;
bool testovanie = true;

int main(int argc, char *argv[]){
    (void) argc;
    (void) argv;
    if (testovanie)
    {
        printf("Compiler - testing script:\n\n");
        for (int i = 0; i < 3; i++)
        {
            tests_init(i);
            file = fopen("test.txt", "r");
            parser();
        }
    }
    else{
        file = fopen("prekladany_subor.txt", "r");
        parser();
    }

    

    // strom testy
    // btree_node *root = NULL;
    // int x = 0;
    // char *found_name_of_symbol = NULL;
    // char *func_params = NULL;
 
    // insert(&root, 4, 10, "double", "", 0);
    // insert(&root, 4, 13, "else", "", 0);
    // insert(&root, 4, 11, "if", "", 0);
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
    // tree_dispose(&root);

    // 	DLList list;
    // 	DLL_Init(&list);
    // 	DLL_InsertLast(&list, '$');
    // 	DLL_InsertLast(&list, 'E');
    // 	DLL_InsertLast(&list, '<');
    // 	DLL_InsertLast(&list, 'F');
    // 	DLL_InsertLast(&list, '>');
    // 	DLLElementPtr top = DLL_TopTerminal(&list, true);
    // 	printf("%c\n", top->data);

    return 0;
}