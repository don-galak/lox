#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

/**
 * The call to advance() "primes the pump" on the scanner.
 * Then a single expression is parsed.
 * After the expression is compiled we should be at the end 
 * of the source code, so we check for the EOF token.
 */
bool compile(const char *source, Chunk *chunk)
{
    initScanner(source);
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression");
}