# SPDX-FileCopyrightText: 2024 Guilherme Puida Moreira <guilherme@puida.xyz>
# SPDX-License-Identifier: BSD-3-Clause

criterion_dep = dependency('criterion')

test_deps = [pddlp_dep, criterion_dep]

unit_tests = executable('pddlp-test', 'testsuite.c',
  dependencies : test_deps,
)

test('unit_tests', unit_tests,
  args     : ['--tap'],
  protocol : 'tap',
)
