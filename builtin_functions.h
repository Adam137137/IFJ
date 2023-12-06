#ifndef BUILIN_FUNCTIONS_H
#define BUILIN_FUNCTIONS_H
#include "compiler.h"

void BUILTIN_Int2Double(int var_int, double var_double);
void BUILTIN_Double2Int(double var_double, int var_int);
void BUILTIN_lenght(char *string, int var_int);
void BUILTIN_ord(char *string, int var_int);
#endif