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
			fflush(stdout);
			search_node = search(tmp->treeRoot,name_of_func);
			return search_node;
		}
        tmp = tmp->nextElement;
    }
    return NULL; 

}
int check_params(btree_node* node_of_func, char* name_of_id){
    for(int i =0; i < node_of_func->func_num_of_param ;i++){			// prehladanie funkcnych parametrov na najdenie id
        if(strcmp(node_of_func->paramsArray[i].identif, name_of_id) == 0){
            return i;
        }
    }
	printf("ID nebolo najdene ani v parametroch funkcie\n");
    handle_error(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
	return 0;
}

btree_node* find_declaration(DLList2 *list, char *name_of_id, char* ret){      //vrati node s name_of_id
    DLLElementPtr2 tmp;
    btree_node *search_node;               //hladana noda s name_of_id
	btree_node *func_node;
	tmp = list->firstElement;
	anti_zanorenie = 0;
    while (tmp != NULL){                 // dokym nenarazime na funkciu
		search_node =  search(tmp->treeRoot, name_of_id);	// vyhladavanie v globalnom ramci premennej
        if (search_node != NULL){
			*ret = 'V';
            return search_node;
        }
		if (tmp->nextElement == NULL && name_of_function != NULL){				//sme v globalnom ramci nenasli sme globalnu premennu, tak hladame ci name_of_id je vo func parametroch
			func_node = search(tmp->treeRoot,name_of_function);
			if (func_node != NULL){			//toto by sa nemalo stat ze funcia nie je v strome ale pre istotu
				*ret = 'F';
				return func_node;
			}
		}
		anti_zanorenie++;
		//printf("A\n");
		tmp = tmp->nextElement;
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
