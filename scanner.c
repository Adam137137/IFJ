#include<stdlib.h>
#include <stdio.h>
/*vyansobil som*/

char* input = "ab /* ano /* ok */ nie */- +";

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

char getChar(int *pos){
    return input[(*pos)++];
}

struct Token getNextToken(){
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

    char c = ' ';
    while (c != '\0')
    {
        c = getChar(&posInInput);
        switch (state)
        {
        case 's':
            if (('a' <=  c && c <= 'z') || ('A' <=  c && c <= 'Z') || c =='_' || c =='?')
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
                posInInput--;

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
            else{
                posInInput--;
                token.type = 2;
                token.attribute = string;
                
                return token;
            }
            break;
        case 'm':                                           // +
            posInInput--;
            token.type = 3;
            token.attribute = "+";

            return token;
        case 'n':                                           // -
            posInInput--;
            token.type = 3;
            token.attribute = "-";

            return token;
        case 'o':                                           // *
            posInInput--;
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
                posInInput--;

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
            char c2 =getChar(&posInInput);                         //precitam o jeden znak viac aby som sa vedel rozhodnut
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
            posInInput--;
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
        case 'q':                                           // =
            posInInput--;
            token.type = 4;
            token.attribute = "=";
            
            return token;
        default:
            break;
        }

    }

    // tu by sme sa niky nemali dostat;
    token.type = 0;
    token.attribute = "err";
    return token;
}



void parser(){
    for (int i = 0; i < 3; i++)
    {
        struct Token nextToken = getNextToken();
        printf("Type: %d     ", nextToken.type);
        printf("Attribute: %s\n", nextToken.attribute);
    }
    
}

int main(){
    printf("input:\n%s\n", input);
    parser();
    puts("");
    return 1;
}