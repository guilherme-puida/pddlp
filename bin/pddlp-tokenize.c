/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2023-2024 Guilherme Puida Moreira
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
