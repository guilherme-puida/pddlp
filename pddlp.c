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
