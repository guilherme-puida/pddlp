/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2023, Guilherme Puida Moreira
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

#include <string.h>

const char *pddlp_token_type_names[] = {
    [PDDLP_TOKEN_LPAREN] = "PDDLP_TOKEN_LPAREN",
    [PDDLP_TOKEN_RPAREN] = "PDDLP_TOKEN_RPAREN",
    [PDDLP_TOKEN_PLUS] = "PDDLP_TOKEN_PLUS",
    [PDDLP_TOKEN_MINUS] = "PDDLP_TOKEN_MINUS",
    [PDDLP_TOKEN_STAR] = "PDDLP_TOKEN_STAR",
    [PDDLP_TOKEN_SLASH] = "PDDLP_TOKEN_SLASH",

    [PDDLP_TOKEN_LT] = "PDDLP_TOKEN_LT",
    [PDDLP_TOKEN_LTE] = "PDDLP_TOKEN_LTE",
    [PDDLP_TOKEN_GT] = "PDDLP_TOKEN_GT",
    [PDDLP_TOKEN_GTE] = "PDDLP_TOKEN_GTE",

    [PDDLP_TOKEN_NUMBER] = "PDDLP_TOKEN_NUMBER",
    [PDDLP_TOKEN_NAME] = "PDDLP_TOKEN_NAME",
    [PDDLP_TOKEN_VARIABLE] = "PDDLP_TOKEN_VARIABLE",

    [PDDLP_TOKEN_ALL] = "PDDLP_TOKEN_ALL",
    [PDDLP_TOKEN_ALWAYS] = "PDDLP_TOKEN_ALWAYS",
    [PDDLP_TOKEN_ALWAYS_WITHIN] = "PDDLP_TOKEN_ALWAYS_WITHIN",
    [PDDLP_TOKEN_AND] = "PDDLP_TOKEN_AND",
    [PDDLP_TOKEN_ASSIGN] = "PDDLP_TOKEN_ASSIGN",
    [PDDLP_TOKEN_DECREASE] = "PDDLP_TOKEN_DECREASE",
    [PDDLP_TOKEN_AT] = "PDDLP_TOKEN_AT",
    [PDDLP_TOKEN_AT_MOST_ONCE] = "PDDLP_TOKEN_AT_MOST_ONCE",
    [PDDLP_TOKEN_DEFINE] = "PDDLP_TOKEN_DEFINE",
    [PDDLP_TOKEN_DOMAIN] = "PDDLP_TOKEN_DOMAIN",
    [PDDLP_TOKEN_EITHER] = "PDDLP_TOKEN_EITHER",
    [PDDLP_TOKEN_END] = "PDDLP_TOKEN_END",
    [PDDLP_TOKEN_EXISTS] = "PDDLP_TOKEN_EXISTS",
    [PDDLP_TOKEN_FORALL] = "PDDLP_TOKEN_FORALL",
    [PDDLP_TOKEN_HOLD_AFTER] = "PDDLP_TOKEN_HOLD_AFTER",
    [PDDLP_TOKEN_HOLD_DURING] = "PDDLP_TOKEN_HOLD_DURING",
    [PDDLP_TOKEN_IMPLY] = "PDDLP_TOKEN_IMPLY",
    [PDDLP_TOKEN_INCREASE] = "PDDLP_TOKEN_INCREASE",
    [PDDLP_TOKEN_IS_VIOLATED] = "PDDLP_TOKEN_IS_VIOLATED",
    [PDDLP_TOKEN_MAXIMIZE] = "PDDLP_TOKEN_MAXIMIZE",
    [PDDLP_TOKEN_MINIMIZE] = "PDDLP_TOKEN_MINIMIZE",
    [PDDLP_TOKEN_NOT] = "PDDLP_TOKEN_NOT",
    [PDDLP_TOKEN_OBJECT] = "PDDLP_TOKEN_OBJECT",
    [PDDLP_TOKEN_OR] = "PDDLP_TOKEN_OR",
    [PDDLP_TOKEN_OVER] = "PDDLP_TOKEN_OVER",
    [PDDLP_TOKEN_PREFERENCE] = "PDDLP_TOKEN_PREFERENCE",
    [PDDLP_TOKEN_PROBLEM] = "PDDLP_TOKEN_PROBLEM",
    [PDDLP_TOKEN_SCALE_UP] = "PDDLP_TOKEN_SCALE_UP",
    [PDDLP_TOKEN_START] = "PDDLP_TOKEN_START",
    [PDDLP_TOKEN_SOMETIME] = "PDDLP_TOKEN_SOMETIME",
    [PDDLP_TOKEN_SOMETIME_AFTER] = "PDDLP_TOKEN_SOMETIME_AFTER",
    [PDDLP_TOKEN_SOMETIME_BEFORE] = "PDDLP_TOKEN_SOMETIME_BEFORE",
    [PDDLP_TOKEN_TOTAL_TIME] = "PDDLP_TOKEN_TOTAL_TIME",
    [PDDLP_TOKEN_UNDEFINED] = "PDDLP_TOKEN_UNDEFINED",
    [PDDLP_TOKEN_WHEN] = "PDDLP_TOKEN_WHEN",
    [PDDLP_TOKEN_WITHIN] = "PDDLP_TOKEN_WITHIN",

    [PDDLP_TOKEN_SYM_ACTION] = "PDDLP_TOKEN_SYM_ACTION",
    [PDDLP_TOKEN_SYM_ACTION_COSTS] = "PDDLP_TOKEN_SYM_ACTION_COSTS",
    [PDDLP_TOKEN_SYM_ADL] = "PDDLP_TOKEN_SYM_ADL",
    [PDDLP_TOKEN_SYM_CONDITION] = "PDDLP_TOKEN_SYM_CONDITION",
    [PDDLP_TOKEN_SYM_CONDITIONAL_EFFECTS] = "PDDLP_TOKEN_SYM_CONDITIONAL_EFFECTS",
    [PDDLP_TOKEN_SYM_CONSTANTS] = "PDDLP_TOKEN_SYM_CONSTANTS",
    [PDDLP_TOKEN_SYM_CONSTRAINTS] = "PDDLP_TOKEN_SYM_CONSTRAINTS",

    [PDDLP_TOKEN_EOF] = "PDDLP_TOKEN_EOF",
    [PDDLP_TOKEN_ERROR] = "PDDLP_TOKEN_ERROR",
};

