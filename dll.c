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

void DLL_InsertFirst( DLList *list, int data ) {
	DLLElementPtr cache = list->firstElement;
	list->firstElement = malloc(sizeof(struct DLLElement));
	if(list->firstElement == NULL){
		handle_error(99);
	}
	list->firstElement->data = data;
	list->firstElement->nextElement = cache;
	list->firstElement->previousElement = NULL;
	if(cache != NULL){	//Set ex-first element's pointer to new element
		cache->previousElement = list->firstElement;
	}
	else{	//If inserted element is the sole element in list, set it to be the last element aswell
		list->lastElement = list->firstElement;
	}
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

void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
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

void DLL_InsertBefore( DLList *list, int data ) {
	if(list->activeElement == NULL){	//Check if the list is active
		return;
	}
	DLLElementPtr cache = malloc(sizeof(struct DLLElement));
	if(cache == NULL){
		handle_error(99);
	}
	cache->data = data;
	cache->previousElement = list->activeElement->previousElement;
	cache->nextElement = list->activeElement;
	list->activeElement->previousElement = cache;
	//If active element is the first element in list, set inserted element as first element
	if(list->activeElement == list->firstElement){
		list->firstElement = cache;
	}
	else{
		cache->previousElement->nextElement = cache;
	}
}

char DLL_DeleteFirst( DLList *list ) {
	if(list->firstElement == NULL){
		return 0;
	}
	if(list->activeElement == list->firstElement){
		list->activeElement = NULL;
	}
	DLLElementPtr cache = list->firstElement;
	list->firstElement = list->firstElement->nextElement;

	if(list->firstElement != NULL){
		list->firstElement->previousElement = NULL;		
	}
	else{	//If first element was the only element in list, set last element to NULL aswell
		list->lastElement = NULL;
	}
	return cache->data;
	free(cache);
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

char DLL_DeleteAfter( DLList *list ) {
	//Check if the list is active or active element is the last element in list
	if(list->activeElement == NULL || list->activeElement == list->lastElement){
		return;
	}
	DLLElementPtr cache = list->activeElement->nextElement;
	list->activeElement->nextElement = cache->nextElement;
	if(cache == list->lastElement){	//If element after active is the last element, set active to the last
		list->lastElement = list->activeElement;
	}
	else
	{
		cache->nextElement->previousElement = list->activeElement;
	}
	return cache->data;
	free(cache);
}

char DLL_DeleteBefore( DLList *list ) {
	//Check if the list is active or active element is the first element in list
	if(list->activeElement == NULL || list->activeElement == list->firstElement){
		return;
	}
	DLLElementPtr cache = list->activeElement->previousElement;
	list->activeElement->previousElement = cache->previousElement;
	if(cache == list->firstElement){	//If element before active is the first element, set active to the first
		list->firstElement = list->activeElement;
	}
	else{
		cache->previousElement->nextElement = list->activeElement;
	}
	return cache->data;
	free(cache);
}

char DLL_GetFirst( DLList *list, int *dataPtr ) {
	if(list->firstElement == NULL){
		handle_error(99);
	}
	return list->firstElement->data;
}

char DLL_GetLast( DLList *list){
	if(list->lastElement == NULL){
		handle_error(99);
	}
	return list->lastElement->data;
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