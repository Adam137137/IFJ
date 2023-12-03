#include "dll_symtable.h"

void DLL_Init2( DLList2 *list ) {
	list->firstElement = NULL;
}

void DLL_InsertFirst2( DLList2 *list) {
	DLLElementPtr2 tmp = list->firstElement;
	list->firstElement = malloc(sizeof(struct DLLElement2));
	if(list->firstElement == NULL){
		handle_error(INTERNAL_ERROR);
	}
    list->firstElement->treeRoot = NULL;                    //inicializacia pointru na strom NULL
	list->firstElement->nextElement = tmp;
	list->firstElement->previousElement = NULL;
	if(tmp != NULL){	//Set ex-first element's pointer to new element
		tmp->previousElement = list->firstElement;
	}
}

void DLL_DeleteFirst2( DLList2 *list ) {
	if(list->firstElement == NULL){
		return;
	}
	DLLElementPtr2 tmp = list->firstElement;
	list->firstElement = list->firstElement->nextElement;
	if(list->firstElement != NULL){
		list->firstElement->previousElement = NULL;
	}
	free(tmp);
}
btree_node* find_function_in_global(DLList2 *list, char *name_of_func){
	DLLElementPtr2 tmp = list->firstElement;
	btree_node *search_node;
	while (tmp != NULL){                 // dokym nenarazime na funkciu
		if (tmp->nextElement == NULL){			//sme v globalnom ramci
			search_node = search(tmp->treeRoot,name_of_func);
			return search_node;
		}
        tmp = tmp->nextElement;
    }
    return NULL; 

}

btree_node* find_declaration(DLList2 *list, char *name_of_id){      //vrati node s name_of_id
    DLLElementPtr2 tmp;
    btree_node *search_node;               //hladana noda s name_of_id
    tmp = list->firstElement;
    while (tmp != NULL){                 // dokym nenarazime na funkciu
		if (tmp->nextElement == NULL){			//sme v globalnom ramci
			
			
		}
        search_node =  search(tmp->treeRoot, name_of_id);
        if (search_node == NULL){
            tmp = tmp->nextElement;
        }
        else{
            // printf("Nasli sme premennu v tabulke - kontrola typu atd.\n");
            return search_node;
        }
    }
    return NULL;                        //ked to nie je ani v globalnom ramci vrati NULL             
}
void DLL_PrintList2(DLList2 *list){
	DLLElementPtr2 currentElement = list->firstElement;
	if (currentElement == NULL)
	{
		printf(" none (list is empty)");
	}

	while (currentElement != NULL)
	{
        printf("%s    ", currentElement->treeRoot->name_of_symbol);
		currentElement = currentElement->nextElement;
	}
	printf("\n");
}
