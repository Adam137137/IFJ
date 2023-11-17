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
bool stop = false;

void unget_token(struct Token token, FILE* file) {
    for (int i = strlen(token.attribute) - 1; i >= 0; i--) {
        ungetc((unsigned char)token.attribute[i], file);
    }
}

bool typ(FILE* file){
    current_token = getNextToken(file);
    if (current_token.type == 4 && strcmp(current_token.attribute, "Int") == 0 ||
        current_token.type == 4 && strcmp(current_token.attribute, "String") == 0 ||
        current_token.type == 4 && strcmp(current_token.attribute, "Double") == 0)
        {
            return true;
        }
    return false;
}

bool typ_s_dvojbodkou(FILE* file){
    current_token = getNextToken(file);
    if (strcmp(current_token.attribute, ":") == 0 && current_token.type == 12){
        return typ(file);
    }
    stop = true;
    unget_token(current_token, file);
    return true;
}

bool priradenie_prave(FILE* file){
    current_token = getNextToken(file);
    struct Token current_token2 = getNextToken(file);
    if (current_token.type == 1 && current_token2.type == 9 && strcmp(current_token2.attribute, "(") == 0){
        printf("funkcia priradenie\n");
        //return parametre_volania(file)
    }
    else if (current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7){
        printf("expression will be reduced\n");
        //precencna analzya(),druhy token treba vymysliet aby sa znova odnacital
    }
    else{
        return false;
    }
}

bool priradenie(FILE* file){
    current_token = getNextToken(file);
    if (current_token.type == 10 && strcmp(current_token.attribute, "=") == 0){
        return priradenie_prave(file);
    }
    else{                        
        unget_token(current_token, file);
        if (stop == true){
            return false;           //ked bola vypustena deklaracia typu nemozeme vypustit priradenie
        }
        return true;
    }

}
bool letnutie(FILE* file){
    current_token = getNextToken(file);
    if (current_token.type == 1){
        return typ_s_dvojbodkou(file) && priradenie(file);
    }
    else{
        return false;
    }
}
bool sekvencia(FILE* file){
    current_token = getNextToken(file);

    //printf("s%s\n", current_token.attribute);
    //printf("Token 2 type: %d\n", current_token.type);
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4){
        return letnutie(file);
    }
    else if (current_token.attribute == "var" && current_token.type == 4){
        //varnutie();
    }
    else if (current_token.attribute == "while" && current_token.type == 4){
        //whilnutie();
    }
    //TODO dalsie mozne neterminaly zo sekvencie
}

struct Token current_token;
void parser(FILE* file){
    if (sekvencia(file) == true){
        printf("ok");
    }
    else{
        printf("Syntax Error");
    }
    // while (current_token.type != 0)
    // {
    //      printf("Type: %d     ", current_token.type);
    //      printf("Attribute: %s\n", current_token.attribute);
    //      current_token = getNextToken(file);
    // }
    
}