#ifndef DLL_H
#define DLL_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct DLLElement {
    char data;
    struct DLLElement *previousElement;
    struct DLLElement *nextElement;
} *DLLElementPtr;

typedef struct {
    DLLElementPtr firstElement;
    DLLElementPtr activeElement;
    DLLElementPtr lastElement;
} DLList;

void DLL_Init( DLList *list );

void DLL_Dispose( DLList *list );

void DLL_InsertLast( DLList *list, char data );

void DLL_Last( DLList *list );

void DLL_InsertAfter( DLList *list, char data );

char DLL_DeleteLast( DLList *list);

void DLL_PrintList(DLList *list);

DLLElementPtr DLL_TopTerminal(DLList *list, bool firstIteration);
#endif