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
#ifndef PDDLP_H_
#define PDDLP_H_

enum pddlp_token_type {
    PDDLP_TOKEN_LPAREN,
    PDDLP_TOKEN_RPAREN,
    PDDLP_TOKEN_PLUS,
    PDDLP_TOKEN_MINUS,
    PDDLP_TOKEN_STAR,
    PDDLP_TOKEN_SLASH,

    PDDLP_TOKEN_LT,
    PDDLP_TOKEN_LTE,
    PDDLP_TOKEN_GT,
    PDDLP_TOKEN_GTE,

    PDDLP_TOKEN_NUMBER,
    PDDLP_TOKEN_NAME,
    PDDLP_TOKEN_VARIABLE,

    PDDLP_TOKEN_ALL,
    PDDLP_TOKEN_ALWAYS,
    PDDLP_TOKEN_ALWAYS_WITHIN,
    PDDLP_TOKEN_AND,
    PDDLP_TOKEN_ASSIGN,
    PDDLP_TOKEN_AT,
    PDDLP_TOKEN_AT_MOST_ONCE,
    PDDLP_TOKEN_DECREASE,
    PDDLP_TOKEN_DEFINE,
    PDDLP_TOKEN_DOMAIN,
    PDDLP_TOKEN_EITHER,
    PDDLP_TOKEN_END,
    PDDLP_TOKEN_EXISTS,
    PDDLP_TOKEN_FORALL,
    PDDLP_TOKEN_HOLD_AFTER,
    PDDLP_TOKEN_HOLD_DURING,
    PDDLP_TOKEN_IMPLY,
    PDDLP_TOKEN_INCREASE,
    PDDLP_TOKEN_IS_VIOLATED,
    PDDLP_TOKEN_MAXIMIZE,
    PDDLP_TOKEN_MINIMIZE,
    PDDLP_TOKEN_NOT,
    PDDLP_TOKEN_OBJECT,
    PDDLP_TOKEN_OR,
    PDDLP_TOKEN_OVER,
    PDDLP_TOKEN_PREFERENCE,
    PDDLP_TOKEN_PROBLEM,
    PDDLP_TOKEN_SCALE_UP,
    PDDLP_TOKEN_START,
    PDDLP_TOKEN_SOMETIME,
    PDDLP_TOKEN_SOMETIME_AFTER,
    PDDLP_TOKEN_SOMETIME_BEFORE,
    PDDLP_TOKEN_TOTAL_TIME,
    PDDLP_TOKEN_UNDEFINED,
    PDDLP_TOKEN_WHEN,
    PDDLP_TOKEN_WITHIN,

    PDDLP_TOKEN_SYM_ACTION,
    PDDLP_TOKEN_SYM_ACTION_COSTS,
    PDDLP_TOKEN_SYM_ADL,
    PDDLP_TOKEN_SYM_CONDITION,
    PDDLP_TOKEN_SYM_CONDITIONAL_EFFECTS,
    PDDLP_TOKEN_SYM_CONSTANTS,
    PDDLP_TOKEN_SYM_CONSTRAINTS,
    PDDLP_TOKEN_SYM_CONTINUOUS_EFFECTS,
    PDDLP_TOKEN_SYM_DERIVED,
    PDDLP_TOKEN_SYM_DERIVED_PREDICATES,
    PDDLP_TOKEN_SYM_DISJUNCTIVE_PRECONDITIONS,
    PDDLP_TOKEN_SYM_DOMAIN,
    PDDLP_TOKEN_SYM_DURATION,
    PDDLP_TOKEN_SYM_DURATION_INEQUALITIES,
    PDDLP_TOKEN_SYM_DURATIVE_ACTION,
    PDDLP_TOKEN_SYM_DURATIVE_ACTIONS,
    PDDLP_TOKEN_SYM_EFFECT,
    PDDLP_TOKEN_SYM_EQUALITY,
    PDDLP_TOKEN_SYM_EXISTENTIAL_PRECONDITIONS,
    PDDLP_TOKEN_SYM_FLUENTS,
    PDDLP_TOKEN_SYM_FUNCTIONS,
    PDDLP_TOKEN_SYM_GOAL,
    PDDLP_TOKEN_SYM_INIT,
    PDDLP_TOKEN_SYM_LENGTH,
    PDDLP_TOKEN_SYM_METRIC,

    PDDLP_TOKEN_EOF,
    PDDLP_TOKEN_ERROR,
};

extern const char *pddlp_token_type_names[];

struct pddlp_token {
    enum pddlp_token_type token_type;
    const char *start;
    int length;

    int line;
    int column;
};

struct pddlp_tokenizer {
    const char *start;
    const char *current;

    int line;
    int column;
};

void
pddlp_init_tokenizer(struct pddlp_tokenizer *, const char *source);

struct pddlp_token
pddlp_scan_token(struct pddlp_tokenizer *);

#endif // PDDLP_H_