static int
is_digit(char c)
{
    return '0' <= c && c <= '9';
}

static int
is_letter(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static int
is_any_char(char c)
{
    return is_digit(c) || is_letter(c) || c == '-' || c == '_';
}

static int
is_at_end(struct pddlp_tokenizer *t)
{
    return *t->current == 0;
}

static char
advance(struct pddlp_tokenizer *t)
{
    t->current++;
    t->column++;
    return t->current[-1];
}

static char
peek(struct pddlp_tokenizer *t)
{
    return *t->current;
}

static char
peek_next(struct pddlp_tokenizer *t)
{
    return is_at_end(t) ? 0 : t->current[1];
}

static int
match(struct pddlp_tokenizer *t, char expected)
{
    if (is_at_end(t) || peek(t) != expected)
        return 0;

    advance(t);
    return 1;
}

static void
skip_whitespace(struct pddlp_tokenizer *t)
{
    for (;;) {
        char c = peek(t);

        if (c == ' ' || c == '\t' || c == '\r') {
            advance(t);
        } else if (c == '\n') {
            t->line++;
            t->column = 0;
            advance(t);
        } else if (c == ';') {
            while (peek(t) != '\n' && !is_at_end(t))
                advance(t);
        } else {
            return;
        }

    }
}

static struct pddlp_token
make_token(struct pddlp_tokenizer *t, enum pddlp_token_type token_type)
{
    struct pddlp_token token;

    token.token_type = token_type;
    token.start = t->start;
    token.length = t->current - t->start;
    token.line = t->line;
    token.column = t->column - token.length;

    return token;
}

static struct pddlp_token
make_if_match(
    struct pddlp_tokenizer *t,
    char expected,
    enum pddlp_token_type if_match,
    enum pddlp_token_type if_not_match)
{
    return make_token(t, match(t, expected) ? if_match : if_not_match);
}

static struct pddlp_token
error_token(struct pddlp_tokenizer *t, const char *message)
{
    struct pddlp_token token;

    token.token_type = PDDLP_TOKEN_ERROR;
    token.start = message;
    token.length = strlen(message);
    token.line = t->line;
    token.column = t->column - (t->current - t->start);

    return token;
}

#define __PDDLP_NAME(name, token)                                       \
{                                                                       \
    int name_length = sizeof(name) - 1;                                 \
    if (token_length == name_length && t->start[1] == name[1] &&        \
        (name_length <= 2 || t->start[2] == name[2]) &&                 \
        (name_length <= 3 || t->start[3] == name[3]) &&                 \
        (name_length <= 4 || t->start[4] == name[4]) &&                 \
        (name_length <= 5 || t->start[5] == name[5]) &&                 \
        (name_length <= 6 || t->start[6] == name[6]) &&                 \
        (name_length <= 7 || t->start[7] == name[7]) &&                 \
        (name_length <= 8 || t->start[8] == name[8]) &&                 \
        (name_length <= 9 || t->start[9] == name[9]) &&                 \
        (name_length <= 10 || t->start[10] == name[10]) &&              \
        (name_length <= 11 || t->start[11] == name[11]) &&              \
        (name_length <= 12 || t->start[12] == name[12]) &&              \
        (name_length <= 13 || t->start[13] == name[13]) &&              \
        (name_length <= 14 || t->start[14] == name[14]) &&              \
        (name_length <= 15 || t->start[15] == name[15])) return token;  \
}

static enum pddlp_token_type
name_type(struct pddlp_tokenizer *t)
{

    int token_length = t->current - t->start;

    // user-defined names can be any length, but language
    // keywords are only >= 2 (at, or) and <= 15 (sometime-before).
    if (token_length < 2 || token_length > 15)
        return PDDLP_TOKEN_NAME;


    // NOTE: we already check the array bounds inside the __PDDLP_NAME macro,
    //       so we can safely disable the warning here.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    switch (t->start[0]) {
    case 'a':
        __PDDLP_NAME("all", PDDLP_TOKEN_ALL);
        __PDDLP_NAME("always", PDDLP_TOKEN_ALWAYS);
        __PDDLP_NAME("always-within", PDDLP_TOKEN_ALWAYS_WITHIN);
        __PDDLP_NAME("and", PDDLP_TOKEN_AND);
        __PDDLP_NAME("assign", PDDLP_TOKEN_ASSIGN);
        __PDDLP_NAME("at", PDDLP_TOKEN_AT);
        __PDDLP_NAME("at-most-once", PDDLP_TOKEN_AT_MOST_ONCE);
        break;
    case 'd':
        __PDDLP_NAME("decrease", PDDLP_TOKEN_DECREASE);
        __PDDLP_NAME("define", PDDLP_TOKEN_DEFINE);
        __PDDLP_NAME("domain", PDDLP_TOKEN_DOMAIN);
        break;
    case 'e':
        __PDDLP_NAME("either", PDDLP_TOKEN_EITHER);
        __PDDLP_NAME("end", PDDLP_TOKEN_END);
        __PDDLP_NAME("exists", PDDLP_TOKEN_EXISTS);
        break;
    case 'f':
        __PDDLP_NAME("forall", PDDLP_TOKEN_FORALL);
        break;
    case 'h':
        __PDDLP_NAME("hold-after", PDDLP_TOKEN_HOLD_AFTER);
        __PDDLP_NAME("hold-during", PDDLP_TOKEN_HOLD_DURING);
        break;
    case 'i':
        __PDDLP_NAME("imply", PDDLP_TOKEN_IMPLY);
        __PDDLP_NAME("increase", PDDLP_TOKEN_INCREASE);
        __PDDLP_NAME("is-violated", PDDLP_TOKEN_IS_VIOLATED);
        break;
    case 'm':
        __PDDLP_NAME("maximize", PDDLP_TOKEN_MAXIMIZE);
        __PDDLP_NAME("minimize", PDDLP_TOKEN_MINIMIZE);
        break;
    case 'n':
        __PDDLP_NAME("not", PDDLP_TOKEN_NOT);
        break;
    case 'o':
        __PDDLP_NAME("object", PDDLP_TOKEN_OBJECT);
        __PDDLP_NAME("or", PDDLP_TOKEN_OR);
        __PDDLP_NAME("over", PDDLP_TOKEN_OVER);
        break;
    case 'p':
        __PDDLP_NAME("preference", PDDLP_TOKEN_PREFERENCE);
        __PDDLP_NAME("problem", PDDLP_TOKEN_PROBLEM);
        break;
    case 's':
        __PDDLP_NAME("scale-up", PDDLP_TOKEN_SCALE_UP);
        __PDDLP_NAME("start", PDDLP_TOKEN_START);
        __PDDLP_NAME("sometime", PDDLP_TOKEN_SOMETIME);
        __PDDLP_NAME("sometime-after", PDDLP_TOKEN_SOMETIME_AFTER);
        __PDDLP_NAME("sometime-before", PDDLP_TOKEN_SOMETIME_BEFORE);
        break;
    case 't':
        __PDDLP_NAME("total-time", PDDLP_TOKEN_TOTAL_TIME);
        break;
    case 'u':
        __PDDLP_NAME("undefined", PDDLP_TOKEN_UNDEFINED);
        break;
    case 'w':
        __PDDLP_NAME("when", PDDLP_TOKEN_WHEN);
        __PDDLP_NAME("within", PDDLP_TOKEN_WITHIN);
        break;
    }
#pragma GCC diagnostic pop

    return PDDLP_TOKEN_NAME;
}

