#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "src/include/lexer.h"

#define VERSION "1.0"
#define EXTENSION ".oldre"

typedef struct Help {
    const char *command;
    const char *description;
} Help;

const Help helplist[] = {
    {"help", "show help message"},
    {"version", "show newest version"}
};

void help() {
    printf("usage: oldre <options_or_file>\n");
    printf("Options:\n");

    size_t count = sizeof(helplist) / sizeof(helplist[0]);
    for(size_t i = 0;i < count;i++) {
        printf("    %s - %s\n", helplist[i].command, helplist[i].description);
    }
}

void version() {
    printf("Oldre v%s\n", VERSION);
}

int is_file(const char *file) {
    FILE *fptr = fopen(file, "r");
    if(!fptr) {
        printf("Failed to open file '%s'\n", file);
        return 1;
    } else {
        fclose(fptr);
    }

    struct stat path_stat;
    if(stat(file, &path_stat) != 0) return 1;

    if (!S_ISREG(path_stat.st_mode)) {
        printf("No such file! '%s'\n", file);
        return 1;
    }

    const char *dot = strrchr(file, '.');
    if(strcmp(dot, EXTENSION) != 0) {
        printf("Oldre extension must be %s", EXTENSION);
        return 1;
    }

    return 0;
}

char *read_the_file(const char *file) {
    FILE *fptr = fopen(file, "r");
    if(!fptr) return NULL;

    fseek(fptr, 0, SEEK_END);
    long length = ftell(fptr);
    rewind(fptr);

    if (length < 0) {
        fclose(fptr);
        return NULL;
    }

    char *buffer = malloc(length + 1);
    if(!buffer) {
        int err = errno;
        fprintf(stderr, "main.c:%d: read_the_file: malloc(%ld) failed for '%s': %s (errno=%d)\n",
                __LINE__, length + 1, file, strerror(err), err);
        fclose(fptr);
        return NULL;
    }

    size_t tell = fread(buffer, 1, length, fptr);
    buffer[tell] = '\0';

    fclose(fptr);
    return buffer;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        help();
        return 1;
    }

    char *cmd = argv[1];

    if (strcmp(cmd, "help") == 0) { help(); return 0; }
    if (strcmp(cmd, "version") == 0) { version(); return 0; }

    if (is_file(cmd) == 0) {
        char *read = read_the_file(cmd);
        if(!read) return 1;

        Lexer lexer = {
            .source = read,
            .cursor = 0
        };

        Token *token = tokenize(&lexer);

        while (token != NULL) {
            printf("%s | %s\n", type_name(token->type), token->value ? token->value : "");
            if (token->type == TYPE_EOF) break;
            free(token->value);
            free(token);
        }

        free(read);
    }

    return 0;
}
