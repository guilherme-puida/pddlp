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

#include <stdbool.h>
#include <string.h>

const char *pddlp_token_type_names[] = {
    [PDDLP_TOKEN_LPAREN] = "PDDLP_TOKEN_LPAREN",
    [PDDLP_TOKEN_RPAREN] = "PDDLP_TOKEN_RPAREN",
    [PDDLP_TOKEN_PLUS] = "PDDLP_TOKEN_PLUS",
    [PDDLP_TOKEN_MINUS] = "PDDLP_TOKEN_MINUS",
    [PDDLP_TOKEN_STAR] = "PDDLP_TOKEN_STAR",
    [PDDLP_TOKEN_SLASH] = "PDDLP_TOKEN_SLASH",
    [PDDLP_TOKEN_EQ] = "PDDLP_TOKEN_EQ",

    [PDDLP_TOKEN_LT] = "PDDLP_TOKEN_LT",
    [PDDLP_TOKEN_LTE] = "PDDLP_TOKEN_LTE",
    [PDDLP_TOKEN_GT] = "PDDLP_TOKEN_GT",
    [PDDLP_TOKEN_GTE] = "PDDLP_TOKEN_GTE",
    [PDDLP_TOKEN_HASH_T] = "PDDLP_TOKEN_HASH_T",

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
    [PDDLP_TOKEN_SYM_CONTINUOUS_EFFECTS] = "PDDLP_TOKEN_SYM_CONTINUOUS_EFFECTS",
    [PDDLP_TOKEN_SYM_DERIVED] = "PDDLP_TOKEN_SYM_DERIVED",
    [PDDLP_TOKEN_SYM_DERIVED_PREDICATES] = "PDDLP_TOKEN_SYM_DERIVED_PREDICATES",
    [PDDLP_TOKEN_SYM_DISJUNCTIVE_PRECONDITIONS] = "PDDLP_TOKEN_SYM_DISJUNCTIVE_PRECONDITIONS",
    [PDDLP_TOKEN_SYM_DOMAIN] = "PDDLP_TOKEN_SYM_DOMAIN",
    [PDDLP_TOKEN_SYM_DOMAIN_AXIOMS] = "PDDLP_TOKEN_SYM_DOMAIN_AXIOMS",
    [PDDLP_TOKEN_SYM_DURATION] = "PDDLP_TOKEN_SYM_DURATION",
    [PDDLP_TOKEN_SYM_DURATION_INEQUALITIES] = "PDDLP_TOKEN_SYM_DURATION_INEQUALITIES",
    [PDDLP_TOKEN_SYM_DURATIVE_ACTION] = "PDDLP_TOKEN_SYM_DURATIVE_ACTION",
    [PDDLP_TOKEN_SYM_DURATIVE_ACTIONS] = "PDDLP_TOKEN_SYM_DURATIVE_ACTIONS",
    [PDDLP_TOKEN_SYM_EFFECT] = "PDDLP_TOKEN_SYM_EFFECT",
    [PDDLP_TOKEN_SYM_EQUALITY] = "PDDLP_TOKEN_SYM_EQUALITY",
    [PDDLP_TOKEN_SYM_EXISTENTIAL_PRECONDITIONS] = "PDDLP_TOKEN_SYM_EXISTENTIAL_PRECONDITIONS",
    [PDDLP_TOKEN_SYM_FLUENTS] = "PDDLP_TOKEN_SYM_FLUENTS",
    [PDDLP_TOKEN_SYM_FUNCTIONS] = "PDDLP_TOKEN_SYM_FUNCTIONS",
    [PDDLP_TOKEN_SYM_GOAL] = "PDDLP_TOKEN_SYM_GOAL",
    [PDDLP_TOKEN_SYM_GOAL_UTILITIES] = "PDDLP_TOKEN_SYM_GOAL_UTILITIES",
    [PDDLP_TOKEN_SYM_INIT] = "PDDLP_TOKEN_SYM_INIT",
    [PDDLP_TOKEN_SYM_LENGTH] = "PDDLP_TOKEN_SYM_LENGHT",
    [PDDLP_TOKEN_SYM_METRIC] = "PDDLP_TOKEN_SYM_METRIC",
    [PDDLP_TOKEN_SYM_NEGATIVE_PRECONDITIONS] = "PDDLP_TOKEN_SYM_NEGATIVE_PRECONDITIONS",
    [PDDLP_TOKEN_SYM_NUMERIC_FLUENTS] = "PDDLP_TOKEN_SYM_NUMERIC_FLUENTS",
    [PDDLP_TOKEN_SYM_OBJECTS] = "PDDLP_TOKEN_SYM_OBJECTS",
    [PDDLP_TOKEN_SYM_PARALLEL] = "PDDLP_TOKEN_SYM_PARALLEL",
    [PDDLP_TOKEN_SYM_PARAMETERS] = "PDDLP_TOKEN_SYM_PARAMETERS",
    [PDDLP_TOKEN_SYM_PRECONDITION] = "PDDLP_TOKEN_SYM_PRECONDITION",
    [PDDLP_TOKEN_SYM_PREDICATES] = "PDDLP_TOKEN_SYM_PREDICATES",
    [PDDLP_TOKEN_SYM_PREFERENCES] = "PDDLP_TOKEN_SYM_PREFERENCES",
    [PDDLP_TOKEN_SYM_QUANTIFIED_PRECONDITIONS] = "PDDLP_TOKEN_SYM_QUANTIFIED_PRECONDITIONS",
    [PDDLP_TOKEN_SYM_REQUIREMENTS] = "PDDLP_TOKEN_SYM_REQUIREMENTS",
    [PDDLP_TOKEN_SYM_SERIAL] = "PDDLP_TOKEN_SYM_SERIAL",
    [PDDLP_TOKEN_SYM_STRIPS] = "PDDLP_TOKEN_SYM_STRIPS",
    [PDDLP_TOKEN_SYM_TIMED_INITIAL_LITERALS] = "PDDLP_TOKEN_SYM_TIMED_INITIAL_LITERALS",
    [PDDLP_TOKEN_SYM_TYPES] = "PDDLP_TOKEN_SYM_TYPES",
    [PDDLP_TOKEN_SYM_TYPING] = "PDDLP_TOKEN_SYM_TYPING",
    [PDDLP_TOKEN_SYM_UNIVERSAL_PRECONDITIONS] = "PDDLP_TOKEN_SYM_UNIVERSAL_PRECONDITIONS",
    [PDDLP_TOKEN_SYM_VARS] = "PDDLP_TOKEN_SYM_VARS",

    [PDDLP_TOKEN_EOF] = "PDDLP_TOKEN_EOF",
    [PDDLP_TOKEN_ERROR] = "PDDLP_TOKEN_ERROR",
};

