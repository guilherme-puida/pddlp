<!--
SPDX-FileCopyrightText: 2024 Guilherme Puida Moreira <guilherme@puida.xyz>
SPDX-License-Identifier: BSD-3-Clause
-->

# PDDLP

PDDLP aims to be a simple, but fast and correct, PDDL tokenizer and parser. It
is written in pure C99 without any external dependencies, and should compile
cleanly under any POSIX-compatible system.

The entire implementation is contained in `pddlp.c` and `pddlp.h`. The other files
in this repository are just used for testing and building.

Ideally, the library should be allocation-free. This makes the parser harder to
implement and use, but it shouldn't be a big problem. If this turns out to be
infeasible, I'll try to design an API that allocates very infrequently,
preferably with a user-supplied allocator.

The code is licensed under the BSD 3-Clause License. Please see `LICENSE` for the
entire text.

This project is in the initial stages of development.

## Tokenizer

The tokenizer is already quite fast, being able to chug through 6605750 tokens
in ~70ms. This is not a comprehensive benchmark, but it at least shows the
overall performance of the tokenizer.

```
$ ./build/pddlp-count-tokens pipesworld-propositional-strips-domain-44.pddl
tokens: 6605750
errors: 0

$ hyperfine './build/pddlp-count-tokens pipesworld-propositional-strips-domain-44.pddl'
Benchmark 1: ./build/pddlp-count-tokens pipesworld-propositional-strips-domain-44.pddl
  Time (mean ± σ):      68.2 ms ±   1.4 ms    [User: 60.1 ms, System: 8.0 ms]
  Range (min … max):    67.4 ms …  75.7 ms    42 runs
```

The "benchmark" input comes from the IPC 2006 competition. It is the current
biggest domain present in the [potassco/pddl-instances github repository]
([direct link to file]).

[postassco/pddl-instances github repository]: https://github.com/potassco/pddl-instances
[direct link to file]: https://github.com/potassco/pddl-instances/blob/master/ipc-2006/domains/pipesworld-propositional-strips/domains/domain-44.pddl

## Building

pddlp uses meson as a build system. Use it as you normally would:

```
meson setup build
meson compile -C build
```

## Testing

There is a basic test-suite implemented. To run the tests:

```
meson test -C build
```

Running tests require the criterion library to be installed. On Debian systems,
you can install it with `apt install libcriterion-dev`.

If you don't have criterion installed, meson will download it for you when you
try to setup the project.

If you want to skip building tests, set the `with_tests` option to false when
configuring meson.

```
meson setup -C build -Dwith_tests=false
```

## Limitations

Only the tokenizer is currently implemented, and it is case-sensitive. Because
of this case-sensitivity, some domains/problems from IPC competitions are not
tokenized correctly.

This case-sensitivity limitation is likely to disappear in the future.

## References

"Daniel L, Kovacs. BNF definition of PDDLP 3.1. 2011"
"Daniel L, Kovacs. Discussion of the original PDDL definitions and their correction. 2011"
