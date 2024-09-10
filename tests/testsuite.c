// SPDX-FileCopyrightText: 2024 Guilherme Puida Moreira <guilherme@puida.xyz>
// SPDX-License-Identifier: BSD-3-Clause

#include "pddlp.h"

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <string.h>

#define LEN(x) (sizeof(x)/sizeof(*(x)))

static void
token_eq(struct pddlp_token expected, struct pddlp_token got)
{
    cr_expect(eq(int, expected.token_type, got.token_type),
        "expected token_type to be %s, got %s",
        pddlp_token_type_names[expected.token_type],
        pddlp_token_type_names[got.token_type]);

    cr_expect(eq(int, expected.line, got.line),
        "expected line to be %d, got %d",
        expected.line, got.line);

    cr_expect(eq(int, expected.column, got.column),
        "expected column to be %d, got %d",
        expected.column, got.column);

    cr_expect(eq(int, expected.length, got.length),
        "expected length to be %d, got %d",
        expected.length, got.length);

    cr_expect(eq(int, strncmp(expected.start, got.start, expected.length), 0),
        "expected content to be '%.*s', got '%.*s'",
        expected.length, expected.start,
        got.length, got.start);
}

static void
expect_list(struct pddlp_tokenizer *tok, struct pddlp_token *expected, int expected_len)
{
    for (int i = 0; i < expected_len; ++i) {
        struct pddlp_token got = pddlp_scan_token(tok);
        token_eq(expected[i], got);
    }

    struct pddlp_token got = pddlp_scan_token(tok);
    cr_assert(eq(int, got.token_type, PDDLP_TOKEN_EOF));
}

#define mktoken(t, s, l, c) \
    { .token_type = t, .start = s, .length = sizeof(s) - 1, .line = l, .column = c }

Test(tokenizer, init) {
    struct pddlp_tokenizer tokenizer;
    pddlp_init_tokenizer(&tokenizer, "");

    cr_expect(eq(int, tokenizer.line, 1));
    cr_expect(eq(int, tokenizer.column, 1));
    cr_expect(eq(tokenizer.start, tokenizer.current));

    struct pddlp_token expected = mktoken(PDDLP_TOKEN_EOF, "", 1, 1);
    struct pddlp_token got = pddlp_scan_token(&tokenizer);
    token_eq(expected, got);

    got = pddlp_scan_token(&tokenizer);
    token_eq(expected, got);
}

Test(tokenizer, single_characters) {
    struct pddlp_tokenizer tokenizer;
    const char *source = "()+-/*=\\";

    pddlp_init_tokenizer(&tokenizer, source);

    struct pddlp_token expected[] = {
        mktoken(PDDLP_TOKEN_LPAREN, "(", 1, 1),
        mktoken(PDDLP_TOKEN_RPAREN, ")", 1, 2),
        mktoken(PDDLP_TOKEN_PLUS, "+", 1, 3),
        mktoken(PDDLP_TOKEN_MINUS, "-", 1, 4),
        mktoken(PDDLP_TOKEN_SLASH, "/", 1, 5),
        mktoken(PDDLP_TOKEN_STAR, "*", 1, 6),
        mktoken(PDDLP_TOKEN_EQ, "=", 1, 7),
        mktoken(PDDLP_TOKEN_ERROR, "unrecognized character", 1, 8),
    };

    expect_list(&tokenizer, expected, LEN(expected));
}

Test(tokenizer, double_characters) {
    struct pddlp_tokenizer tokenizer;
    const char *source =
        ">\n"
        ">=\n"
        "<\n"
        "<=\n"
        ">>=<<=\n"
        "#t";

    pddlp_init_tokenizer(&tokenizer, source);

    struct pddlp_token expected[] = {
        mktoken(PDDLP_TOKEN_GT, ">", 1, 1),
        mktoken(PDDLP_TOKEN_GTE, ">=", 2, 1),
        mktoken(PDDLP_TOKEN_LT, "<", 3, 1),
        mktoken(PDDLP_TOKEN_LTE, "<=", 4, 1),
        mktoken(PDDLP_TOKEN_GT, ">", 5, 1),
        mktoken(PDDLP_TOKEN_GTE, ">=", 5, 2),
        mktoken(PDDLP_TOKEN_LT, "<", 5, 4),
        mktoken(PDDLP_TOKEN_LTE, "<=", 5, 5),
        mktoken(PDDLP_TOKEN_HASH_T, "#t", 6, 1),
    };

    expect_list(&tokenizer, expected, LEN(expected));
}