static bool
tok_is_digit(char c)
{
    return '0' <= c && c <= '9';
}

static bool
tok_is_letter(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static bool
tok_is_any_char(char c)
{
    return tok_is_digit(c) || tok_is_letter(c) || c == '-' || c == '_';
}

static bool
tok_is_at_end(struct pddlp_tokenizer *t)
{
    return *t->current == 0;
}

static char
tok_advance(struct pddlp_tokenizer *t)
{
    t->current++;
    t->column++;
    return t->current[-1];
}

static char
tok_peek(struct pddlp_tokenizer *t)
{
    return *t->current;
}

static char
tok_peek_next(struct pddlp_tokenizer *t)
{
    return tok_is_at_end(t) ? 0 : t->current[1];
}

static bool
tok_match(struct pddlp_tokenizer *t, char expected)
{
    if (tok_is_at_end(t) || tok_peek(t) != expected)
        return false;

    tok_advance(t);
    return true;
}

static void
tok_skip_whitespace(struct pddlp_tokenizer *t)
{
    for (;;) {
        char c = tok_peek(t);

        if (c == ' ' || c == '\t' || c == '\r') {
            tok_advance(t);
        } else if (c == '\n') {
            t->line++;
            t->column = 0;
            tok_advance(t);
        } else if (c == ';') {
            while (tok_peek(t) != '\n' && !tok_is_at_end(t))
                tok_advance(t);
        } else {
            return;
        }

    }
}

static struct pddlp_token
tok_make_token(struct pddlp_tokenizer *t, enum pddlp_token_type token_type)
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
tok_make_if_match(
    struct pddlp_tokenizer *t,
    char expected,
    enum pddlp_token_type if_match,
    enum pddlp_token_type if_not_match)
{
    return tok_make_token(t, tok_match(t, expected) ? if_match : if_not_match);
}

static struct pddlp_token
tok_error_token(struct pddlp_tokenizer *t, const char *message)
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
do {                                                                    \
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
} while (0)

static enum pddlp_token_type
tok_name_type(struct pddlp_tokenizer *t)
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

#define __PDDLP_SYM(symbol, token)                                      \
do {                                                                    \
    int symbol_length = sizeof(symbol) - 1;                             \
    if (token_length == symbol_length && start[1] == symbol[1] &&       \
        (symbol_length <= 2 || start[2] == symbol[2]) &&                \
        (symbol_length <= 3 || start[3] == symbol[3]) &&                \
        (symbol_length <= 4 || start[4] == symbol[4]) &&                \
        (symbol_length <= 5 || start[5] == symbol[5]) &&                \
        (symbol_length <= 6 || start[6] == symbol[6]) &&                \
        (symbol_length <= 7 || start[7] == symbol[7]) &&                \
        (symbol_length <= 8 || start[8] == symbol[8]) &&                \
        (symbol_length <= 9 || start[9] == symbol[9]) &&                \
        (symbol_length <= 10 || start[10] == symbol[10]) &&             \
        (symbol_length <= 11 || start[11] == symbol[11]) &&             \
        (symbol_length <= 12 || start[12] == symbol[12]) &&             \
        (symbol_length <= 13 || start[13] == symbol[13]) &&             \
        (symbol_length <= 14 || start[14] == symbol[14]) &&             \
        (symbol_length <= 15 || start[15] == symbol[15]) &&             \
        (symbol_length <= 16 || start[16] == symbol[16]) &&             \
        (symbol_length <= 17 || start[17] == symbol[17]) &&             \
        (symbol_length <= 18 || start[18] == symbol[18]) &&             \
        (symbol_length <= 19 || start[19] == symbol[19]) &&             \
        (symbol_length <= 20 || start[20] == symbol[20]) &&             \
        (symbol_length <= 21 || start[21] == symbol[21]) &&             \
        (symbol_length <= 22 || start[22] == symbol[22]) &&             \
        (symbol_length <= 23 || start[23] == symbol[23]) &&             \
        (symbol_length <= 24 || start[24] == symbol[24]) &&             \
        (symbol_length <= 25 || start[25] == symbol[25])) return token; \
} while (0)

static enum pddlp_token_type
tok_symbol_type(struct pddlp_tokenizer *t)
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
        __PDDLP_SYM("continuous-effects", PDDLP_TOKEN_SYM_CONTINUOUS_EFFECTS);
        break;
    case 'd':
        __PDDLP_SYM("derived", PDDLP_TOKEN_SYM_DERIVED);
        __PDDLP_SYM("derived-predicates", PDDLP_TOKEN_SYM_DERIVED_PREDICATES);
        __PDDLP_SYM("disjunctive-preconditions", PDDLP_TOKEN_SYM_DISJUNCTIVE_PRECONDITIONS);
        __PDDLP_SYM("domain", PDDLP_TOKEN_SYM_DOMAIN);
        __PDDLP_SYM("domain-axioms", PDDLP_TOKEN_SYM_DOMAIN_AXIOMS);
        __PDDLP_SYM("duration", PDDLP_TOKEN_SYM_DURATION);
        __PDDLP_SYM("duration-inequalities", PDDLP_TOKEN_SYM_DURATION_INEQUALITIES);
        __PDDLP_SYM("durative-action", PDDLP_TOKEN_SYM_DURATIVE_ACTION);
        __PDDLP_SYM("durative-actions", PDDLP_TOKEN_SYM_DURATIVE_ACTIONS);
        break;
    case 'e':
        __PDDLP_SYM("effect", PDDLP_TOKEN_SYM_EFFECT);
        __PDDLP_SYM("equality", PDDLP_TOKEN_SYM_EQUALITY);
        __PDDLP_SYM("existential-preconditions", PDDLP_TOKEN_SYM_EXISTENTIAL_PRECONDITIONS);
        break;
    case 'f':
        __PDDLP_SYM("fluents", PDDLP_TOKEN_SYM_FLUENTS);
        __PDDLP_SYM("functions", PDDLP_TOKEN_SYM_FUNCTIONS);
        break;
    case 'g':
        __PDDLP_SYM("goal", PDDLP_TOKEN_SYM_GOAL);
        __PDDLP_SYM("goal-utilities", PDDLP_TOKEN_SYM_GOAL_UTILITIES);
        break;
    case 'i':
        __PDDLP_SYM("init", PDDLP_TOKEN_SYM_INIT);
        break;
    case 'l':
        __PDDLP_SYM("length", PDDLP_TOKEN_SYM_LENGTH);
        break;
    case 'm':
        __PDDLP_SYM("metric", PDDLP_TOKEN_SYM_METRIC);
        break;
    case 'n':
        __PDDLP_SYM("negative-preconditions", PDDLP_TOKEN_SYM_NEGATIVE_PRECONDITIONS);
        __PDDLP_SYM("numeric-fluents", PDDLP_TOKEN_SYM_NUMERIC_FLUENTS);
        break;
    case 'o':
        __PDDLP_SYM("objects", PDDLP_TOKEN_SYM_OBJECTS);
        break;
    case 'p':
        __PDDLP_SYM("parallel", PDDLP_TOKEN_SYM_PARALLEL);
        __PDDLP_SYM("parameters", PDDLP_TOKEN_SYM_PARAMETERS);
        __PDDLP_SYM("precondition", PDDLP_TOKEN_SYM_PRECONDITION);
        __PDDLP_SYM("predicates", PDDLP_TOKEN_SYM_PREDICATES);
        __PDDLP_SYM("preferences", PDDLP_TOKEN_SYM_PREFERENCES);
        break;
    case 'q':
        __PDDLP_SYM("quantified-preconditions", PDDLP_TOKEN_SYM_QUANTIFIED_PRECONDITIONS);
        break;
    case 'r':
        __PDDLP_SYM("requirements", PDDLP_TOKEN_SYM_REQUIREMENTS);
        break;
    case 's':
        __PDDLP_SYM("serial", PDDLP_TOKEN_SYM_SERIAL);
        __PDDLP_SYM("strips", PDDLP_TOKEN_SYM_STRIPS);
        break;
    case 't':
        __PDDLP_SYM("timed-initial-literals", PDDLP_TOKEN_SYM_TIMED_INITIAL_LITERALS);
        __PDDLP_SYM("types", PDDLP_TOKEN_SYM_TYPES);
        __PDDLP_SYM("typing", PDDLP_TOKEN_SYM_TYPING);
        break;
    case 'u':
        __PDDLP_SYM("universal-preconditions", PDDLP_TOKEN_SYM_UNIVERSAL_PRECONDITIONS);
        break;
    case 'v':
        __PDDLP_SYM("vars", PDDLP_TOKEN_SYM_VARS);
        break;
    }
