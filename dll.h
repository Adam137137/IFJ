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

void DLL_init(DLList *);

void DLL_Dispose( DLList * );

void DLL_InsertLast( DLList *, char );

void DLL_Last( DLList * );

void DLL_InsertAfter( DLList *, char );

char DLL_GetValue( DLList *);

DLLElementPtr DLL_TopTerminal(DLList *, bool );