#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define null ((void *)0)

typedef enum TokenType {
    TYPE_INT, TYPE_ARRAY, TYPE_STRUCT, TYPE_IF, TYPE_FOR,
    TYPE_FUNC, TYPE_RETURN, TYPE_IMPORT, TYPE_PLUS, TYPE_MIN,
    TYPE_DIV, TYPE_MUL, TYPE_BITWISE_AND, TYPE_BITWISE_OR, TYPE_UNARY,
    TYPE_BITWISE_XOR, TYPE_LSHIFT, TYPE_RSHIFT, TYPE_EQUAL_PLUS, TYPE_EQUAL_MIN,
    TYPE_EQUAL_DIV, TYPE_EQUAL_MUL, TYPE_EQUAL_BITWISE_AND, TYPE_EQUAL_BITWISE_OR,
    TYPE_EQUAL_UNARY, TYPE_EQUAL_LSHIFT, TYPE_EQUAL_RSHIFT, TYPE_EQUAL_XOR, TYPE_EOF,
    TYPE_DOUBLE_EQUAL, TYPE_LPAREN, TYPE_RPAREN, TYPE_LBRACKED, TYPE_RBRACKED, TYPE_LBRACE,
    TYPE_ID, TYPE_INT_LITERAL, TYPE_HEX_LITERAL, TYPE_UNKNOWN, TYPE_RBRACE, TYPE_EQUAL, TYPE_EMPTY,
    TYPE_SEMICOLON
} TokenType;

typedef struct LexerType {
    const char *keyword;
    TokenType type;
} LexerType;

typedef struct Token {
    TokenType type;
    char *value;
    size_t line;
    size_t column;
} Token;

typedef struct Lexer {
    const char *source;
    size_t cursor;
    size_t line;
    size_t column;
} Lexer;

TokenType tokenize_lookup(const char *lexema);
const char *type_name(TokenType type);
char peek(Lexer *lexer);
char advance(Lexer *lexer);

void skipped(Lexer *lexer);
char *dup(const char *source);
Token *tokenize(Lexer *lexer);