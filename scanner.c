/*
 * Projekt: Implementace překladače imperativního jazyka IFJ23
 *
 * autori suboru:
 * xvarsa01 - Adam Varsanyi
 * 247112 - Peter Gvozdjak
 *
 * 
*/

#include "scanner.h"
#include "compiler.h"

char *key_words[] = {"Double","Double?", "else", "func", "if", "Int", "Int?", "let", "nil", "return", "String", "String?", "var", "while"};
char *built_in_functions[] = {"readString", "readInt", "readDouble", "write", "Int2Double", "Double2Int", "length", "substring", "ord", "chr"};
char string [100];
bool integerWithE = false;
int a;
char c2;
char c3;

void string_reset(char *string){
    for (size_t i = 0; i < 100; i++)
    {
        string[i]='\0';
    }
}


bool IsItKeyWord (char *c){
    for (int i = 0; i < key_words_length; i++)
    {
        if (strcmp(c, key_words[i]) == 0)
        {
            return true;
        }
    }
    return false;
}
bool IsItBuiltIn (char *c){
    for (int i = 0; i < built_in_functions_length; i++)
    {
        if (strcmp(c, built_in_functions[i]) == 0)
        {
            return true;
        }
    }
    return false;
}


struct Token getNextToken(){
    struct Token token;
    token.first_in_line = false;
    bool viac_riadkovy_string = false;             
    char state = 's';
    bool first_in_new_line = false;
    bool text_in_new_line = false;
    bool expect_new_line = false;
    bool multiline_exit_possible = true;
    int comments_inside_count = 0;

    // 1  - identifikator                    napr.: Position_01
    // 2  - int 
    // 3  - double
    // 4  - klucove slovo        napr.: / String? / while
    // 5  - operatory
    // 6  - operatory porovnavacie
    // 7  - string
    // 8  - string """
    // 9  - ??
    // 10 - priradenie (=)
    // 11 - ->
    // 12 - :
    // 13 - ,
    // 14 - !
    // 15 - _
    // 16 - vstavana funkcia
    // 20 - (
    // 21 - )
    // 22 - {
    // 23 - }
    
    string_reset(string);
    int string_pos = 0;