Test(tokenizer, name) {
    struct pddlp_tokenizer tokenizer;
    const char *source =
        "name\n"
        "name-hyphens\n"
        "name_underscores\n"
        "namedigit1\n"
        "name-hypens-digit-10\n"
        "name_underscores_digit12\n"
        "name_with-all-3_elements\n";

    pddlp_init_tokenizer(&tokenizer, source);

    struct pddlp_token expected[] = {
        mktoken(PDDLP_TOKEN_NAME, "name", 1, 1),
        mktoken(PDDLP_TOKEN_NAME, "name-hyphens", 2, 1),
        mktoken(PDDLP_TOKEN_NAME, "name_underscores", 3, 1),
        mktoken(PDDLP_TOKEN_NAME, "namedigit1", 4, 1),
        mktoken(PDDLP_TOKEN_NAME, "name-hypens-digit-10", 5, 1),
        mktoken(PDDLP_TOKEN_NAME, "name_underscores_digit12", 6, 1),
        mktoken(PDDLP_TOKEN_NAME, "name_with-all-3_elements", 7, 1),
    };

    expect_list(&tokenizer, expected, LEN(expected));
}

Test(tokenizer, number) {
    struct pddlp_tokenizer tokenizer;
    const char *source =
        "0\n"
        "10\n"
        "1.0\n"
        "12.923\n"
        "10 11 12\n";

    pddlp_init_tokenizer(&tokenizer, source);

    struct pddlp_token expected[] = {
        mktoken(PDDLP_TOKEN_NUMBER, "0", 1, 1),
        mktoken(PDDLP_TOKEN_NUMBER, "10", 2, 1),
        mktoken(PDDLP_TOKEN_NUMBER, "1.0", 3, 1),
        mktoken(PDDLP_TOKEN_NUMBER, "12.923", 4, 1),
        mktoken(PDDLP_TOKEN_NUMBER, "10", 5, 1),
        mktoken(PDDLP_TOKEN_NUMBER, "11", 5, 4),
        mktoken(PDDLP_TOKEN_NUMBER, "12", 5, 7),
    };

    expect_list(&tokenizer, expected, LEN(expected));
}

Test(tokenizer, keywords) {
    struct pddlp_tokenizer tokenizer;
    const char *source =
        "all\n"
        "always\n"
        "always-within\n"
        "and\n"
        "assign\n"
        "at\n"
        "at-most-once\n"
        "decrease\n"
        "define\n"
        "domain\n"
        "either\n"
        "end\n"
        "exists\n"
        "forall\n"
        "hold-after\n"
        "hold-during\n"
        "imply\n"
        "increase\n"
        "is-violated\n"
        "maximize\n"
        "minimize\n"
        "not\n"
        "object\n"
        "or\n"
        "over\n"
        "preference\n"
        "problem\n"
        "scale-up\n"
        "sometime\n"
        "sometime-after\n"
        "sometime-before\n"
        "start\n"
        "total-time\n"
        "undefined\n"
        "when\n"
        "within\n";

    pddlp_init_tokenizer(&tokenizer, source);

    struct pddlp_token expected[] = {
        mktoken(PDDLP_TOKEN_ALL, "all", 1, 1),
        mktoken(PDDLP_TOKEN_ALWAYS, "always", 2, 1),
        mktoken(PDDLP_TOKEN_ALWAYS_WITHIN, "always-within", 3, 1),
        mktoken(PDDLP_TOKEN_AND, "and", 4, 1),
        mktoken(PDDLP_TOKEN_ASSIGN, "assign", 5, 1),
        mktoken(PDDLP_TOKEN_AT, "at", 6, 1),
        mktoken(PDDLP_TOKEN_AT_MOST_ONCE, "at-most-once", 7, 1),
        mktoken(PDDLP_TOKEN_DECREASE, "decrease", 8, 1),
        mktoken(PDDLP_TOKEN_DEFINE, "define", 9, 1),
        mktoken(PDDLP_TOKEN_DOMAIN, "domain", 10, 1),
        mktoken(PDDLP_TOKEN_EITHER, "either", 11, 1),
        mktoken(PDDLP_TOKEN_END, "end", 12, 1),
        mktoken(PDDLP_TOKEN_EXISTS, "exists", 13, 1),
        mktoken(PDDLP_TOKEN_FORALL, "forall", 14, 1),
        mktoken(PDDLP_TOKEN_HOLD_AFTER, "hold-after", 15, 1),
        mktoken(PDDLP_TOKEN_HOLD_DURING, "hold-during", 16, 1),
        mktoken(PDDLP_TOKEN_IMPLY, "imply", 17, 1),
        mktoken(PDDLP_TOKEN_INCREASE, "increase", 18, 1),
        mktoken(PDDLP_TOKEN_IS_VIOLATED, "is-violated", 19, 1),
        mktoken(PDDLP_TOKEN_MAXIMIZE, "maximize", 20, 1),
        mktoken(PDDLP_TOKEN_MINIMIZE, "minimize", 21, 1),
        mktoken(PDDLP_TOKEN_NOT, "not", 22, 1),
        mktoken(PDDLP_TOKEN_OBJECT, "object", 23, 1),
        mktoken(PDDLP_TOKEN_OR, "or", 24, 1),
        mktoken(PDDLP_TOKEN_OVER, "over", 25, 1),
        mktoken(PDDLP_TOKEN_PREFERENCE, "preference", 26, 1),
        mktoken(PDDLP_TOKEN_PROBLEM, "problem", 27, 1),
        mktoken(PDDLP_TOKEN_SCALE_UP, "scale-up", 28, 1),
        mktoken(PDDLP_TOKEN_SOMETIME, "sometime", 29, 1),
        mktoken(PDDLP_TOKEN_SOMETIME_AFTER, "sometime-after", 30, 1),
        mktoken(PDDLP_TOKEN_SOMETIME_BEFORE, "sometime-before", 31, 1),
        mktoken(PDDLP_TOKEN_START, "start", 32, 1),
        mktoken(PDDLP_TOKEN_TOTAL_TIME, "total-time", 33, 1),
        mktoken(PDDLP_TOKEN_UNDEFINED, "undefined", 34, 1),
        mktoken(PDDLP_TOKEN_WHEN, "when", 35, 1),
        mktoken(PDDLP_TOKEN_WITHIN, "within", 36, 1),
    };

    expect_list(&tokenizer, expected, LEN(expected));
}

