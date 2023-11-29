#ifndef SYMTABLE_H
#define SYMTABLE_H
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

typedef struct btree_node{                      // structure of node
    char *name_of_symbol;                       // identifier
    int token_type;                             // variable (1) / func (4),  if it is variable we check 
    
    // only in variable 
    bool inicialized;                                                  
    char *data_type;                            // int, double, string                           
    bool let;                                   // if we use let, this is true or in let it is var
    int value_int;                              // if data_type is int
    char *value_string;                         // if data_type is string
    double value_double;                        // if data_type is double

    // only in functions
    char *func_param;                           // params of func
    int func_num_of_param;
    char *return_type;

    
    
    struct btree_node *left;                    // pointer to left child
    struct btree_node *right;                   // pointer to right child
    int height;                                 // height of node
} btree_node;

char *string_dup(char *string);
void init(btree_node **root);
void insert(btree_node **root, char *name_of_symbol, int token_type, bool inicialized, char *data_type, bool let, int value_int, char *value_string, double value_double, char *func_param, int func_num_of_param, char *return_type);
btree_node *search(btree_node *root, char *name_of_symbol);
// void node_delete(btree_node **root, int token_type);
void tree_dispose(btree_node **root);
void printtree(btree_node *root, int level);
#endif
