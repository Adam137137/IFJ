#include "compiler.h"



int main(int argc, char *argv[]){
    (void) argc;
    (void) argv;
    FILE *file = fopen("test.txt", "r");
    parser(file);
    puts("");
    return 0;
}