#undef __PDDLP_NAME

#define __PDDLP_SYM(symbol, token) \
{ \
    int symbol_length = sizeof(symbol) - 1; \
    if (token_length == symbol_length && start[1] == symbol[1] && \
        (symbol_length <= 2 || start[2] == symbol[2]) && \
        (symbol_length <= 3 || start[3] == symbol[3]) && \
        (symbol_length <= 4 || start[4] == symbol[4]) && \
        (symbol_length <= 5 || start[5] == symbol[5]) && \
        (symbol_length <= 6 || start[6] == symbol[6]) && \
        (symbol_length <= 7 || start[7] == symbol[7]) && \
        (symbol_length <= 8 || start[8] == symbol[8]) && \
        (symbol_length <= 9 || start[9] == symbol[9]) && \
        (symbol_length <= 10 || start[10] == symbol[10]) && \
        (symbol_length <= 11 || start[11] == symbol[11]) && \
        (symbol_length <= 12 || start[12] == symbol[12]) && \
        (symbol_length <= 13 || start[13] == symbol[13]) && \
        (symbol_length <= 14 || start[14] == symbol[14]) && \
        (symbol_length <= 15 || start[15] == symbol[15]) && \
        (symbol_length <= 16 || start[16] == symbol[16]) && \
        (symbol_length <= 17 || start[17] == symbol[17]) && \
        (symbol_length <= 18 || start[18] == symbol[18]) && \
        (symbol_length <= 19 || start[19] == symbol[19]) && \
        (symbol_length <= 20 || start[20] == symbol[20]) && \
        (symbol_length <= 21 || start[21] == symbol[21]) && \
        (symbol_length <= 22 || start[22] == symbol[22]) && \
        (symbol_length <= 23 || start[23] == symbol[23]) && \
        (symbol_length <= 24 || start[24] == symbol[24]) && \
        (symbol_length <= 25 || start[25] == symbol[25])) return token; \
}

