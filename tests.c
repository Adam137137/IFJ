/*
 * Projekt: Implementace překladače imperativního jazyka IFJ23
 *
 * autori suboru: 
 * xvarsa01 - Adam Varsanyi
 * 247112 - Peter Gvozdjak
 * 
*/

#include "compiler.h"
#include "tests.h"

void tests_init(int test_case){
    file = fopen("test.txt", "w");

    printf("test case %d:\n", test_case+1);

    switch (test_case)
    {
    case 0:
        test_letnutie();
        break;
    case 1:
        test_ifnutie();
        break;
    case 2:
        test_whilnutie();
        break;
    default:
        break;
    }
    fclose(file);
}


void test_letnutie(){
    char string []= "let id : Double\n";
    fprintf(file, "%s", string);
    printf("%s\n",string);
}
void test_ifnutie(){
    char string []= "if let id : Double {\n let id : Int\n } else {\n let id : String}\n";
    fprintf(file, "%s",string);
    printf("%s\n",string);
}
void test_whilnutie(){
    char string []= "while let id : Double {\n let id : Int}\n";
    fprintf(file, "%s", string);
    printf("%s\n",string);
}
void toto_preslo(){
//     if  2+1 != 3{
//     if 3+4 <6 {
//         var id : Int
//     }
//     else{
//         x = 5+1*(6-4)
//     }
//     let id = foo (with:2/3)
// }
// else{

// }
    char string []= "dopis si to tam \n";
    fprintf(file, "%s", string);
    printf("%s\n",string);
}
void test_scanner(){

    test_letnutie();
    test_ifnutie();
    test_whilnutie();
}


