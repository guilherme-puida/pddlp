CFLAGS = -Wall -Wextra -pedantic -std=c99

.PHONY: all clean

all: build/pddlp-tokenize

clean:
	rm -rf build

build/%: %.c pddlp.c pddlp.h
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@
