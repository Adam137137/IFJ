#ifndef COMPILER_H
#define COMPILER_H
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "symtable.h"
#include "scanner.h"
#include "parser.h"

// typedef enum {
//     NO_ERROR = 0,
//     LEXICAL_ERROR = 1,
//     SYNTAX_ERROR = 2,
//     SEMANTIC_UNDEFINED_FUNCTION = 3,
//     SEMANTIC_PARAMETER_MISMATCH = 4,
//     SEMANTIC_UNDEFINED_VARIABLE = 5,
//     SEMANTIC_RETURN_STATEMENT = 6,
//     SEMANTIC_TYPE_COMPATIBILITY = 7,
//     SEMANTIC_TYPE_INFERENCE = 8,
//     OTHER_SEMANTIC_ERROR = 9,
//     INTERNAL_ERROR = 99
// } ErrorCode;
// ErrorCode error;

extern FILE *file;
#endif
