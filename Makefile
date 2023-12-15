CFLAGS = -Wall -Wextra -pedantic -std=c99

.PHONY: all clean test

all: build/pddlp-tokenize build/pddlp.a

clean:
	rm -rf build

test: build/pddlp-tokenize run-tests.tcl
	./run-tests.tcl

build/%: %.c pddlp.c pddlp.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

# TODO: this is kinda hacky. The build system should
# probably be replaced with something more robust, like
# CMake or Meson.
build/pddlp.a: pddlp.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c pddlp.c -o build/pddlp.o
	ar rcs build/pddlp.a build/pddlp.o
