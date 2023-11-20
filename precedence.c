#include "precedence.h"
#include "compiler.h"
#include "dll.h"

//      +	-	*	/	(	)	i	$
// +	>	>	<	<	<	>	<	>
// -	>	>	<	<	<	>	<	>
// *	>	>	>	>	<	>	<	>
// /	>	>	>	>	<	>	<	>
// (	<	<	<	<	<	=	<	
// )	>	>	>	>		>		>
// i	>	>	>	>		>		>
// $	<	<	<	<	<		<	


bool reduce_exp(){
    // uz je nacitany token vyrazu -> pojde do while
    DLList list;
    DLL_init(&list);
    DLL_InsertLast(&list, '$');
    while (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7)    // stale sme vo vyraze
    {
        // TODO: nacitat dalsie tokeny a zistit ci ide o validny vyraz
        
        // ak sa neda urobit vyraz return false
    }
    
    // nacitany token uz nie je vyraz ale nieco ine, treba ho asi vratit
    DLL_Dispose(&list);
    return true;
}