static enum pddlp_token_type
symbol_type(struct pddlp_tokenizer *t)
{
    // we skip one character to avoid dealing with ':'
    const char *start = t->start + 1;

    int token_length = t->current - start;

    // symbols can have length of >= 3 (adl) and <= 25 (disjunctive-preconditions)
    if (token_length < 3 || token_length > 25)
        return PDDLP_TOKEN_ERROR;

    // NOTE: we already check the array bounds inside the __PDDLP_SYM macro,
    //       so we can safely disable the warning here.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
    switch (start[0]) {
    case 'a':
        __PDDLP_SYM("action", PDDLP_TOKEN_SYM_ACTION);
        __PDDLP_SYM("action-costs", PDDLP_TOKEN_SYM_ACTION_COSTS);
        __PDDLP_SYM("adl", PDDLP_TOKEN_SYM_ADL);
        break;
    case 'c':
        __PDDLP_SYM("condition", PDDLP_TOKEN_SYM_CONDITION);
        __PDDLP_SYM("conditional-effects", PDDLP_TOKEN_SYM_CONDITIONAL_EFFECTS);
        __PDDLP_SYM("constants", PDDLP_TOKEN_SYM_CONSTANTS);
        __PDDLP_SYM("constraints", PDDLP_TOKEN_SYM_CONSTRAINTS);
        break;
    }
#pragma GCC diagnostic pop

    return PDDLP_TOKEN_ERROR;
}

