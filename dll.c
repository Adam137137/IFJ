#include "dll.h"
#include "compiler.h"

void DLL_Init( DLList *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

void DLL_Dispose( DLList *list ) {
	while(list->firstElement != NULL){	//Until the list is empty, delete elements one by one
		DLLElementPtr cache = list->firstElement->nextElement;
		free(list->firstElement);
		list->firstElement = cache;
	}
	list->activeElement = NULL;
	list->lastElement = NULL;
}

void DLL_InsertLast( DLList *list, char data ) {
	DLLElementPtr cache = malloc(sizeof(struct DLLElement));
	if(cache == NULL){
		handle_error(99);
	}
	cache->data = data;
	if(list->lastElement == NULL){	//If the list is empty
		cache->nextElement = NULL;
		cache->previousElement = NULL;
		list->firstElement = cache;
		list->lastElement = cache;
	}
	else{
		cache->previousElement = list->lastElement;
		list->lastElement->nextElement = cache;
		list->lastElement = cache;
		list->lastElement->nextElement = NULL;
	}
}

void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

void DLL_InsertAfter( DLList *list, char data ) {
	if(list->activeElement == NULL){	//Check if the list is active
		return;
	}
	DLLElementPtr cache = malloc(sizeof(struct DLLElement));
	if(cache == NULL){
		handle_error(99);
	}
	cache->data = data;
	cache->nextElement = list->activeElement->nextElement;
	cache->previousElement = list->activeElement;
	list->activeElement->nextElement = cache;
	//If active element is the last element in list, set inserted element as last element
	if(list->activeElement == list->lastElement){
		list->lastElement = cache;
	}
	else{
		cache->nextElement->previousElement = cache;
	}
}

char DLL_DeleteLast( DLList *list ) {
	if(list->lastElement == NULL){
		return 0;
	}
	if(list->activeElement == list->lastElement){
		list->activeElement = NULL;
	}
	DLLElementPtr cache = list->lastElement;
	list->lastElement = list->lastElement->previousElement;
	
	if(list->lastElement != NULL){
		list->lastElement->nextElement = NULL;
	}
	else{	//If last element was the only element in list, set first element to NULL aswell
		list->firstElement = NULL;
	}
	return cache->data;
	free(cache);
}

//Function to find the top most terminal on "stack" and return pointer to it
//fisrtIteration tells us if we have to set active element to last or not, when calling set to true
//Return NULL if there is no terminal, unlikely to happen (missing $)
DLLElementPtr DLL_TopTerminal(DLList *list, bool firstIteration){
	if(list->lastElement == NULL){
		return NULL;
	}
	if(firstIteration){
		DLL_Last(list);
	}
	//If element is terminal, return its pointer
	if((list->activeElement->data <= 47 && list->activeElement->data >= 36) || list->activeElement->data == 105){
		return list->activeElement;
	}
	else{
		list->activeElement = list->activeElement->previousElement;
		if(list->activeElement == NULL){
			return NULL;
		}
		return DLL_TopTerminal(list, false);
	}
}

void DLL_PrintList(DLList *list){
	DLLElementPtr currentElement = list->lastElement;
	if (currentElement == NULL)
	{
		printf(" none (list is empty)");
	}

	while (currentElement != NULL)
	{
		printf(" %c", currentElement->data);
		currentElement = currentElement->previousElement;
	}
	printf("\n");
}