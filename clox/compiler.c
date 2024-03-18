#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

typedef struct
{
    Token current;
    Token previous;
    bool hadError;
} Parser;

Parser parser;

static void errorAt(Token* token, const char* message)
{
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR) {
        // Nothing.
    }
    else {
        /**
         * '%.*s' is a format specifier for a string.
         * The '.*' indicates that the width of the string will be determined dynamically
         * by the argument that follows it. In this case, 'token.length' determines the width of the string.
         * 'token.start' is the pointer to the start of the string.
         */
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char* message)
{
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char* message)
{
    errorAt(&parser.current, message);
}

static void advance()
{
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanToken();
        if (parser.current.type != TOKEN_ERROR)
            break;

        errorAtCurrent(parser.current.start);
    }
}

/**
 * The call to advance() "primes the pump" on the scanner.
 * Then a single expression is parsed.
 * After the expression is compiled we should be at the end
 * of the source code, so we check for the EOF token.
 */
bool compile(const char* source, Chunk* chunk)
{
    initScanner(source);
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression");
    return !parser.hadError;
}