// SPDX-FileCopyrightText: 2023 Guilherme Puida Moreira <guilherme@puida.xyz>
// SPDX-License-Identifier: BSD-3-Clause

#include "pddlp.h"

#include <stdio.h>
#include <stdlib.h>

struct count_tokens_result {
    int token_count;
    int error_count;
};

static struct count_tokens_result
count_tokens(const char *source)
{
struct pddlp_tokenizer tokenizer;
    pddlp_init_tokenizer(&tokenizer, source);

    struct count_tokens_result result = {
        .token_count = 0,
        .error_count = 0,
    };

    for (;;) {
        struct pddlp_token token = pddlp_scan_token(&tokenizer);
        enum pddlp_token_type token_type = token.token_type;

        if (token_type == PDDLP_TOKEN_EOF)
            break;

        if (token_type == PDDLP_TOKEN_ERROR)
            result.error_count++;

        result.token_count++;
    }

    return result;
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

    struct count_tokens_result result = count_tokens(source);
    printf("tokens: %d\nerrors: %d\n", result.token_count, result.error_count);
    free(source);
    return 0;
}
