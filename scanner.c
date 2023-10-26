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
    char* input = "abc + 123 = XYZ \nefg";
    return input[i++];
}

struct Token getNextToken(){
    struct Token token;
    int i=0;
    char state = 's';

    char string [100];
    string_reset(string);   
    int string_pos = 0;


    // 1 - id
    // 2 - int
    // 3 - operator
    // 4 - key word
    // 5 - assigment

    char c = ' ';
    while (c != '\0')
    {
        c = getChar();
        switch (state)
        {
        case 's':
            if (('a' <=  c && c <= 'z') || ('A' <=  c && c <= 'Z'))
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
                state='p';
            }
            else if (c == '=')
            {
                state='q';
            }

            else{
                state = 's';
            }
            
            
            break;
        case 'a':                                           //  ID / key word
            if (('0' <=  c && c <= '9') ||('a' <=  c && c <= 'z') || ('A' <=  c && c <= 'Z'))
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
        case 'p':                                           // /
            i--;
            token.type = 3;
            token.attribute = "/";

            return token;
        case 'q':                                           // =
            i--;
            token.type = 3;
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
    for (int i = 0; i < 7; i++)
    {
        struct Token nextToken = getNextToken();
        printf("Type: %d     ", nextToken.type);
        printf("Attribute: %s\n", nextToken.attribute);
    }
    
}


int main(){

    parser();
    puts("");
    return 1;
}