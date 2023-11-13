#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct btree_node{                      // structure of node
    int token_type;                             // pre klucove slova : to bude 4, ak to tam nebude, novy strom kde toto bude 1                            
    int key;                                   // identifier
    char *name_of_symbol;                       // value of element
    char *func_param;
    int func_num_of_param;
    struct btree_node *left;                    // pointer to left child
    struct btree_node *right;                   // pointer to right child
} btree_node;

btree_node *create_node(int token_type, int key, char *name_of_symbol, char *func_param, int func_num_of_param);
void insert(btree_node **root, int token_type, int key, char *name_of_symbol, char *func_param, int func_num_of_param);
bool search(btree_node *root, int token_type, int key, char **name_of_symbol, char **func_param, int *func_num_of_param);
void node_delete(btree_node **root, int token_type, int key);
void printtab(int numtabs);
void printtree(btree_node *root, int level);
