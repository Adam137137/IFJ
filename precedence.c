#include "precedence.h"
#include "compiler.h"

bool reduce_exp(){
    // uz je nacitany token vyrazu -> pojde do while
    
    while (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7)    // stale sme vo vyraze
    {
        // TODO: nacitat dalsie tokeny a zistit ci ide o validny vyraz
        
        // ak sa neda urobit vyraz return false
    }
    
    // nacitany token uz nie je vyraz ale nieco ine, treba ho asi vratit
    return true;
}