#pragma GCC diagnostic pop

    return PDDLP_TOKEN_ERROR;
}

#undef __PDDLP_SYM

static struct pddlp_token
tok_tokenize_number(struct pddlp_tokenizer *t)
{
    while (tok_is_digit(tok_peek(t)))
        tok_advance(t);

    if (tok_peek(t) == '.' && tok_is_digit(tok_peek_next(t))) {
        tok_advance(t);

        while (tok_is_digit(tok_peek(t)))
            tok_advance(t);
    }

    return tok_make_token(t, PDDLP_TOKEN_NUMBER);
}

static struct pddlp_token
tok_tokenize_name(struct pddlp_tokenizer *t)
{
    while (tok_is_any_char(tok_peek(t))) tok_advance(t);

    return tok_make_token(t, tok_name_type(t));
}

static struct pddlp_token
tok_tokenize_symbol(struct pddlp_tokenizer *t)
{
    while (tok_is_any_char(tok_peek(t))) tok_advance(t);

    enum pddlp_token_type token_type = tok_symbol_type(t);
    if (token_type == PDDLP_TOKEN_ERROR)
        return tok_error_token(t, "unknown symbol");

    return tok_make_token(t, token_type);
}

static struct pddlp_token
tok_tokenize_variable(struct pddlp_tokenizer *t)
{
    int should_error = !tok_is_letter(tok_peek(t));

    tok_advance(t);

    while (tok_is_any_char(tok_peek(t))) tok_advance(t);

    if (should_error)
        return tok_error_token(t, "first character of a variable should be a letter");

    return tok_make_token(t, PDDLP_TOKEN_VARIABLE);
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
    tok_skip_whitespace(t);
    t->start = t->current;

    if (tok_is_at_end(t))
        return tok_make_token(t, PDDLP_TOKEN_EOF);

    char c = tok_advance(t);

    if (tok_is_digit(c)) return tok_tokenize_number(t);
    if (tok_is_letter(c)) return tok_tokenize_name(t);

    switch(c) {
    case ':': return tok_tokenize_symbol(t);
    case '?': return tok_tokenize_variable(t);
    case '(': return tok_make_token(t, PDDLP_TOKEN_LPAREN);
    case ')': return tok_make_token(t, PDDLP_TOKEN_RPAREN);
    case '+': return tok_make_token(t, PDDLP_TOKEN_PLUS);
    case '-': return tok_make_token(t, PDDLP_TOKEN_MINUS);
    case '*': return tok_make_token(t, PDDLP_TOKEN_STAR);
    case '/': return tok_make_token(t, PDDLP_TOKEN_SLASH);
    case '=': return tok_make_token(t, PDDLP_TOKEN_EQ);
    case '<': return tok_make_if_match(t, '=', PDDLP_TOKEN_LTE, PDDLP_TOKEN_LT);
    case '>': return tok_make_if_match(t, '=', PDDLP_TOKEN_GTE, PDDLP_TOKEN_GT);
    case '#': if (tok_match(t, 't')) return tok_make_token(t, PDDLP_TOKEN_HASH_T);
    }

    return tok_error_token(t, "unrecognized character");
}