Test(tokenizer, symbols) {
    struct pddlp_tokenizer tokenizer;
    const char *source =
        ":action\n"
        ":action-costs\n"
        ":adl\n"
        ":condition\n"
        ":conditional-effects\n"
        ":constants\n"
        ":constraints\n"
        ":continuous-effects\n"
        ":derived\n"
        ":derived-predicates\n"
        ":disjunctive-preconditions\n"
        ":domain\n"
        ":domain-axioms ; logistics-round-1-adl\n"
        ":duration\n"
        ":duration-inequalities\n"
        ":durative-action\n"
        ":durative-actions\n"
        ":effect\n"
        ":equality\n"
        ":existential-preconditions\n"
        ":fluents\n"
        ":functions\n"
        ":goal\n"
        ":goal-utilities ; woodworking-net-benefit-optimal-numeric-fluents\n"
        ":init\n"
        ":length\n"
        ":metric\n"
        ":negative-preconditions\n"
        ":numeric-fluents\n"
        ":objects\n"
        ":parallel\n"
        ":parameters\n"
        ":precondition\n"
        ":predicates\n"
        ":preferences\n"
        ":quantified-preconditions\n"
        ":requirements\n"
        ":serial\n"
        ":strips\n"
        ":timed-initial-literals\n"
        ":types\n"
        ":typing\n"
        ":universal-preconditions\n"
        ":vars ; mystery-prime-round-1-adl\n"
        ":sm\n"
        ":invalid\n"
        ":super-big-invalid-symbol-is-super-long\n";


    pddlp_init_tokenizer(&tokenizer, source);

    struct pddlp_token expected[] = {
        mktoken(PDDLP_TOKEN_SYM_ACTION, ":action", 1, 1),
        mktoken(PDDLP_TOKEN_SYM_ACTION_COSTS, ":action-costs", 2, 1),
        mktoken(PDDLP_TOKEN_SYM_ADL, ":adl", 3, 1),
        mktoken(PDDLP_TOKEN_SYM_CONDITION, ":condition", 4, 1),
        mktoken(PDDLP_TOKEN_SYM_CONDITIONAL_EFFECTS, ":conditional-effects", 5, 1),
        mktoken(PDDLP_TOKEN_SYM_CONSTANTS, ":constants", 6, 1),
        mktoken(PDDLP_TOKEN_SYM_CONSTRAINTS, ":constraints", 7, 1),
        mktoken(PDDLP_TOKEN_SYM_CONTINUOUS_EFFECTS, ":continuous-effects", 8, 1),
        mktoken(PDDLP_TOKEN_SYM_DERIVED, ":derived", 9, 1),
        mktoken(PDDLP_TOKEN_SYM_DERIVED_PREDICATES, ":derived-predicates", 10, 1),
        mktoken(PDDLP_TOKEN_SYM_DISJUNCTIVE_PRECONDITIONS, ":disjunctive-preconditions", 11, 1),
        mktoken(PDDLP_TOKEN_SYM_DOMAIN, ":domain", 12, 1),
        mktoken(PDDLP_TOKEN_SYM_DOMAIN_AXIOMS, ":domain-axioms", 13, 1),
        mktoken(PDDLP_TOKEN_SYM_DURATION, ":duration", 14, 1),
        mktoken(PDDLP_TOKEN_SYM_DURATION_INEQUALITIES, ":duration-inequalities", 15, 1),
        mktoken(PDDLP_TOKEN_SYM_DURATIVE_ACTION, ":durative-action", 16, 1),
        mktoken(PDDLP_TOKEN_SYM_DURATIVE_ACTIONS, ":durative-actions", 17, 1),
        mktoken(PDDLP_TOKEN_SYM_EFFECT, ":effect", 18, 1),
        mktoken(PDDLP_TOKEN_SYM_EQUALITY, ":equality", 19, 1),
        mktoken(PDDLP_TOKEN_SYM_EXISTENTIAL_PRECONDITIONS, ":existential-preconditions", 20, 1),
        mktoken(PDDLP_TOKEN_SYM_FLUENTS, ":fluents", 21, 1),
        mktoken(PDDLP_TOKEN_SYM_FUNCTIONS, ":functions", 22, 1),
        mktoken(PDDLP_TOKEN_SYM_GOAL, ":goal", 23, 1),
        mktoken(PDDLP_TOKEN_SYM_GOAL_UTILITIES, ":goal-utilities", 24, 1),
        mktoken(PDDLP_TOKEN_SYM_INIT, ":init", 25, 1),
        mktoken(PDDLP_TOKEN_SYM_LENGTH, ":length", 26, 1),
        mktoken(PDDLP_TOKEN_SYM_METRIC, ":metric", 27, 1),
        mktoken(PDDLP_TOKEN_SYM_NEGATIVE_PRECONDITIONS, ":negative-preconditions", 28, 1),
        mktoken(PDDLP_TOKEN_SYM_NUMERIC_FLUENTS, ":numeric-fluents", 29, 1),
        mktoken(PDDLP_TOKEN_SYM_OBJECTS, ":objects", 30, 1),
        mktoken(PDDLP_TOKEN_SYM_PARALLEL, ":parallel", 31, 1),
        mktoken(PDDLP_TOKEN_SYM_PARAMETERS, ":parameters", 32, 1),
        mktoken(PDDLP_TOKEN_SYM_PRECONDITION, ":precondition", 33, 1),
        mktoken(PDDLP_TOKEN_SYM_PREDICATES, ":predicates", 34, 1),
        mktoken(PDDLP_TOKEN_SYM_PREFERENCES, ":preferences", 35, 1),
        mktoken(PDDLP_TOKEN_SYM_QUANTIFIED_PRECONDITIONS, ":quantified-preconditions", 36, 1),
        mktoken(PDDLP_TOKEN_SYM_REQUIREMENTS, ":requirements", 37, 1),
        mktoken(PDDLP_TOKEN_SYM_SERIAL, ":serial", 38, 1),
        mktoken(PDDLP_TOKEN_SYM_STRIPS, ":strips", 39, 1),
        mktoken(PDDLP_TOKEN_SYM_TIMED_INITIAL_LITERALS, ":timed-initial-literals", 40, 1),
        mktoken(PDDLP_TOKEN_SYM_TYPES, ":types", 41, 1),
        mktoken(PDDLP_TOKEN_SYM_TYPING, ":typing", 42, 1),
        mktoken(PDDLP_TOKEN_SYM_UNIVERSAL_PRECONDITIONS, ":universal-preconditions", 43, 1),
        mktoken(PDDLP_TOKEN_SYM_VARS, ":vars", 44, 1),
        mktoken(PDDLP_TOKEN_ERROR, "unknown symbol", 45, 1),
        mktoken(PDDLP_TOKEN_ERROR, "unknown symbol", 46, 1),
        mktoken(PDDLP_TOKEN_ERROR, "unknown symbol", 47, 1),
    };

    expect_list(&tokenizer, expected, LEN(expected));
}


Test(tokenizer, variable) {
    struct pddlp_tokenizer tokenizer;
    const char *source =
        "?var\n"
        "?var1\n"
        "?var-hy\n"
        "?var__\n"
        "?1 ?_ ?-\n";

    pddlp_init_tokenizer(&tokenizer, source);

    struct pddlp_token expected[] = {
        mktoken(PDDLP_TOKEN_VARIABLE, "?var", 01, 01),
        mktoken(PDDLP_TOKEN_VARIABLE, "?var1", 02, 01),
        mktoken(PDDLP_TOKEN_VARIABLE, "?var-hy", 03, 01),
        mktoken(PDDLP_TOKEN_VARIABLE, "?var__", 04, 01),
        mktoken(PDDLP_TOKEN_ERROR, "first character of a variable should be a letter", 05, 01),
        mktoken(PDDLP_TOKEN_ERROR, "first character of a variable should be a letter", 05, 04),
        mktoken(PDDLP_TOKEN_ERROR, "first character of a variable should be a letter", 05, 07),
    };

    expect_list(&tokenizer, expected, LEN(expected));
}
