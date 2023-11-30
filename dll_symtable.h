#ifndef DLL_SYMTABLE_H
#define DLL_SYMTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "compiler.h"

typedef struct DLLElement2 {
    btree_node *treeRoot;
    struct DLLElement2 *previousElement;
    struct DLLElement2 *nextElement;
} *DLLElementPtr2;

typedef struct {
    DLLElementPtr2 firstElement;
    DLLElementPtr2 activeElement;
    DLLElementPtr2 lastElement;
} DLList2;

void DLL_PrintList2(DLList2 *list);
void DLL_InsertFirst2( DLList2 *list);
void DLL_Init2( DLList2 *list );
void DLL_DeleteFirst2( DLList2 *list );
btree_node* find_declaration_of_variable(DLList2 *list, char *name_of_id);



#endif