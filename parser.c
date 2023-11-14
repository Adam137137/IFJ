#include "parser.h"

// bool parametre(){
//     return paramete() && ciarka() && parametre();
// }
// bool parameter(){
//     if (current_token.type == "id")
//     {
//         /* code */
//     }
    

// }


struct Token current_token;
void parser(FILE* file){
    current_token = getNextToken(file);
    while (current_token.type != 0)
    {
        printf("Type: %d     ", current_token.type);
        printf("Attribute: %s\n", current_token.attribute);
        current_token = getNextToken(file);
    }
    
}