#include "compiler.h"



int main(int argc, char *argv[]){
    (void) argc;
    (void) argv;
    FILE *file = fopen("test.txt", "r");
    char firstchar = fgetc(file);
    printf("%c", firstchar);
    parser(file);
    puts("");
    return 1;
    return 0;
}