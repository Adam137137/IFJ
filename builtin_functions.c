#include "builtin_functions.h"

void BUILTIN_Int2Double(int var_int, double var_double){
    sprintf(buffer1.data, "%sInt2Double:\n", buffer1.data);
    sprintf(buffer1.data, "%s   PUSHFRAME\n", buffer1.data);
    sprintf(buffer1.data, "%s   INT2FLOAT %s %s\n", buffer1.data, var_double, var_int);

    sprintf(buffer1.data, "%s   RETURN\n", buffer1.data);
}

void BUILTIN_Double2Int(double var_double, int var_int){
    sprintf(buffer1.data, "%sDouble2Int:\n", buffer1.data);
    sprintf(buffer1.data, "%s   PUSHFRAME\n", buffer1.data);
    sprintf(buffer1.data, "%s   FLOAT2INT %s %s\n", buffer1.data, var_int, var_double);

    sprintf(buffer1.data, "%s   RETURN\n", buffer1.data);
}

void BUILTIN_lenght(char *string, int var_int){
    sprintf(buffer1.data, "%slenght:\n", buffer1.data);
    sprintf(buffer1.data, "%s   PUSHFRAME\n", buffer1.data);
    sprintf(buffer1.data, "%s   STRLEN %s %s\n", buffer1.data, var_int, string);

    sprintf(buffer1.data, "%s   RETURN\n", buffer1.data);
}

void BUILTIN_ord(char *string, int var_int){
    sprintf(buffer1.data, "%sord:\n", buffer1.data);
    sprintf(buffer1.data, "%s   PUSHFRAME\n", buffer1.data);
    sprintf(buffer1.data, "%s   STR2INT %d %s 0\n", buffer1.data, var_int, string);

    sprintf(buffer1.data, "%s   RETURN\n", buffer1.data);
}