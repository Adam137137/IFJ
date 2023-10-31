#include<stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
    static int posInInput=0;

    char string [100];
    string_reset(string);
    int string_pos = 0;

    int comments_inside_count = 0;

    // 1 - string (identifikator / identifikator typu / klucove slovo )   napr.: Position_01 / String? / while
    // 2 - int 
    // 2 - double
    // 3 - operator
    // 4 - priradenie (=)
    // 5 - zatvorky
    // 6 - operatory porovnavacie
    // 7 - string


    char c = ' ';
    while (c  != EOF)
    {
        c = getc(file);
        switch (state)
        {
        case 's':
            if (isalpha(c) || c =='_' || c =='?')
            {   
                state='a';
                string[string_pos]=c;
                string_pos++;
            }
            else if ('0' <=  c && c <= '9')
            {
                state='b';
                string[string_pos]=c;
                string_pos++;
            }
            else if (c == '+')
            {
                state='m';
            }
            else if (c == '-')
            {
                state='n';
            }
            else if (c == '*')
            {
                state='o';
            }
            else if (c == '/')
            {
                state=90;
            }
            else if (c == '=')
            {
                state='q';
            }
            else if (c == '?')      //operator ??
            {
                state='r';
            }
            else if (c == '*'){
                state = 'o';
            }
            else if (c == '<'){
                state = 70;
            }
            else if( c == '>'){
                state = 60;
            }
            else if (c == '"'){
                string[string_pos]=c;
                string_pos++;
                state = 80;
            }
            else if (c == EOF){
                break;
            }
            else{
                state = 's';
            }
            
            break;

        case 'a':                                           //  ID / key word
            if (('0' <=  c && c <= '9') ||('a' <=  c && c <= 'z') || ('A' <=  c && c <= 'Z') || c =='_' || c =='?')
            {
                state='a';
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
        case 'b':                                           // integer
            if ('0' <=  c && c <= '9')
            {
                string[string_pos]=c;
                string_pos++;
            }
            else if (c == '.'){                            //decimal
                string[string_pos]=c;
                string_pos++;
                state = 100;
            }
            else if ( c == 'e' || c == 'E'){
                string[string_pos]=c;
                string_pos++;
                state = 101;
            }
            else{
                ungetc(c, file);
                token.type = 2;
                token.attribute = string;
                return token;
            }
            break;
        case 100:
            string[string_pos]=c;
            string_pos++;
            if ('0' <=  c && c <= '9'){
                
                state = 101;
            }
            else{
                printf("lexical error\n");
                //return LEXICAL_ERROR;
            }
            break;
        
        case 101:
            if ('0' <=  c && c <= '9')
            {
                string[string_pos]=c;
                string_pos++;
                state = 101;
            }
            else if (c == 'e' || c == 'E'){
                string[string_pos]=c;
                string_pos++;
                state = 102;
            }
            else{
                ungetc(c,file);
                token.type = 2;
                token.attribute = string;
                return token;
            }
            break;

        case 102:
            string[string_pos]=c;
            string_pos++;
            if ('0' <=  c && c <= '9'){
                
                state = 104;
            }
            else if (c == '+' || c == '-'){
                
                state = 103;
            }
            else{
                printf("Lexical error\n");
            }
            break;

        case 103:
            string[string_pos]=c;
            string_pos++;
            if ('0' <=  c && c <= '9'){
                state = 104;
            }
            else{
                printf("Lexical error\n");
            }
            break;
        case 104:
            
            if ('0' <=  c && c <= '9'){
                string[string_pos]=c;
                string_pos++;
                state = 104;
            }
            else{
                ungetc(c,file);
                token.type = 2;
                token.attribute = string;
                return token;
            }
            break;

        case 80:                                           //string
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
        case 81:
            if (c == 'n' || c == 't' || c == '\"' || c == '\\'){
                //todo dokoncit stringy ked dostaneme escape charaktery + este napr\u{1F} previes zo 16tkovej do dekadickej sustavy
            }
        case 60:
            if (c == '='){
                state = 61;
            }
            else{
                ungetc(c,file);
                token.type = 6;
                token.attribute = ">";
                return token;
            }
        case 61:
            ungetc(c,file);
            token.type = 6;
            token.attribute = ">=";
            return token;

        case 70:
            if (c == '='){
                state = 71;
            }
            else{
                ungetc(c,file);
                token.type = 6;
                token.attribute = "<";
                return token;
            }
            break;
        case 71:
            ungetc(c,file);
            token.type = 6;
            token.attribute = "<=";
            return token;
        
        case 'm':                                           // +
            ungetc(c,file);
            token.type = 3;
            token.attribute = "+";
            return token;

        case 'n':                                           // -
            ungetc(c,file);
            token.type = 3;
            token.attribute = "-";

            return token;
        case 'o':                                           // *
            ungetc(c,file);
            token.type = 3;
            token.attribute = "*";
            return token;
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
            else{
                state = 91;
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
                    //printf("%d", comments_inside_count);
                    state = 95;
                }
            }
            else{
                //printf("%d", comments_inside_count);
                state = 95;
            }
            break;
        case 'q':                                         // =
            if (c == '='){
                state = 'v';
            }
            else{
                ungetc(c,file);
                token.type = 4;
                token.attribute = "=";
                return token;
            }
            break;
        case 'v':
            ungetc(c,file);
            token.type = 3;
            token.attribute = "==";
            return token;
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