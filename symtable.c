/*
 * Projekt: Implementace překladače imperativního jazyka IFJ23
 *
 * autori suboru: 
 * xperec00 - Mario Perecz
 * 
*/


#include "symtable.h"
#include "compiler.h"

char *string_dup(char *string){
    if(string == NULL){
        handle_error(INTERNAL_ERROR);
    }

    char *copied_string = malloc((strlen(string) + 1 )*sizeof(char));
    if(copied_string == NULL){
        handle_error(INTERNAL_ERROR);
    }
    strcpy(copied_string, string);
    return copied_string;
}

int maximum(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

// negative number => we will go to right subtree, positive number => we will go to left subtree
int lexicographic_compare(char *node_identifier, char *new_identifier){
    int i = 0;
    int result;
    while(new_identifier[i] != '\0'){
        result = node_identifier[i] - new_identifier[i]; 
        if(result == 0){
            i++;
        }
        else{
            return result;
        }
        
    }
    if(result == 0){                                            // case when we reach null terminator in new_identifier and they look identical
        if(strlen(node_identifier) > strlen(new_identifier)){  // we have to determine whether they are the same word or new_identifier is a prefix to node_identifier
            return 2;                                          // just return any positive number
        }
        else{
            return 0;                 // identical names of variables
        }
    }
    return result;    
}

int height_of_node(btree_node *root){
    if(root == NULL){
        return -1;
    }
    else{
        int count_left = height_of_node(root->left);
        int count_right = height_of_node(root->right);
        return 1 + maximum(count_left, count_right);
    }
}

int balance_factor(btree_node * root){
    if(root == NULL){
        return 0;
    }
    return height_of_node(root->left) - height_of_node(root->right);
}

btree_node *rotate_right(btree_node *root){
    btree_node *new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    root->height = height_of_node(root);
    new_root->height = height_of_node(new_root);
    return new_root;
}

btree_node *rotate_left(btree_node *root){
    btree_node *new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;
    root->height = height_of_node(root);
    new_root->height = height_of_node(new_root);
    return new_root; 
}

void init(btree_node **root) {
    *root = NULL;
}

btree_node *create_node(char *name_of_symbol, int token_type, bool inicialized, char data_type, bool nil, bool let, int value_int, char *value_string, double value_double){
    btree_node *node = (btree_node *)malloc(sizeof(btree_node));
    if(node == NULL){                           // check for allocation
        handle_error(INTERNAL_ERROR);                            
    }
    
    node->name_of_symbol = string_dup(name_of_symbol);                // duplicating the string given, allocating memmory for it and adding it to tree
    if(node->name_of_symbol == NULL){
        free(node);
        handle_error(INTERNAL_ERROR);                                
    }

    node->token_type = token_type;
    node->inicialized = inicialized;

    node->data_type = data_type;
    node->nil = nil;
    node->let = let;
    node->value_int = value_int;

    node->value_string = string_dup(value_string);
    if(node->value_string == NULL){
        free(node);
        if(node->name_of_symbol != NULL){
            free(node->name_of_symbol);
        }    
        handle_error(INTERNAL_ERROR);
    }

    node->value_double = value_double;

    for (int i = 0; i < 10; i++) {
        node->paramsArray[i].name = NULL;
        node->paramsArray[i].identif = NULL;
        node->paramsArray[i].type = '\0';
    }
    node->func_num_of_param = 0;

    node->return_type = '\0';

    node->left = NULL;
    node->right = NULL;
    return node;
}

void insert_data_type(btree_node **root, char *name_of_funcion, char data_type, bool nil){
    if(*root == NULL){                          // should not be NULL only updating node
       handle_error(INTERNAL_ERROR);
    }
    else{
        btree_node *temp = search(*root, name_of_funcion);
        temp->data_type = data_type;
        temp->nil = nil;
    }
}

void insert_return_typ(btree_node **root, char *name_of_funcion, char return_type){
    if(*root == NULL){
       handle_error(INTERNAL_ERROR);                    // should not be NULL only updating node
    }
    else{
        btree_node *temp = search(*root, name_of_funcion);
        temp->return_type = return_type;
    }
}

void insert_params(btree_node **root, char *name_of_funcion, int which_attribute, char *atribute){
    if(*root == NULL){
       handle_error(INTERNAL_ERROR);                    // should not be NULL only updating node
    }
    else{
        btree_node *temp = search(*root, name_of_funcion);
        int i = 0;
        while (temp->paramsArray[i].type != '\0')       // finding closest empty space to store params
        {
            i++;
        }
        temp->func_num_of_param = i+1;
        if (which_attribute == 1)                       // name
        {
            temp->paramsArray[i].name = atribute;
        }
        else if (which_attribute == 2)                  // identifier
        {
            temp->paramsArray[i].identif = atribute;
        }
        else{                                           // type
            temp->paramsArray[i].type = atribute[0];
        }
    }
}

void insert_variable(btree_node **root, char *name_of_symbol, int token_type, bool let){
    if(*root == NULL){
        *root = create_node(name_of_symbol, token_type, false, '\0', false, let, 0, "", 0);
    }
    else{
        if(lexicographic_compare((*root)->name_of_symbol, name_of_symbol) < 0){
            insert_variable(&((*root)->right), name_of_symbol, token_type, let);    
        }
        else if(lexicographic_compare((*root)->name_of_symbol, name_of_symbol) > 0){
            insert_variable(&((*root)->left), name_of_symbol, token_type, let);
        }
        else{
            handle_error(SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE);
        }
        (*root)->height = height_of_node(*root);
        int factor = balance_factor(*root);
        if(factor == 2 && lexicographic_compare((*root)->left->name_of_symbol, name_of_symbol) > 0){      // LL case
            *root = rotate_right(*root);
        }

        if(factor == -2 && lexicographic_compare((*root)->right->name_of_symbol, name_of_symbol) < 0){    // RR case
            *root = rotate_left(*root);
        }

        if(factor == 2 && lexicographic_compare((*root)->left->name_of_symbol, name_of_symbol) < 0){      // LR case
            (*root)->left = rotate_left((*root)->left);
            *root = rotate_right(*root);
        }

        if(factor == -2 && lexicographic_compare((*root)->right->name_of_symbol, name_of_symbol) > 0){    // RL case
            (*root)->right = rotate_right((*root)->right);
            *root = rotate_left(*root);
        }
    }
}

void insert_func(btree_node **root, char *name_of_symbol, int token_type){
    if(*root == NULL){
        *root = create_node(name_of_symbol, token_type, false, false, '\0', false, 0, "", 0);
    }
    else{
        if(lexicographic_compare((*root)->name_of_symbol, name_of_symbol) < 0){
            insert_func(&((*root)->right), name_of_symbol, token_type);    
        }
        else if(lexicographic_compare((*root)->name_of_symbol, name_of_symbol) > 0){
            insert_func(&((*root)->left), name_of_symbol, token_type);
        }
        else{
            handle_error(SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE);
        }
        (*root)->height = height_of_node(*root);
        int factor = balance_factor(*root);
        if(factor == 2 && lexicographic_compare((*root)->left->name_of_symbol, name_of_symbol) > 0){      // LL case
            *root = rotate_right(*root);
        }

        if(factor == -2 && lexicographic_compare((*root)->right->name_of_symbol, name_of_symbol) < 0){    // RR case
            *root = rotate_left(*root);
        }

        if(factor == 2 && lexicographic_compare((*root)->left->name_of_symbol, name_of_symbol) < 0){      // LR case
            (*root)->left = rotate_left((*root)->left);
            *root = rotate_right(*root);
        }

        if(factor == -2 && lexicographic_compare((*root)->right->name_of_symbol, name_of_symbol) > 0){    // RL case
            (*root)->right = rotate_right((*root)->right);
            *root = rotate_left(*root);
        }
    }
}

// lexicographicly comparing identifiers and returning pointer to node it has found it, else returns NULL
btree_node *search(btree_node *root, char *name_of_symbol){
    if (root == NULL){                          // case, when identifier not found
        return NULL;
    }

    if (strcmp(root->name_of_symbol, name_of_symbol) == 0){                 // identifier found
        return root;
    }
    else if (strcmp(root->name_of_symbol, name_of_symbol) != 0){          // recurs 
        if(lexicographic_compare(root->name_of_symbol, name_of_symbol) > 0){
            return search(root->left, name_of_symbol);
        }
        else if (lexicographic_compare(root->name_of_symbol, name_of_symbol) < 0){
            return search(root->right, name_of_symbol);
        }
    }
    return NULL;  
}

void tree_dispose(btree_node **root) {
    if((*root) != NULL){
        tree_dispose(&((*root)->left));
        tree_dispose(&((*root)->right));
        
        if((*root)->name_of_symbol != NULL){
            free((*root)->name_of_symbol);
        }                                               // checking if it is not an empty string
        free(*root);
        (*root) = NULL;
        return;
    }
}

void printtab(int numtabs){                     // helping prints to debugg
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
    printf("root name_of_symbol : %s\n", root->name_of_symbol);
    printf("params:           name: %s             identif: %s         typ: %c       return type: %c \n", root->paramsArray[0].name, root->paramsArray[0].identif, root->paramsArray[0].type, root->return_type);
    printf("height: %d\n", root->height);
    printtab(level);
    
    printf("left\n");
    printtree(root->left, level+1);
    printtab(level);

    printf("right\n");
    printtree(root->right, level+1);
    printtab(level);
    printf("done\n");
}