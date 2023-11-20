#include "parser.h"
#include "compiler.h"

bool stop = false;
struct Token current_token, current_token2;

bool ladenie = 0;
void token_print(){
    if (ladenie)
    {
        printf("%s\n", current_token.attribute);
    }
}

void unget_token(struct Token token){
    if (token.type == 0){
        ungetc(EOF, file);
    }
    else{
        for (int i = strlen(token.attribute) - 1; i >= 0; i--) {
            ungetc((unsigned char)token.attribute[i], file);
        }
    }
}

bool func_declar(){
    current_token = getNextToken();
    if (current_token.type != 1){               // id
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 20){              // (
        return false;
    }
    if (parametre() == false){                  // parametre
            return false;
    }
    current_token = getNextToken();
    if (current_token.type != 21){              // )
            return false;
    }
    if (sipka_typ() == false){                  // ->typ
            return false;
    }
    current_token = getNextToken();
    if (current_token.type != 22){              // {
        return false;        
    }
    current_token = getNextToken();
    if (sekvencia() == false){                  // sekvencia
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){              // }
        return false;
    }
    return true;
}

bool parametre(){
    current_token = getNextToken();
    token_print();
    if (current_token.type == 1 ||current_token.type == 15)
    {
        return parameter() && param_zostatok();
    }
    else if (current_token.type == 21)               // )
    {
        unget_token(current_token);
        return true;
    }
    return false;
}

bool parameter(){
    // token uz je nacitany
    if (current_token.type == 1)                    // id
    {
        return zbytok_param();
    }
    else if (current_token.type == 15)              // _
    {
        current_token = getNextToken();
        if (current_token.type != 1)                // id
        {
            return false;
        }
        current_token = getNextToken();
        token_print();
        if (current_token.type != 12)                // :
        {
            return false;
        }
        return typ();
    }
    return false;
}

bool param_zostatok(){
    current_token = getNextToken();
    token_print();
    if (current_token.type == 13)                   // ,
    {
        current_token = getNextToken();
        return parameter() && param_zostatok();
    }
    else if (current_token.type == 21)              // epsilon
    {
        unget_token(current_token);
        return true;
    }
    return false;
}

bool zbytok_param(){
    current_token = getNextToken();
    if (current_token.type == 1 || current_token.type == 15)
    {
        token_print();
        current_token = getNextToken();
        if (current_token.type == 12)           // :
        {
            token_print();
            return typ();
        }
        return false;
    }
    return false;
}

bool sipka_typ(){
    current_token = getNextToken();
    if (current_token.type == 11)
    {
        return typ();
    }
    else if(current_token.type == 22){          // {
        unget_token(current_token);
        return true;
    }
    return false;
}

bool typ(){
    current_token = getNextToken();
    token_print();
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
    if (strcmp(current_token.attribute, ":") == 0 && current_token.type == 12){     //ked nacita : tak urcime typ
        return typ();
    }
    stop = true;                       //epsilon pravidlo - ked nacita hocico ine okrem dvojbodky, poznacime ze typ bol vynechany
    unget_token(current_token);
    return true;
}

bool priradenie_prave(){
    token_print();
    current_token = getNextToken();
    current_token2 = getNextToken();
    token_print();
    if (current_token.type == 1 && current_token2.type == 20){              // id (
        if (parametre_volania() == false){
            return false;
        }
        current_token = getNextToken();
        return (current_token.type == 21) ? true : false;                   // )
    }
    else if (current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7){
        printf("expression will be reduced:\n");
        token_print();
        unget_token(current_token2);                       //toto asi treba dat pred reduce_exp
        if (reduce_exp() == false){                         //tu uz su nacitane rovno prve dva tokeny
            return false;
        }
        printf("tu\n");
        token_print();
        return true;
    }
    return false;
}

bool rovna_sa__priradenie(){
    current_token = getNextToken();                                     // =
    if (current_token.type == 10 && strcmp(current_token.attribute, "=") == 0){
        return priradenie_prave();
    }
    else if(stop == true){              //ked bola vypustena deklaracia typu nemozeme vypustit priradenie
        return false;
    }
    else{
        unget_token(current_token);          //epsilon prechod vratime nacitany token  
        return true;
    }

}
bool letnutie(){
    current_token = getNextToken();
    token_print();
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

bool relacia(){
    printf("precedencna\n");
    if (reduce_exp() == false){
        return false;
    }
    return true;
}

bool podmienka(){
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4)
    {
        return letnutie();
    }
    else if(current_token.type == 1 || current_token.type == 2 ||  current_token.type == 3 || current_token.type == 7 || current_token.type == 20)
    {
        return relacia();
    }
    return false;
}

