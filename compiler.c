/*
 * Projekt: Implementace překladače imperativního jazyka IFJ23
 *
 * autori suboru: 
 * xperec00 - Mario Perecz
 * 
*/

#include "compiler.h"

// NO_ERROR = 0,
// LEXICAL_ERROR = 1,
// SYNTAX_ERROR = 2,
// SEMANTIC_UNDEFINED_FUNCTION = 3,
// SEMANTIC_PARAMETER_MISMATCH = 4,
// SEMANTIC_UNDEFINED_VARIABLE = 5,
// SEMANTIC_RETURN_STATEMENT = 6,
// SEMANTIC_TYPE_COMPATIBILITY = 7,
// SEMANTIC_TYPE_INFERENCE = 8,
// OTHER_SEMANTIC_ERROR = 9,
// INTERNAL_ERROR = 99

void handle_error(int error){
    switch (error)
    {
    // case NO_ERROR:
    //     fprintf(stdout, "no problem\n");
    //     break;
    case LEXICAL_ERROR:
        fprintf(stderr,"lexical error\n");
        exit(LEXICAL_ERROR);
        break;
    case SYNTAX_ERROR:
        fprintf(stderr,"syntax error\n");
        exit(SYNTAX_ERROR);
        break;
    case SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE:
        fprintf(stderr,"semantic error: undefined function or redefined variable\n");
        exit(SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE);
        break;
    case SEMANTIC_PARAMETER_MISMATCH:
        fprintf(stderr,"semantic error: parameter mismatch\n");
        exit(SEMANTIC_PARAMETER_MISMATCH);
        break;
    case SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE:
        fprintf(stderr,"semantic error: undefined variable or uninitialized variable\n");
        exit(SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE);
        break;
    case SEMANTIC_RETURN_STATEMENT:
        fprintf(stderr,"semantic error: function not correctly returned\n");
        exit(SEMANTIC_RETURN_STATEMENT);
        break;
    case SEMANTIC_TYPE_COMPATIBILITY:
        fprintf(stderr,"semantic error: incompatible types\n");
        exit(SEMANTIC_TYPE_COMPATIBILITY);
        break;
    case SEMANTIC_TYPE_INFERENCE:
        fprintf(stderr,"semantic error: semantic type inference error\n");
        exit(SEMANTIC_TYPE_INFERENCE);
        break;
    case OTHER_SEMANTIC_ERROR:
        fprintf(stderr,"semantic error\n");
        exit(OTHER_SEMANTIC_ERROR);
        break;
    case INTERNAL_ERROR:
        fprintf(stderr,"internal error of compiler\n");
        exit(INTERNAL_ERROR);
        break;
    default:
        fprintf(stdout, "no problem\n");
        break;
    }
}   