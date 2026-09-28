#include "include/lexer.h"

const LexerType keywordlist[] = {
    {"int", TYPE_INT}, {"array", TYPE_ARRAY}, {"struct", TYPE_STRUCT},
    {"if", TYPE_IF}, {"for", TYPE_FOR}, {"func", TYPE_FUNC},
    {"return", TYPE_RETURN}, {"import", TYPE_IMPORT}, {null, TYPE_EMPTY}
};

const LexerType oplist[] = {
    {"+", TYPE_PLUS}, {"-", TYPE_MIN}, {"*", TYPE_MUL}, {"/", TYPE_DIV},
    {"&", TYPE_BITWISE_AND}, {"|", TYPE_BITWISE_OR}, {"^", TYPE_BITWISE_XOR}, {"~", TYPE_UNARY},
    {">>", TYPE_RSHIFT}, {"<<", TYPE_LSHIFT}, {"+=", TYPE_EQUAL_PLUS}, {"-=", TYPE_EQUAL_MIN},
    {"*=", TYPE_EQUAL_MUL}, {"/=", TYPE_EQUAL_DIV}, {"&=", TYPE_EQUAL_BITWISE_AND}, {"~=", TYPE_EQUAL_UNARY},
    {"|=", TYPE_EQUAL_BITWISE_OR}, {"^=", TYPE_EQUAL_XOR}, {"<<=", TYPE_EQUAL_LSHIFT}, {">>=", TYPE_EQUAL_RSHIFT},
    {"==", TYPE_DOUBLE_EQUAL}, {"(", TYPE_LPAREN}, {")", TYPE_RPAREN}, {"{", TYPE_LBRACKED},
    {"}", TYPE_RBRACKED}, {"[", TYPE_LBRACE}, {"]", TYPE_RBRACE}, {"=", TYPE_EQUAL},
    {";", TYPE_SEMICOLON}, {null, TYPE_EMPTY}
};

const char *type_names[] = {
    "TYPE_INT", "TYPE_ARRAY", "TYPE_STRUCT", "TYPE_IF", "TYPE_FOR",
    "TYPE_FUNC", "TYPE_RETURN", "TYPE_IMPORT", "TYPE_PLUS", "TYPE_MIN",
    "TYPE_DIV", "TYPE_MUL", "TYPE_BITWISE_AND", "TYPE_BITWISE_OR", "TYPE_UNARY",
    "TYPE_BITWISE_XOR", "TYPE_LSHIFT", "TYPE_RSHIFT", "TYPE_EQUAL_PLUS", "TYPE_EQUAL_MIN",
    "TYPE_EQUAL_DIV", "TYPE_EQUAL_MUL", "TYPE_EQUAL_BITWISE_AND", "TYPE_EQUAL_BITWISE_OR",
    "TYPE_EQUAL_UNARY", "TYPE_EQUAL_LSHIFT", "TYPE_EQUAL_RSHIFT", "TYPE_EQUAL_XOR", "TYPE_EOF",
    "TYPE_DOUBLE_EQUAL", "TYPE_LPAREN", "TYPE_RPAREN", "TYPE_LBRACKED", "TYPE_RBRACKED",
    "TYPE_LBRACE", "TYPE_ID", "TYPE_INT_LITERAL", "TYPE_HEX_LITERAL", "TYPE_UNKNOWN",
    "TYPE_RBRACE", "TYPE_EQUAL", "TYPE_EMPTY", "TYPE_SEMICOLON"
};

const char *type_name(TokenType type) {
    if (type < 0 || type > TYPE_SEMICOLON) return "TYPE_INVALID";
    return type_names[type];
}

TokenType tokenize_lookup(const char *lexema) {
    if (!lexema) return TYPE_UNKNOWN;

    for (long i = 0; keywordlist[i].keyword; i++)
        if (strcmp(lexema, keywordlist[i].keyword) == 0) return keywordlist[i].type;

    for (long i = 0; oplist[i].keyword; i++)
        if (strcmp(lexema, oplist[i].keyword) == 0) return oplist[i].type;

    return TYPE_ID;
}

