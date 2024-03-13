#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

void compile(const char* source) {
    initScanner(source);
    int line = -1;
    for(;;) {
        Token token = scanToken();
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        /**
         * '%.*s' is a format specifier for a string.
         * The '.*' indicates that the width of the string will be determined dynamically
         * by the argument that follows it. In this case, 'token.length' determines the width of the string.
         * 'token.start' is the pointer to the start of the string.
        */
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) break;
    }
}