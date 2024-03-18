#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

static void repl() {
    char line[1024];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        interpret(line);
    }
}

/**
 * We want to allocate a big enough string to read the whole file, but we don't
 * know how big the file is until we have read it.
 * 1. We open the file ("rb" means the file is in binary. Not sure why it's needed)
 * 2. We call fseek with 0l (long integer value with all the bits set to zero - generally the definition of 0)
 *    and SEEK_END to reach the end of the file.
 * 3. Then we call ftell, which tells us how many bytes we are from the start of the file.
 *    Since we seeked to the end, that's the size.
 * 4. We rewind back to the beginning.
 * 5. Allocate a string of fileSize.
 * 6. Read the whole file in a single batch.
 */
static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    // File doesn't exist, or user doesn't have access to it.
    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0l, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
    }

    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}

static void runFile(const char* path) {
    char* source = readFile(path);
    InterpretResult result = interpret(source);
    free(source);

    if (result == INTERPRET_COMPILE_ERROR)
        exit(65);
    if (result == INTERPRET_RUNTIME_ERROR)
        exit(70);
}

int main(int argc, const char* argv[]) {
    initVM();

    if (argc == 1) {
        repl();
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        printf(stderr, "Usage: clox [path]\n");
        exit(64);
    }

    freeVM();
    return 0;
}