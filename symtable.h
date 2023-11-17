#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct btree_node{                      // structure of node
    int token_type;                             // pre klucove slova : to bude 4, ak to tam nebude, novy strom kde toto bude 1                            
    int key;                                    // identifier
    char *name_of_symbol;                       // attribute
    char *func_param;
    int func_num_of_param;
    struct btree_node *left;                    // pointer to left child
    struct btree_node *right;                   // pointer to right child
    int height;                                 // height of tree
} btree_node;

void insert(btree_node **root, int token_type, int key, char *name_of_symbol, char *func_param, int func_num_of_param);
bool search(btree_node *root, int token_type, int key, char **name_of_symbol, char **func_param, int *func_num_of_param);
void node_delete(btree_node **root, int token_type, int key);
void tree_dispose(btree_node **root);
void printtree(btree_node *root, int level);
int height_of_node(btree_node * root);
#endif