bool whilnutie(){
    current_token = getNextToken();
    if (podmienka()== false){
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 22){              // {
        return false;        
    }
    current_token = getNextToken();

    if (sekvencia() == false){
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){              // }
        return false;
    }
    current_token = getNextToken();
    return true;
}

bool ifnutie(){
    current_token = getNextToken();
    if (podmienka()== false){
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 22){                                                  // {
        return false;        
    }
    current_token = getNextToken();
    if (sekvencia() == false){                                                      // sekvencia
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){                                                  // }
        return false;
    }
    current_token = getNextToken();
    if (strcmp(current_token.attribute, "else") != 0 || current_token.type != 4){   // else
        return false;        
    }
    current_token = getNextToken();
    if (current_token.type != 22){                                                  // {
        return false;        
    }
    current_token = getNextToken();
    if (sekvencia() == false){                                                      // sekvencia
        return false;
    }
    current_token = getNextToken();
    if (current_token.type != 23){                                                  // }
        return false;
    }
    return true;
}

bool param_vol_zost(){
    current_token = getNextToken();
    if (current_token.type == 13){                  // ,
        current_token = getNextToken();             // id,string,int...(proste dalsi param)
        return parameter_volania() && param_vol_zost();
    }
    else if (current_token.type == 21){
        unget_token(current_token);                 // epsilon prechod musime vratit nacitany token naspat
        return true;
    }
    return false;
}

bool parameter_volania(){
    current_token2 = getNextToken();                           //: ak je dvojbodka je to volanie f(with: sth)
    
    if (current_token2.type == 12){              // id :                   
        printf("precedencna\n"); 
        current_token = getNextToken();                   // otazka je ze ci chceme nacitat token uz to alebo az v reduce_exp
        if (reduce_exp() == false){
            return false;
        }
        return true;
    }
    else if (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8){    //ked nacita vyraz string,double,int,(...
        unget_token(current_token2);            //vratime token a zacneme precedencnu analyzu vyrazu
        printf("precedencnaa\n");
        if (reduce_exp() == false){
            return false;
        }
        return true;
    }
    return false;
}
bool parametre_volania(){
    current_token = getNextToken();
    if (current_token.type == 1 || current_token.type == 2 || current_token.type == 3 || current_token.type == 7 || current_token.type == 8){    //ked nacita vyraz string,double,int,(...
        return parameter_volania() && param_vol_zost();
    }
    else if (current_token.type == 21){         // epsilon )
        unget_token(current_token);
        return true;
    }
    return false;
}

bool priradenie_zost(){
    current_token = getNextToken();                             

    if (current_token.type == 20 && parametre_volania()){       // ( paramter    
        current_token = getNextToken();
        return (current_token.type == 21) ? true : false;       // )
    }

    else if (current_token.type == 10){                         // =
        return priradenie_prave();
    }
    return false;
}

bool idnutie(){
    return priradenie_zost();
}

bool sekvencia(){
    //token_print();
    if (strcmp(current_token.attribute, "let") == 0 && current_token.type == 4){
        return letnutie();
    }
    else if (strcmp(current_token.attribute, "var") == 0 && current_token.type == 4){
        return varnutie();
    }
    else if (strcmp(current_token.attribute, "while") == 0 && current_token.type == 4){
        return whilnutie();
    }
    else if (strcmp(current_token.attribute, "if") == 0 && current_token.type == 4){
        return ifnutie();
    }
    else if (strcmp(current_token.attribute, "func") == 0 && current_token.type == 4){
        return func_declar();
    }
    else if(current_token.type == 1){
        return idnutie();
    }
    else if(current_token.type == 2 || current_token.type == 3){
        return reduce_exp();
    }
    return false;
    //TODO dalsie mozne neterminaly zo sekvencie
}

void parser(){
    while (1){
        current_token = getNextToken();
        //printf("%d\n", current_token.type);
        //printf("%s\n", current_token.attribute);
        if (current_token.type == 0 && strcmp(current_token.attribute, "END") == 0){
            printf("ok");
            break;
        }
        else if (sekvencia() == false){
            printf("Syntax Error");
            break;
        }
    }
    // current_token = getNextToken();
    // if (sekvencia() == true){
    //     printf("ok");
    // }
    // else{
    //     printf("Syntax Error");
    // }
    // while (current_token.type != 0)
    // {
    //     printf("Type: %d     ", current_token.type);
    //     printf("Attribute: %s         ", current_token.attribute);
    //     printf("prvy: %d\n", (int)current_token.first_in_line);
    //     current_token = getNextToken();
    // }
    
}
    