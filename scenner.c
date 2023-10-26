#include<stdlib.h>
#include <stdio.h>

struct Token
{
    int type;
    int attribute;
};

void string_reset(char* string){
    for (size_t i = 0; i < 100; i++)
    {
        string[i]='\0';
    }
}

void lexal(char *s){

    int i=0;
    char c=s[0];
    char state = 's';

    char string [100];
    string_reset(string);
    
    int string_pos = 0;

    struct Token token_out [100];
    int token_nmbr = 0;
    // 1 - id
    // 2 - int
    // 3 - operator
    // 4 - key word
    // 5 - assigment


    while (c != '\0')
    {
        c =s[i];

        switch (state)
        {
        case 's':
            if (('a' <=  c && c <= 'z') || ('A' <=  c && c <= 'Z'))
            {
                state='a';
                string[string_pos]=c;
                string_pos++;
                printf("char: %c je prve pismeno \n", c );
            }
            else if ('0' <=  c && c <= '9')
            {
                state='b';
                string[string_pos]=c;
                string_pos++;
                printf("char: %c je cislo \n", c );
            }
            else if (c == '+')
            {
                state='m';
                printf("char: %c je + \n", c );
            }
            else if (c == '-')
            {
                state='n';
                printf("char: %c je - \n", c );
            }
            else if (c == '*')
            {
                state='o';
                printf("char: %c je * \n", c );
            }
            else if (c == '/')
            {
                state='p';
                printf("char: %c je / \n", c );
            }
            else if (c == '=')
            {
                state='q';
                printf("char: %c je = \n", c );
            }

            else{
                state = 's';
                printf("char: %c je ine \n", c );
            }
            
            
            break;
        case 'a':                                           //  ID / key word
            if (('0' <=  c && c <= '9') ||('a' <=  c && c <= 'z') || ('A' <=  c && c <= 'Z'))
            {
                state='a';
                string[string_pos]=c;
                string_pos++;
                printf("char: %c je dalsie pismeno \n", c );
            }
            else{
                i--;
                token_out[token_nmbr].type = 1;
                token_nmbr++;
                
                state = 's';
                printf("%s \n", string);
                string_reset(string);
                string_pos=0;
            }
            break;
        case 'b':                                           // integer
            if ('0' <=  c && c <= '9')
            {
                string[string_pos]=c;
                string_pos++;
                printf("char: %c je dalsie cislo \n", c );
            }
            else{
                i--;
                token_out[token_nmbr].type = 2;
                token_nmbr++;
                state = 's';
                printf("%s \n", string);
                string_reset(string);
                string_pos=0;
            }
            break;
        case 'm':                                           // +
            token_out[token_nmbr].type = 3;
            token_nmbr++;
            state = 's';
            break;
        case 'n':                                           // -
            token_out[token_nmbr].type = 3;
            token_nmbr++;
            state = 's';
            break;
        case 'o':                                           // *
            token_out[token_nmbr].type = 3;
            token_nmbr++;
            state = 's';
            break;
        case 'p':                                           // /
            token_out[token_nmbr].type = 3;
            token_nmbr++;
            state = 's';
            break;
        case 'q':                                           // =
            token_out[token_nmbr].type = 5;
            token_nmbr++;
            state = 's';
            break;
        default:
            break;
        }
        i++;
    }

    puts("TOKENY:");
    for (int i = 0; i < 10; i++) {
        
        printf("%d, ", token_out[i].type);
    }
    

}



int main(){

    lexal("abc + 123 = X\nefg");
    puts("");
    return 1;
}