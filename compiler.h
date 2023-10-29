

typedef enum {
    NO_ERROR = 0,
    LEXICAL_ERROR = 1,
    SYNTAX_ERROR = 2,
    SEMANTIC_UNDEFINED_FUNCTION = 3,
    SEMANTIC_PARAMETER_MISMATCH = 4,
    SEMANTIC_UNDEFINED_VARIABLE = 5,
    SEMANTIC_RETURN_STATEMENT = 6,
    SEMANTIC_TYPE_COMPATIBILITY = 7,
    SEMANTIC_TYPE_INFERENCE = 8,
    OTHER_SEMANTIC_ERROR = 9,
    INTERNAL_ERROR = 99
} ErrorCode;
ErrorCode error;



struct Token
{
    int type;
    char *attribute;
};


void string_reset(char*);
char getChar(int*);
struct Token getNextToken();
void parser();