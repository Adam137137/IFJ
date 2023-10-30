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

// token_type to know which tree, searching through keys, and returning true or false whether it found it, other params are for returning the values
bool search(btree_node *root, int token_type, int key, char **name_of_symbol, char **func_param, int *func_num_of_param){
    if (root == NULL){                          // case, when the key is not there
        return false;
    }

    if (root->token_type != token_type){        // checking if we are in a good tree
        return false;
    }
    
    if (root->key == key){                             // found the key, returning valuse in variables
        *name_of_symbol = strdup(root->name_of_symbol);
        if(*name_of_symbol == NULL){
            return false;
        }
        
        if(strlen(root->func_param) > 0){                   // check whether we have an empty string = a function has parameters
            *func_param = strdup(root->func_param);
            if(*func_param == NULL){
                free(*name_of_symbol);
                return false;
            }
        }
        else{
            *func_param = NULL;
        }

        if(func_num_of_param != 0){                                 // toto mozeme teoreticky spojit s tym hore
            *func_num_of_param = root->func_num_of_param;
        }
        return true;
    }
    else if (root->key != key){                                 // recurs
        if(key < root->key){
            return search(root->left, token_type, key, name_of_symbol, func_param, func_num_of_param);
        }
        else if (key > root->key){
            return search(root->right, token_type, key, name_of_symbol, func_param, func_num_of_param);
        }
        
    }
    return false;     // idk ci to tu musi byt
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


/*int main(){
    btree_node *root = NULL;
    int x = 0;
    char *found_name_of_symbol = NULL;
    char *func_params = NULL;
 
    insert(&root, 4, 1, "double", "", 0);
    insert(&root, 4, 2, "else", "", 0);
    insert(&root, 4, 3, "if", "", 0);
    insert(&root, 4, 4, "ugabugag", "", 0);
    insert(&root, 4, 5, "lol", "", 0);
    printtree(root, x);
    
    bool result = search(root, 4, 3, &found_name_of_symbol, &func_params, 0);
    if (result) {
        printf("Found: %s\n", found_name_of_symbol);
        free(found_name_of_symbol);    
    } else {
        printf("Not found.\n");
    }

    return 0;
}*/