char peek(Lexer *lexer) {
    return lexer->source[lexer->cursor];
}

char advance(Lexer *lexer) {
    char c = peek(lexer);

    if(c != '\0') {
        lexer->cursor++;

        if(c == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else {
            lexer->column++;
        }
    }

    return c;
}

void skipped(Lexer *lexer) {
    while(peek(lexer) != '\0' && isspace((unsigned char)peek(lexer))) {
        advance(lexer);
    }

    if(peek(lexer) == '/' && lexer->source[lexer->cursor + 1] == '/') {
        while(peek(lexer) != '\0' && peek(lexer) != '\n') {
            advance(lexer);
        }
    }
}

char *dup(const char *source) {
    if(!source) return null;
    size_t length = strlen(source) + 1;

    char *dest = malloc(length);
    if(!dest) return null;

    strcpy(dest, source);
    return dest;
}

Token *tokenize(Lexer *lexer) {
    skipped(lexer);

    size_t token_line = lexer->line;
    size_t token_column = lexer->column;

    unsigned char c = peek(lexer);
    Token *tokens = malloc(sizeof(Token));

    if (c == '\0') {
        tokens->type = TYPE_EOF;
        tokens->value = dup(null);
        tokens->line = token_line;
        tokens->column = token_column;

        if (!tokens->value) {
            free(tokens);
            return null;
        }

        return tokens;
    }

    if(isalpha(c) || c == '_') {
        size_t start = lexer->cursor;
        while(isalnum((unsigned char)peek(lexer)) || peek(lexer) == '_') {
            advance(lexer);
        }

        size_t length = lexer->cursor - start;
        tokens->value = malloc(length + 1);

        if(!tokens->value) {
            free(tokens);
            return null;
        }

        memcpy(tokens->value, lexer->source + start, length);
        tokens->value[length] = '\0';
        tokens->type = TYPE_ID;

        tokens->type = tokenize_lookup(tokens->value);
        return tokens;
    }

    if (c == '0' &&
        (lexer->source[lexer->cursor + 1] == 'x' ||
        lexer->source[lexer->cursor + 1] == 'X')) {
        size_t start = lexer->cursor;

        advance(lexer);
        advance(lexer);

        while (isxdigit((unsigned char)peek(lexer))) {
            advance(lexer);
        }

        size_t length = lexer->cursor - start;
        tokens->value = malloc(length + 1);

        if (!tokens->value) {
            free(tokens);
            return null;
        }

        memcpy(tokens->value, lexer->source + start, length);
        tokens->value[length] = '\0';

        tokens->type = TYPE_HEX_LITERAL;
        return tokens;
    }

    if(isdigit(c)) {
        size_t start = lexer->cursor;
        while(isdigit((unsigned char)peek(lexer))) {
            advance(lexer);
        }

        size_t length = lexer->cursor - start;
        tokens->value = malloc(length + 1);

        if(!tokens->value) {
            free(tokens);
            return null;
        }

        memcpy(tokens->value, lexer->source + start, length);
        tokens->value[length] = '\0';

        tokens->type = TYPE_INT_LITERAL;
        return tokens;
    }

    size_t oplen = 0;
    for (long i = 0; oplist[i].keyword; i++) {
        size_t n = strlen(oplist[i].keyword);
        if (n <= oplen) continue;
        if (strncmp(lexer->source + lexer->cursor, oplist[i].keyword, n) == 0) oplen = n;
    }

    if (oplen == 0) {
        advance(lexer);
        tokens->type = TYPE_UNKNOWN;
        tokens->value = dup(null);
        return tokens;
    }

    size_t length = oplen;
    tokens->value = malloc(length + 1);

    if (!tokens->value) {
        free(tokens);
        return null;
    }

    memcpy(tokens->value, lexer->source + lexer->cursor, length);
    tokens->value[length] = '\0';

    for (size_t i = 0; i < length; i++) advance(lexer);

    tokens->line = token_line;
    tokens->column = token_column;
    
    tokens->type = tokenize_lookup(tokens->value);
    return tokens;
}