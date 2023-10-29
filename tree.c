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
    struct btree_node *left;                    // pointer to left son
    struct btree_node *right;                   // pointer to right son
} btree_node;

btree_node *create_node(int token_type, int key, char *name_of_symbol, char *func_param, int func_num_of_param){
    btree_node *node = (btree_node *)malloc(sizeof(btree_node));
    if(node == NULL){                           // check for allocation
        return NULL;                            // podla zadania sa vracia 99, ale to je dobra chujovina potom
    }
    node->token_type = token_type;
    node->key = key;

    node->name_of_symbol = strdup(name_of_symbol);                // duplicating the string given, allocating memmory for it and adding it to tree
    if(node->name_of_symbol == NULL){
        free(node);
        return NULL;                                // podla zadania sa vracia 99, ale to je dobra chujovina potom
    }
    
    node->func_param = strdup(func_param);
    if(node->func_param == NULL){
        free(node);
        free(node->name_of_symbol);
        return NULL;
    }

    node->func_num_of_param = func_num_of_param;

    node->left = NULL;
    node->right = NULL;
    return node;
}

void insert(btree_node **root, int token_type, int key, char *name_of_symbol, char *func_param, int func_num_of_param){
    if(*root == NULL){
        *root = create_node(token_type, key, name_of_symbol, func_param, func_num_of_param);
    }
    else{
        if(key < (*root)->key){
            insert(&((*root)->left), token_type, key, name_of_symbol, func_param, func_num_of_param);    
        }
        else if(key > (*root)->key){
            insert(&((*root)->right), token_type, key, name_of_symbol, func_param, func_num_of_param);
        }
    }
}

void printtab(int numtabs){
    for(int i = 0; i < numtabs; i++){
        printf("\t");
    }
}

void printtree(btree_node *root, int level){
    if(root == NULL){
        printtab(level);
        printf("empty root\n");
        return;
    }
    printtab(level);
    printf("root key : %d\n", root->key);
    printf("root name_of_symbol : %s\n", root->name_of_symbol);
    printtab(level);
    
    printf("left\n");
    printtree(root->left, level+1);
    printtab(level);

    printf("right\n");
    printtree(root->right, level+1);
    printtab(level);
    printf("done\n");
}


int main_tree(){
    btree_node *root = NULL;
    int x = 0;
    insert(&root, 4, 1, "double", "", 0);
    insert(&root, 4, 2, "else", "", 0);
    insert(&root, 4, 3, "if", "", 0);
    insert(&root, 4, 4, "ugabugag", "", 0);
    insert(&root, 4, 0, "lol", "", 0);
    printtree(root, x);
    
    return 0;
}