    char c = ' ';
    while (c  != EOF)
    {
        if (string_pos ==98)
        {
            handle_error(INTERNAL_ERROR);
        }
        
        c = getc(file);
        switch (state)
        {
        case 's':
            if (isalpha(c) || c =='_')
            {   
                if (first_in_new_line)
                {
                    instruction_per_line = 0;
                    token.first_in_line = true;
                    first_in_new_line = false;
                }
                
                state=1;
                string[string_pos]=c;
                string_pos++;
            }
            else if ('0' <=  c && c <= '9')
            {
                if (first_in_new_line)
                {
                    instruction_per_line =0;
                    token.first_in_line = true;
                    first_in_new_line = false;
                }

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
                c2 =getc(file);
                c3 =getc(file);
                if (c2 == '"' && c3 == '"')
                {
                    viac_riadkovy_string = true;
                    state = 60;
                }
                else{
                    state = 60;
                    ungetc(c3,file);
                    ungetc(c2,file);
                }
            }
            else if (c == '(')
            {
                token.type = 20;
                token.attribute = "(";
                return token;
            }
            else if (c == ')')
            {
                token.type = 21;
                token.attribute = ")";
                return token;
            }
            else if (c == '{')
            {
                token.type =22;
                token.attribute = "{";
                return token;
            }
            else if (c == '}')
            {
                token.type = 23;
                token.attribute = "}";
                return token;
            }
            else if (c == ',')
            {
                token.type = 13;
                token.attribute = ",";
                return token;
            }
            else if (c == ':')
            {
                token.type = 12;
                token.attribute = ":";
                return token;
            }
            else if (c == '\n')
            {
                // puts("prvy");
                instruction_per_line = 0;
                first_in_new_line =  true;
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
                
                if (IsItKeyWord(string))
                {
                    token.type = 4;
                }
                else if (IsItBuiltIn(string)){
                    token.type = 16;    
                }
                else if (strcmp(string, "_") == 0)
                {
                    token.type = 15;
                }
                
                else{
                    token.type = 1;
                }

                token.attribute = string_dup(string);
                
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
                token.attribute = string_dup(string);
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
                handle_error(LEXICAL_ERROR);
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
                token.attribute = string_dup(string);
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
                handle_error(LEXICAL_ERROR);
            }
            break;

        case 14:
            string[string_pos]=c;
            string_pos++;
            if ('0' <=  c && c <= '9'){
                state = 15;
            }
            else{
                handle_error(LEXICAL_ERROR);
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
                token.attribute = string_dup(string);
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
            return token;
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
        case 50:
            if (c == '?'){
                token.type = 9;
                token.attribute = "??";
                return token;
            }
            else{
                handle_error(LEXICAL_ERROR);
            }
            break;
        case 60:                                           //string
            if (c == '"' && viac_riadkovy_string == false){             //ked nie je viacriadkovy string tak staci 1x" na ukoncenie
                token.type = 7;
                token.attribute = string_dup(string);
                return token;  
            }
            else if (c == '"' && viac_riadkovy_string == true){
                c2 = getc(file); c3 =  getc(file);                             
                if (c2 == '"' && c3 == '"'){            //ukoncenie viacriaddkoveho stringu
                    if (!multiline_exit_possible)
                    {
                        handle_error(LEXICAL_ERROR);
                    }                    
                    token.type = 8;
                    token.attribute = string_dup(string);
                    viac_riadkovy_string = false;
                    return token;
                }
                else{
                    handle_error(LEXICAL_ERROR);
                }
            }
            else if (c > (char)32 && c != 34 && c != 35 && c != 92){               // v stringu nemoze byt " bez opacneho lomitka
                if (expect_new_line)
                {
                    string[string_pos]='\\';
                    string_pos++;
                    string[string_pos]='0';
                    string_pos++;
                    string[string_pos]='1';
                    string_pos++;
                    string[string_pos] = '0';
                    string_pos++;
                    expect_new_line = false;
                }
                string[string_pos]=c;
                string_pos++;
                text_in_new_line = true;
                multiline_exit_possible = false;
            }
            else if (c == 32){            // 32 je ' '
                if (expect_new_line)
                {
                    string[string_pos]='\\';
                    string_pos++;
                    string[string_pos]='0';
                    string_pos++;
                    string[string_pos]='1';
                    string_pos++;
                    string[string_pos] = '0';
                    string_pos++;
                    expect_new_line = false;
                }
                    string[string_pos] = '\\';
                    string_pos++;
                    string[string_pos] = '0';
                    string_pos++;
                    string[string_pos] = '3';
                    string_pos++;
                    string[string_pos] = '2';
                    string_pos++;
            }
            else if (c == 35){            // 35 je '#'
                string[string_pos] = '\\';
                string_pos++;
                string[string_pos] = '0';
                string_pos++;
                string[string_pos] = '3';
                string_pos++;
                string[string_pos] = '5';
                string_pos++;
            }
            else if (c == '\\'){
                state = 65;
            }
            else if (c == '\n' && viac_riadkovy_string){
                if (text_in_new_line){
                    expect_new_line = true;
                    text_in_new_line = false;
                    multiline_exit_possible = true;
                }
            }
            else{
                handle_error(LEXICAL_ERROR);
            }
            break;
        case 65:
            if (c == 'n'){
                string[string_pos]='\\';
                string_pos++;
                string[string_pos]='0';
                string_pos++;
                string[string_pos]='1';
                string_pos++;
                string[string_pos] = '0';
                string_pos++;
                state = 60;
            }
            else if (c == 't'){
                string[string_pos]='\\';
                string_pos++;
                string[string_pos]='0';
                string_pos++;
                string[string_pos]='1';
                string_pos++;
                string[string_pos] = '1';
                string_pos++;
                string[string_pos] = '\t';
                string_pos++;
                state = 60;

            }
            else if (c == '\"'){
                string[string_pos] = '\"';
                string_pos++;
                state = 60;
                
            }
            else if (c == '\\'){
                string[string_pos] = '\\';
                string_pos++;
                state = 60;
                
            }
            else if (c == '\r'){
                string[string_pos] = '\r';
                string_pos++;
                state = 60;
                
            }
            else if (c == 'u'){
                state = 66;
            }

            else{
                string[string_pos]='\\';
                string_pos++;
                string[string_pos]='0';
                string_pos++;
                string[string_pos]='9';
                string_pos++;
                string[string_pos] = '2';
                string_pos++;
                state = 60;
                ungetc(c, file);
            }
            break;          // TODO

        case 66:
            if (c == '{'){
                state = 67;
            }
            else{
                handle_error(LEXICAL_ERROR);
            }
            break;
        case 67:
            c3 = getc(file);
            if (97 <= c && c <= 102){
                c= c-32; 
            }
            if (97 <= c3 && c3 <= 102){
                c3 = c3-32;
            }
            if (((65 <= c && c <= 70) || (48 <= c && c <= 57)) || ((65 <= c3 && c3 <= 70) ||  (48 <= c3 && c3 <= 57))){
                char res[3];
                char *endPtr;
                res[0] = c;
                res[1] = c3;
                res[2] = '\0';
                unsigned int a = strtol(res,&endPtr,16);
                string[string_pos]= (char)a;
                string_pos++;
                //printf("%s", string);
                state = 69;
            }
            else{
                handle_error(LEXICAL_ERROR);
            }
            //ungetc(c3,file);
            //ungetc(c, file);
            break;
        case 69:
            //printf("%c\n", c);
            if (c == '}'){
                state = 60;
            }
            else{
                handle_error(LEXICAL_ERROR);
            }
            break;





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
                token.type = 5;
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
            c2 =getc(file);                         //precitam o jeden znak viac aby som sa vedel rozhodnut
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
            // fprintf(stderr, "this state was not implemented\n");
            handle_error(INTERNAL_ERROR);       
            break;
        }

    }
    // posledny token;
    if (comments_inside_count !=0){
        handle_error(LEXICAL_ERROR);
    }
    
    token.type = 0;
    token.attribute = "END";
    return token;
}
