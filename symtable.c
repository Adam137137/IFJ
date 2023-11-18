#include "symtable.h"

char *string_dup(char *string){
    if(string == NULL){
        return NULL;
    }

    char *copied_string = malloc((strlen(string) + 1 )*sizeof(char));
    if(copied_string == NULL){
        return NULL;
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

btree_node *create_node(int token_type, int key, char *name_of_symbol, char *func_param, int func_num_of_param){
    btree_node *node = (btree_node *)malloc(sizeof(btree_node));
    if(node == NULL){                           // check for allocation
        return NULL;                            // podla zadania sa vracia 99, ale to je dobra chujovina potom
    }
    node->token_type = token_type;
    node->key = key;
    node->name_of_symbol = string_dup(name_of_symbol);                // duplicating the string given, allocating memmory for it and adding it to tree
    if(node->name_of_symbol == NULL){
        free(node);
        return NULL;                                // podla zadania sa vracia 99, ale to je dobra chujovina potom
    }

    node->func_param = string_dup(func_param);
    if(node->func_param == NULL){
        free(node);
        free(node->name_of_symbol);
        return NULL;
    }

    node->func_num_of_param = func_num_of_param;

    // node->height = 0;                               // aj bez tohto to tam da implicitne 0, ale v debuggeri to nedalo nic
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
        (*root)->height = height_of_node(*root);
        int factor = balance_factor(*root);
        if(factor == 2 && key < (*root)->left->key){                             // LL case
            *root = rotate_right(*root);
        }

        if(factor == -2 && key > (*root)->right->key){                          // RR case
            *root = rotate_left(*root);
        }

        if(factor == 2 && key > (*root)->left->key){                             // LR case
            (*root)->left = rotate_left((*root)->left);
            *root = rotate_right(*root);
        }

        if(factor == -2 && key < (*root)->right->key){                          // RL case
            (*root)->right = rotate_right((*root)->right);
            *root = rotate_left(*root);
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
        *name_of_symbol = string_dup(root->name_of_symbol);                 // duplicating the string given, allocating memmory for it, will be returned in param
        if(*name_of_symbol == NULL){
            return false;
        }
        
        if(strlen(root->func_param) > 0){                   // check whether we have an empty string = a function has parameters
            *func_param = string_dup(root->func_param);
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

void tree_dispose(btree_node **root) {
    if((*root) != NULL){
        tree_dispose(&((*root)->left));
        tree_dispose(&((*root)->right));
        
        if((*root)->name_of_symbol != NULL){
            free((*root)->name_of_symbol);
        }
        if((*root)->func_param != NULL){                        // checking if it is not an empty string
            free((*root)->func_param);
        }
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
    printf("root key : %d\n", root->key);
    printf("root name_of_symbol : %s\n", root->name_of_symbol);
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