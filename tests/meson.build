criterion_dep = dependency('criterion')

test_deps = [pddlp_dep, criterion_dep]

unit_tests = executable('pddlp-test', 'testsuite.c',
  dependencies : test_deps,
)

test('unit_tests', unit_tests,
  args     : ['--tap'],
  protocol : 'tap',
)
