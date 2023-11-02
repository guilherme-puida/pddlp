CFLAGS = -Wall -Wextra -pedantic -std=c99

.PHONY: all clean test

all: build/pddlp-tokenize

clean:
	rm -rf build

test: build/pddlp-tokenize run-tests.tcl
	./run-tests.tcl

build/%: %.c pddlp.c pddlp.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@
