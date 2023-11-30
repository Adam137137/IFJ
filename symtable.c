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
            return -2;                                          // just return any negative number
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

btree_node *create_node(char *name_of_symbol, int token_type, bool inicialized, char *data_type, bool let, int value_int, char *value_string, double value_double, param_struct_t paramsArray[10], int func_num_of_param, char return_type){
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

    node->data_type = string_dup(data_type);
    if(node->data_type == NULL){
        free(node);
        free(node->name_of_symbol);
        handle_error(INTERNAL_ERROR);
    }
    
    node->let = let;
    node->value_int = value_int;

    node->value_string = string_dup(value_string);
    if(node->value_string == NULL){
        free(node);
        free(node->name_of_symbol);
        free(node->data_type);
        handle_error(INTERNAL_ERROR);
    }

    node->value_double = value_double;

    for (int i = 0; i < 10; i++) {
        node->paramsArray[i].name = NULL;
        node->paramsArray[i].identif = NULL;
        node->paramsArray[i].type = '\0';
    }
    node->func_num_of_param = func_num_of_param;

    node->return_type = return_type;

    // node->height = 0;                               // aj bez tohto to tam da implicitne 0, ale v debuggeri to nedalo nic
    node->left = NULL;
    node->right = NULL;
    return node;
}
void insert_return_typ(btree_node **root, char *name_of_funcion, char return_type){
    if(*root == NULL){
       handle_error(INTERNAL_ERROR);                    //nemala by byt NULL, lebo iba upadtujeme nodu dopisanim return typu
    }
    else{
        btree_node *temp = search(*root, name_of_funcion);
        temp->return_type = return_type;
    }
}
void insert_params(btree_node **root, char *name_of_funcion, int which_attribute, char *atribute){
    if(*root == NULL){
       handle_error(INTERNAL_ERROR);                    //nemala by byt NULL, lebo iba upadtujeme nodu dopisanim parametrov
    }
    else{
        btree_node *temp = search(*root, name_of_funcion);
        int i = 0;
        while (temp->paramsArray[i].type != '\0')       // snazi sa najst najblizsie volne miesto pre ulozenie parametrov
        {
            i++;
        }
        temp->func_num_of_param = i+1;
        if (which_attribute == 1)                       // meno
        {
            temp->paramsArray[i].name = atribute;
        }
        else if (which_attribute == 2)                  // identifik
        {
            temp->paramsArray[i].identif = atribute;
        }
        else{                                           // typ
            temp->paramsArray[i].type = atribute[0];
        }
    }
}
void insert(btree_node **root, char *name_of_symbol, int token_type, bool inicialized, char *data_type, bool let, int value_int, char *value_string, double value_double, param_struct_t paramsArray[10], int func_num_of_param, char return_type){
    if(*root == NULL){
        *root = create_node(name_of_symbol, token_type, inicialized, data_type, let, value_int, value_string, value_double, paramsArray, func_num_of_param, return_type);
    }
    else{
        if(lexicographic_compare((*root)->name_of_symbol, name_of_symbol) < 0){
            insert(&((*root)->right), name_of_symbol, token_type, inicialized, data_type, let, value_int, value_string, value_double, paramsArray, func_num_of_param, return_type);    
        }
        else if(lexicographic_compare((*root)->name_of_symbol, name_of_symbol) > 0){
            insert(&((*root)->left), name_of_symbol, token_type, inicialized, data_type, let, value_int, value_string, value_double, paramsArray, func_num_of_param, return_type);
        }
        else{
            //handle_error();  TO DO
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
        // puts("nenasli sme");
        return NULL;
    }

    if (strcmp(root->name_of_symbol, name_of_symbol) == 0){                 // identifier found
        // puts("nasli sme");
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

// skipped for now
/*
void replace_highest_in_left_tree(btree_node **root, btree_node *target){
    if((*root)->right == NULL){                                                         // case where, we do not have right child, this is the node, that will replace targer
        
        target->key = (*root)->key;
        target->func_num_of_param = (*root)->func_num_of_param;
        
        if(strlen(target->name_of_symbol) < strlen((*root)->name_of_symbol))                    // check if we have enough space for new string
        {
            target->name_of_symbol = realloc(target->name_of_symbol, strlen((*root)->name_of_symbol)+1);      // reallocating memory for new string, (+1 to include null terminator)
            if(target->name_of_symbol == NULL){                                 // check if the allocation is correct   
                return;             // treba pomenit error
            }
            strcpy(target->name_of_symbol,(*root)->name_of_symbol);                     // copy of the string
        }

        if(strlen(target->func_param) < strlen((*root)->func_param))                    // check if we have enough space for new string
        {
            target->func_param = realloc(target->func_param, strlen((*root)->func_param)+1);      
            if(target->func_param == NULL){                                   
                return;                 // treba pomenit error
            }
            strcpy(target->func_param,(*root)->func_param);                     
        }
        else{                                                                   // no need to reallocate, just copy
            strcpy(target->name_of_symbol,(*root)->name_of_symbol);
            strcpy(target->func_param,(*root)->func_param);
        }
        
        if((*root)->name_of_symbol != NULL){                            // checking if it is not an empty string
            free((*root)->name_of_symbol);                              
        }                                                               // freeing all the values stored
        if((*root)->func_param != NULL){                        
            free((*root)->func_param);
        }
        
        btree_node *tmp = (*root);                                      // store the root to be deleted
        *root = (*root)->left;                                          // updating pointer of parent to point to new child // pls help ://////
        free(tmp);
    }
    else{
        replace_highest_in_left_tree(&((*root)->right), target);
    }
}

void node_delete(btree_node **root, int token_type, int key){
    
    // ak zavolame nad prazdnym podstromom, not sure, co spravit
    
    if(key < (*root)->key){                                     // searching our key
        node_delete(&((*root)->left), token_type, key);
    }
    else if(key > (*root)->key){
        node_delete(&((*root)->right), token_type, key);
    }
    else{
        if((*root)->left == NULL && (*root)->right == NULL){        // node has no children
            
            if((*root)->name_of_symbol != NULL){
                free((*root)->name_of_symbol);
            }
            if((*root)->func_param != NULL){                        // checking if it is not an empty string
                free((*root)->func_param);
            }
            free(*root);
            *root = NULL;                                               // setting pointer to root to NULL, so the parent's pointer will point to NULL
        }
        else if((*root)->left != NULL && (*root)->right != NULL){           // node with 2 children
            replace_highest_in_left_tree(&((*root)->left), *root);
        }  
        else{                                                               // deleting a node with one child
            if((*root)->left == NULL){                                     // has only right child
                btree_node *tmp;                                            // pointer to store the child
                tmp = (*root)->right;

                if((*root)->name_of_symbol != NULL){
                    free((*root)->name_of_symbol);
                }
                if((*root)->func_param != NULL){                        // checking if it is not an empty string
                    free((*root)->func_param);
                }
                
                (*root)->right = NULL;                                  // right root pointer set to NULL before freeing the node
                free(*root);
                *root = tmp;                                            // after free the child can become the new root
            }
            else{                                                           // has only left child
                btree_node *tmp;                                            
                tmp = (*root)->left;

                if((*root)->name_of_symbol != NULL){
                    free((*root)->name_of_symbol);
                }
                if((*root)->func_param != NULL){                        // checking if it is not an empty string
                    free((*root)->func_param);
                }
                
                (*root)->left = NULL;                                   // left root pointer set to NULL before freeing the node
                free(*root);
                *root = tmp; 
            }
        }                                        
    }
}
*/
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