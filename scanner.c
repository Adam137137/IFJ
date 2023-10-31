#include<stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

struct Token
{
    int type;
    char *attribute;
};

void string_reset(char* string){
    for (size_t i = 0; i < 100; i++)
    {
        string[i]='\0';
    }
}


struct Token getNextToken(FILE* file){
    struct Token token;

    char state = 's';

    char string [100];
    string_reset(string);
    int string_pos = 0;

    int comments_inside_count = 0;

    // 1  - identifikator                    napr.: Position_01
    // 2  - int 
    // 3  - double
    // 4  - klucove slovo        napr.: / String? / while
    // 5  - operatory
    // 6  - operatory porovnavacie
    // 7  - string
    // 8  - string """
    // 9  - zatvorky
    // 10 - priradenie (=)
    // 11 - ->
    // 12 - :
    // 13 - ,
    // 14 - !


    bool integerWithE = false;
    char c = ' ';
    while (c  != EOF)
    {
        c = getc(file);
        switch (state)
        {
        case 's':
            if (isalpha(c) || c =='_')
            {   
                state=1;
                string[string_pos]=c;
                string_pos++;
            }
            else if ('0' <=  c && c <= '9')
            {
                state=10;
                string[string_pos]=c;
                string_pos++;
            }
            else if (c == '+')
            {
                state=20;
            }
            else if (c == '-')
            {
                state=21;
            }
            else if (c == '*')
            {
                state=25;
            }
            else if (c == '/')
            {
                state=90;
            }
            else if (c == '!')
            {
                state=30;
            }
            else if (c == '=')
            {
                state=35;
            }
            else if (c == '<')
            {
                state=40;
            }
            else if (c == '>')
            {
                state=45;
            }
            else if (c == '?')      //operator ??
            {
                state=50;
            }

            else if (c == '"'){
                char c2 =getc(file);
                char c3 =getc(file);
                if (c2 == '"' && c3 == '"')
                {
                    state = 70;
                }
                else{
                    state = 60;
                }
                ungetc(c2,file);
                ungetc(c3,file);
            }

            else if (c == '(')
            {
                token.type = 9;
                token.attribute = "(";
                return token;
            }
            else if (c == ')')
            {
                token.type = 9;
                token.attribute = ")";
                return token;
            }
            else if (c == '{')
            {
                token.type = 9;
                token.attribute = "{";
                return token;
            }
            else if (c == '}')
            {
                token.type = 9;
                token.attribute = "}";
                return token;
            }
            else if (c == ',')
            {
                token.type = 13;
                token.attribute = ",";
                return token;
            }
            
            break;

        case 1:                                           //  ID / key word
            if (('0' <=  c && c <= '9') ||('a' <=  c && c <= 'z') || ('A' <=  c && c <= 'Z') || c =='_' || c =='?')
            {
                string[string_pos]=c;
                string_pos++;

            }
            else{
                ungetc(c,file);

                /* je to identifikator alebo klucove slovo, pozri sa do tabulky
                 boolean IsKeyword IsItKeyWord(string);

                if (IsKeyword)
                {
                    token.type = 4;
                }
                else{
                    token.type = 1
                }
                */

                token.type = 1;
                token.attribute = string;
                
                return token;
            }
            break;
        case 10:                                           // integer
            if ('0' <=  c && c <= '9')
            {
                string[string_pos]=c;
                string_pos++;
            }
            else if (c == '.'){                            //float
                string[string_pos]=c;
                string_pos++;
                state = 11;
            }
            else if ( c == 'e' || c == 'E'){
                integerWithE=true;
                string[string_pos]=c;
                string_pos++;
                state = 13;
            }
            else{
                ungetc(c, file);
                token.type = 2;
                token.attribute = string;
                return token;
            }
            break;
        case 11:
            string[string_pos]=c;
            string_pos++;
            if ('0' <=  c && c <= '9'){     
                state = 12;
            }
            else{
                printf("lexical error\n");
                //return LEXICAL_ERROR;
            }
            break;
        
        case 12:
            if ('0' <=  c && c <= '9')
            {
                string[string_pos]=c;
                string_pos++;
            }
            else if (c == 'e' || c == 'E'){
                string[string_pos]=c;
                string_pos++;
                state = 13;
            }
            else{
                ungetc(c,file);
                token.type = 3;                     // FLOAT
                token.attribute = string;
                return token;
            }
            break;

        case 13:
            string[string_pos]=c;
            string_pos++;
            if ('0' <=  c && c <= '9'){
                state = 15;
            }
            else if (c == '+' || c == '-'){
                state = 14;
            }
            else{
                printf("Lexical error\n");
            }
            break;

        case 14:
            string[string_pos]=c;
            string_pos++;
            if ('0' <=  c && c <= '9'){
                state = 15;
            }
            else{
                printf("Lexical error\n");
            }
            break;
        case 15:
            if ('0' <=  c && c <= '9'){
                string[string_pos]=c;
                string_pos++;
            }
            else{
                ungetc(c,file);
                if (integerWithE)
                {
                    token.type = 2;
                    integerWithE = false;
                }
                else{
                    token.type = 3;
                }
                token.attribute = string;
                return token;
            }
            break;
        case 20:
            ungetc(c,file);
            token.type = 5;
            token.attribute = "+";
            return token;
        case 21:                                           // -
            if (c=='>')
            {
                state = 22;
                break;
            }
            else
            {
                ungetc(c,file);
                token.type = 5;
                token.attribute = "-";
                return token;
            }
        case 22:
            ungetc(c,file);
            token.type = 11;
            token.attribute = "->";
            break;
        case 25:                                           // *
            ungetc(c,file);
            token.type = 5;
            token.attribute = "*";
            return token;
        case 30:                                           // !
            if (c=='=')
            {
                state = 31;
                break;
            }
            else
            {
                ungetc(c,file);
                token.type = 14;
                token.attribute = "!";
                return token;
            }
        case 31:
            ungetc(c,file);
            token.type = 6;
            token.attribute = "!=";
            return token;

            break;
        case 35:                                            // =
            if (c == '='){
                state = 36;
                break;
            }
            else{
                ungetc(c,file);
                token.type = 10;
                token.attribute = "=";
                return token;
            }
        case 36:
            ungetc(c,file);
            token.type = 6;
            token.attribute = "==";
            return token;

            break;

        case 40:
            if (c == '='){
                state = 41;
                break;
            }
            else{
                ungetc(c,file);
                token.type = 6;
                token.attribute = "<";
                return token;
            }
        case 41:
            ungetc(c,file);
            token.type = 6;
            token.attribute = "<=";
            return token;
        case 45:
            if (c == '='){
                state = 46;
                break;
            }
            else{
                ungetc(c,file);
                token.type = 6;
                token.attribute = ">";
                return token;
            }
        case 46:
            ungetc(c,file);
            token.type = 6;
            token.attribute = ">=";
            return token;

        case 60:                                           //string
            if (c == '"'){
                string[string_pos]=c;
                string_pos++;
                //ungetc(c,file);
                token.type = 7;
                token.attribute = string;
                return token;  
            }
            else if (c > (char)31){
                string[string_pos]=c;
                string_pos++;
                state = 80;
            }
            else if (c == '\\'){
                state = 81;
            }
            else{
                printf("lexical errorsss\n");
            }
            break;
        case 65:
            if (c == 'n' || c == 't' || c == '\"' || c == '\\'){
               //todo dokoncit stringy ked dostaneme escape charaktery + este napr\u{1F} previes zo 16tkovej do dekadickej sustavy
                state = 60;
            }
            break;          // TODO






        case 90:                                            // "/"
            if (c == '/')                                   // "//" - riadkovy koementar
            {
                state = 91;
            }
            else if (c == '*')                              // "/*" - blokovy komentar
            {
                comments_inside_count++;
                state = 95;
            }
            
            else{                                           //  delenie
                ungetc(c,file);
                token.type = 3;
                token.attribute = "/";
                return token;
            }
            break;
        case 91:
            if (c == '\n')
            {
                state = 's';
            }
            break;            
        case 95:                                                // "/*" - blokovy koementar
            char c2 =getc(file);                         //precitam o jeden znak viac aby som sa vedel rozhodnut
            //printf("%d", c);
            //printf("%d\n", c2);
            if (c == '*' && c2 == '/')
            {               
                state = 96;
            }
            else if (c == '/' && c2 == '*')
            {
                state = 90;
            }
            else{
                
                state = 95;
            }
            ungetc(c2,file);
            break;    
        case 96:
            if (c =='/')
            {
                comments_inside_count--;
                if (comments_inside_count == 0)
                {                    
                    state = 's';
                }
                else{
                    state = 95;
                }
            }
            else{
                state = 95;
            }
            break;

        default:
            //printf("error");
            break;
        }

    }
    // posledny token;
    token.type = 0;
    token.attribute = "END";
    return token;
}



void parser(FILE* file){
    struct Token nextToken = getNextToken(file);
    while (nextToken.type != 0)
    {
        printf("Type: %d     ", nextToken.type);
        printf("Attribute: %s\n", nextToken.attribute);
        nextToken = getNextToken(file);
    }
    
}

int main(int argc, char *argv[]){
    FILE *file = fopen("test.txt", "r");
    //char firstchar = fgetc(file);
    //printf("%c", firstchar);
    parser(file);
    puts("");
    return 1;
}