// SPDX-FileCopyrightText: 2023 Guilherme Puida Moreira <guilherme@puida.xyz>
// SPDX-License-Identifier: BSD-3-Clause

#include "pddlp.h"

#include <stdio.h>
#include <stdlib.h>

static int
print_all_tokens(const char *source)
{
    struct pddlp_tokenizer tokenizer;
    pddlp_init_tokenizer(&tokenizer, source);

    int error_count = 0;

    for (;;) {
        struct pddlp_token token = pddlp_scan_token(&tokenizer);
        enum pddlp_token_type token_type = token.token_type;

        if (token_type == PDDLP_TOKEN_EOF) {
            printf("EOF\n");
            break;
        }

        if (token_type == PDDLP_TOKEN_ERROR)
            error_count++;

        printf("[%02d:%02d] %s %.*s\n",
            token.line, token.column,
            pddlp_token_type_names[token_type],
            token.length, token.start);
    }

    return error_count;
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return -1;
    }

    char *file_name = argv[1];

    FILE *file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "couldn't open %s\n", file_name);
        return -1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *source = malloc(sizeof(*source) * file_size + 1);
    long read_amount = fread(source, 1, file_size, file);
    source[read_amount] = 0;
    fclose(file);

    int error_count = print_all_tokens(source);
    if (error_count)
        printf("error count: %d\n", error_count);

    free(source);
    return 0;
}
