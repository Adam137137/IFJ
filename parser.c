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
struct Token current_token;
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
        return true;
        //return parametre_volania(file)
    }
    else if (current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7){
        printf("expression will be reduced\n");
        printf("%s\n", current_token.attribute);
        unget_token(current_token2, file);
        return true;
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

bool varnutie(FILE* file){
    current_token = getNextToken(file);
    if (current_token.type == 1){
        return typ_s_dvojbodkou(file) && priradenie(file);
    }
    else{
        return false;
    }
}

bool whilnutie(FILE* file){
    bool res;
    current_token = getNextToken(file);
    if (current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7){
        printf("expression will be reduced\n");
        //dame na precedencnu analyzu nech zjednoduchsi vyraz alebo podmienku
        current_token = getNextToken(file);
        //current_token2 = getNextToken(file);
        if (current_token.type == 9 && strcmp(current_token.attribute, "{") == 0){
            current_token = getNextToken(file);
            res = sekvencia(file);
        }
        current_token = getNextToken(file);
        return res && current_token.type == 9 && strcmp(current_token.attribute, "}") == 0;
    }
    return false;

    
}
bool sekvencia(FILE* file){
    current_token = getNextToken(file);
    printf("%s\n", current_token.attribute);
    printf("type: %d\n", current_token.type);
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4){
        if (letnutie(file) == false){
            return false;
        }
        return sekvencia(file);
    }
    else if (strcmp(current_token.attribute, "var") == 0 && current_token.type == 4){
        if (varnutie(file) == false){
            return false;
        }
        sekvencia(file);
    }
    // else if (strcmp(current_token.attribute, "while") == 0 && current_token.type == 4){
    //     if (whilnutie(file) == false){
    //         printf("somt");
    //         return false;
    //     }
    //     sekvencia(file);
    // }
    // else{
    //     return false;           //not sure
    // }
    //TODO dalsie mozne neterminaly zo sekvencie
}

void parser(FILE* file){
    //current_token = getNextToken(file);
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