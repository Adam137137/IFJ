#include<stdlib.h>
#include <stdio.h>

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

char getChar(){
    static int i =0;
    char* input = "abc + 123 = //komentar ktory sa nezobrazi \n efg /*dalsi komentar*/ 456+789 KONIEC";
    return input[i++];
}

struct Token getNextToken(){
    struct Token token;
    int i=0;
    char state = 's';

    char string [100];
    string_reset(string);   
    int string_pos = 0;


    // 1 - string (identifikator / identifikator typu / klucove slovo )   napr.: Position_01 / String? / while
    // 2 - int 
    // 2 - double
    // 3 - operator
    // 4 - priradenie (=)
    // 5 - zatvorky

    char c = ' ';
    while (c != '\0')
    {
        c = getChar();
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
                i--;
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
                i--;
                token.type = 2;
                token.attribute = string;
                
                return token;
            }
            break;
        case 'm':                                           // +
            i--;
            token.type = 3;
            token.attribute = "+";

            return token;
        case 'n':                                           // -
            i--;
            token.type = 3;
            token.attribute = "-";

            return token;
        case 'o':                                           // *
            i--;
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
                state = 95;
            }
            
            else{                                           //  delenie
                i--;
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
            if (c == '*')
            {
                state = 96;
            }
            else{
                state = 95;
            }
            break;    
        case 96:
            if (c =='/')
            {
                state = 's';
            }
            else{
                state = 95;
            }
            break;
        case 'q':                                           // =
            i--;
            token.type = 4;
            token.attribute = "=";
            
            return token;
        default:
            break;
        }
        i++;
    }
    token.type = 0;
    token.attribute = "err";
    return token;
}



void parser(){
    for (int i = 0; i < 10; i++)
    {
        struct Token nextToken = getNextToken();
        printf("Type: %d     ", nextToken.type);
        printf("Attribute: %s\n", nextToken.attribute);
    }
    
}


int main(){
    puts("abc + 123 = //komentar ktory sa nezobrazi \n efg /*dalsi komentar*/ 456+789 KONIEC");
    parser();
    puts("");
    return 1;
}