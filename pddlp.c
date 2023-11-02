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

	[PDDLP_TOKEN_DECREASE] = "PDDLP_TOKEN_DECREASE",
	[PDDLP_TOKEN_EITHER] = "PDDLP_TOKEN_EITHER",
	[PDDLP_TOKEN_END] = "PDDLP_TOKEN_END",
	[PDDLP_TOKEN_EXISTS] = "PDDLP_TOKEN_EXISTS",
	[PDDLP_TOKEN_FORALL] = "PDDLP_TOKEN_FORALL",
	[PDDLP_TOKEN_NOT] = "PDDLP_TOKEN_NOT",
	[PDDLP_TOKEN_PREFERENCE] = "PDDLP_TOKEN_PREFERENCE",
	[PDDLP_TOKEN_TOTAL_TIME] = "PDDLP_TOKEN_TOTAL_TIME",

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
make_if_match(struct pddlp_tokenizer *t,
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
	token.column = t->column;

	return token;
}

static enum pddlp_token_type
check_name(struct pddlp_tokenizer *t, int start, int length,
	   const char *rest, enum pddlp_token_type token_type)
{
	if (t->current - t->start == start + length &&
	    memcmp(t->start + start, rest, length) == 0) {
		return token_type;
	}

	return PDDLP_TOKEN_NAME;
}

static enum pddlp_token_type
name_type(struct pddlp_tokenizer *t)
{
	// TODO(puida): list of language keywords to implement:
	// all
	// always
	// always-within
	// and
	// assign
	// at
	// at end (i don't know if this is correct, a hyphen would be more usual)
	// at-most-once
	// OK decrease
	// OK either
	// OK end
	// OK exists
	// OK forall
	// hold-after
	// hold-during
	// imply
	// increase
	// is-violated
	// maximize
	// minimize
	// OK not
	// object
	// or
	// over
	// OK preference
	// scale-up
	// sometime
	// sometime-after
	// sometime-before
	// start
	// OK total-time
	// when
	// within
	switch (t->start[0]) {
	case 'd': return check_name(t, 1, 7, "ecrease", PDDLP_TOKEN_DECREASE);
	case 'e':
		if (t->current - t->start > 1) {
			switch (t->start[1]) {
			case 'i': return check_name(t, 2, 4, "ther", PDDLP_TOKEN_EITHER);
			case 'n': return check_name(t, 2, 1, "d", PDDLP_TOKEN_END);
			case 'x': return check_name(t, 2, 4, "ists", PDDLP_TOKEN_EXISTS);
			}
		}
		break;
	case 'f': return check_name(t, 1, 5, "orall", PDDLP_TOKEN_FORALL);
	case 'n': return check_name(t, 1, 2, "ot", PDDLP_TOKEN_NOT);
	case 'p': return check_name(t, 1, 9, "reference", PDDLP_TOKEN_PREFERENCE);
	case 't': return check_name(t, 1, 9, "otal-time", PDDLP_TOKEN_TOTAL_TIME);
	}
	return PDDLP_TOKEN_NAME;
}

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