#undef __PDDLP_SYM

static struct pddlp_token
tokenize_number(struct pddlp_tokenizer *t)
{
    while (is_digit(peek(t)))
        advance(t);

    if (peek(t) == '.' && is_digit(peek_next(t))) {
        advance(t);

        while (is_digit(peek(t)))
            advance(t);
    }

    return make_token(t, PDDLP_TOKEN_NUMBER);
}

static struct pddlp_token
tokenize_name(struct pddlp_tokenizer *t)
{
    while (is_any_char(peek(t))) advance(t);

    return make_token(t, name_type(t));
}

static struct pddlp_token
tokenize_symbol(struct pddlp_tokenizer *t)
{
    while (is_any_char(peek(t))) advance(t);

    return make_token(t, symbol_type(t));
}

static struct pddlp_token
tokenize_variable(struct pddlp_tokenizer *t)
{
    int should_error = !is_letter(peek(t));

    advance(t);

    while (is_any_char(peek(t))) advance(t);

    if (should_error)
        return error_token(t, "first character of a variable should be a letter");

    return make_token(t, PDDLP_TOKEN_VARIABLE);
}

void
pddlp_init_tokenizer(struct pddlp_tokenizer *t, const char *source)
{
    t->start = source;
    t->current = source;
    t->line = 1;
    t->column = 1;
}

struct pddlp_token
pddlp_scan_token(struct pddlp_tokenizer *t)
{
    skip_whitespace(t);
    t->start = t->current;

    if (is_at_end(t))
        return make_token(t, PDDLP_TOKEN_EOF);

    char c = advance(t);

    if (c == ':') return tokenize_symbol(t);
    if (c == '?') return tokenize_variable(t);
    if (is_digit(c)) return tokenize_number(t);
    if (is_letter(c)) return tokenize_name(t);

    switch(c) {
    case '(': return make_token(t, PDDLP_TOKEN_LPAREN);
    case ')': return make_token(t, PDDLP_TOKEN_RPAREN);
    case '+': return make_token(t, PDDLP_TOKEN_PLUS);
    case '-': return make_token(t, PDDLP_TOKEN_MINUS);
    case '*': return make_token(t, PDDLP_TOKEN_STAR);
    case '/': return make_token(t, PDDLP_TOKEN_SLASH);
    case '<': return make_if_match(t, '=', PDDLP_TOKEN_LTE, PDDLP_TOKEN_LT);
    case '>': return make_if_match(t, '=', PDDLP_TOKEN_GTE, PDDLP_TOKEN_GT);
    }

    return error_token(t, "unrecognized character");
}
