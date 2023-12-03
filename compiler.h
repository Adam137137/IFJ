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
#include "precedence.h"
#include "dll.h"
#include "tests.h"
#include "dll_symtable.h"
#include "code_generator.h"

typedef enum {
    NO_ERROR = 0,
    LEXICAL_ERROR = 1,
    SYNTAX_ERROR = 2,
    SEMANTIC_UNDEFINED_FUNCTION_OR_REDEFINED_VARIABLE = 3,
    SEMANTIC_PARAMETER_MISMATCH = 4,
    SEMANTIC_UNDEFINED_OR_UNINITIALIZED_VARIABLE = 5,
    SEMANTIC_RETURN_STATEMENT = 6,
    SEMANTIC_TYPE_COMPATIBILITY = 7,
    SEMANTIC_TYPE_INFERENCE = 8,
    OTHER_SEMANTIC_ERROR = 9,
    INTERNAL_ERROR = 99
} ErrorCode;

typedef struct {
    char *data;        
    int size;     
    int capacity; 
} dynamic_buffer;

void handle_error(int error);
void initDynamicArray(dynamic_buffer *buffer);
char *unique_name(char *string, int number);

extern FILE *file;
extern bool prvy_prechod;
extern dynamic_buffer buffer1;  // main buffer for code
extern dynamic_buffer buffer2;  // other buffer 
extern int frame_counter;
extern char* name_of_function;
#endif
