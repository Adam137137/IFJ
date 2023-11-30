#include "dll_symtable.h"

void DLL_Init2( DLList2 *list ) {
	list->activeElement = NULL;
	list->firstElement = NULL;
	list->lastElement = NULL;
}

void DLL_InsertFirst2( DLList2 *list) {
	DLLElementPtr2 cache = list->firstElement;
	list->firstElement = malloc(sizeof(struct DLLElement2));
	if(list->firstElement == NULL){
		handle_error(INTERNAL_ERROR);
	}
    list->firstElement->treeRoot = NULL;                    //inicializacia pointru na strom NULL
	list->firstElement->nextElement = cache;
	list->firstElement->previousElement = NULL;
	if(cache != NULL){	//Set ex-first element's pointer to new element
		cache->previousElement = list->firstElement;
	}
	else{	//If inserted element is the sole element in list, set it to be the last element aswell
		list->lastElement = list->firstElement;
	}
}

void DLL_DeleteFirst2( DLList2 *list ) {
	if(list->firstElement == NULL){
		return;
	}
	if(list->activeElement == list->firstElement){
		list->activeElement = NULL;
	}
	DLLElementPtr2 cache = list->firstElement;
	list->firstElement = list->firstElement->nextElement;

	if(list->firstElement != NULL){
		list->firstElement->previousElement = NULL;		
	}
	else{	//If first element was the only element in list, set last element to NULL aswell
		list->lastElement = NULL;
	}
}
btree_node* find_declaration_of_variable(DLList2 *list, char *name_of_id){      //vrati node s name_of_id
    DLLElementPtr2 tmp;
    btree_node *search_node;               //hladana noda s name_of_id
    tmp = list->firstElement;
    while (tmp != NULL || tmp->treeRoot->token_type != 4){                 // dokym nenarazime na funkciu
        search_node =  search(tmp->treeRoot, name_of_id);
        if (search_node == NULL){
            tmp = tmp->nextElement;
        }
        else{
            printf("Nasli sme premennu v tabulke - kontrola typu atd.");
            return search_node;
        }
    }
    search_node =  search(tree_main, name_of_id);               //posledna sanca ze premenna je globalna v globalnom ramci
    if (search_node != NULL){
        return search_node;
    }
    else{
        return NULL;                        //ked to nie je ani v globalnom ramci vrati NULL
    }             
}
void DLL_PrintList2(DLList2 *list){
	DLLElementPtr2 currentElement = list->lastElement;
	if (currentElement == NULL)
	{
		printf(" none (list is empty)");
	}

	while (currentElement != NULL)
	{
        printf("%s", currentElement->treeRoot->name_of_symbol);
		currentElement = currentElement->previousElement;
	}
	printf("\n");
}
