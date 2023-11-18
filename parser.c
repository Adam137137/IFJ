#include "parser.h"
#include "compiler.h"

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
struct Token current_token, current_token2;

void unget_token(struct Token token) {
    for (int i = strlen(token.attribute) - 1; i >= 0; i--) {
        ungetc((unsigned char)token.attribute[i], file);
    }
}

bool typ(){
    current_token = getNextToken();
    printf("%s\n", current_token.attribute);
    if (current_token.type == 4 && ( 
        strcmp(current_token.attribute, "Int") == 0 ||
        strcmp(current_token.attribute, "String") == 0 ||
        strcmp(current_token.attribute, "Double") == 0 ||
        strcmp(current_token.attribute, "Int?") == 0 ||
        strcmp(current_token.attribute, "String?") == 0 ||
        strcmp(current_token.attribute, "Double?") == 0))
        {
            return true;
        }
    return false;
}

bool dvojbodka_typ(){
    current_token = getNextToken();
    printf("%s\n", current_token.attribute);
    if (strcmp(current_token.attribute, ":") == 0 && current_token.type == 12){
        return typ();
    }
    stop = true;
    unget_token(current_token);
    return true;
}

bool priradenie_prave(){
    current_token = getNextToken();
    current_token2 = getNextToken();
    if (current_token.type == 1 && current_token2.type == 9 && strcmp(current_token2.attribute, "(") == 0){
        printf("funkcia priradenie\n");
        //return parametre_volania()
        return true;
    }
    else if (current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7){
        printf("expression will be reduced\n");
        printf("%s\n", current_token.attribute);
        unget_token(current_token2);
        return true;
        //precencna analzya(),druhy token treba vymysliet aby sa znova odnacital
        printf("%s\n", current_token.attribute);
        unget_token(current_token2);
        return true;
    }
    else{
        return false;
    }
}

bool rovna_sa__priradenie(){
    current_token = getNextToken();
    if (current_token.type == 10 && strcmp(current_token.attribute, "=") == 0){
        return priradenie_prave();
    }
    else{                        
        unget_token(current_token);
        if (stop == true){
            return false;           //ked bola vypustena deklaracia typu nemozeme vypustit priradenie
        }
        return true;
    }

}
bool letnutie(){
    current_token = getNextToken();
    if (current_token.type == 1){
        return dvojbodka_typ() && rovna_sa__priradenie();
    }
    else{
        return false;
    }
}


bool varnutie(){
    current_token = getNextToken(file);
    if (current_token.type == 1){
        return dvojbodka_typ(file) && rovna_sa__priradenie(file);
    }
    else{
        return false;
    }
}

bool whilnutie(){
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

bool sekvencia(){
    current_token = getNextToken();
    printf("%s\n", current_token.attribute);
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4){
        return letnutie();
    }
    else if (strcmp(current_token.attribute, "var") == 0 && current_token.type == 4){
        return varnutie();
    }
    else if (strcmp(current_token.attribute, "while") == 0 && current_token.type == 4){
        return whilnutie();
    }
    return false;
    //TODO dalsie mozne neterminaly zo sekvencie
}

void parser(){
    if (sekvencia() == true){
        printf("ok");
    }
    else{
        printf("Syntax Error");
    }
    // while (current_token.type != 0)
    // {
    //      printf("Type: %d     ", current_token.type);
    //      printf("Attribute: %s\n", current_token.attribute);
    //      current_token = getNextToken();
    // }
    
}