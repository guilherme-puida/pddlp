project('pddlp', 'c',
  version         : '0.1',
  license         : ['BSD-3-Clause'],
  default_options : 'c_std=c99',
)

pddlp_inc = include_directories('pddlp')
pddlp_src = files('pddlp/pddlp.c')

pddlp_lib = library('pddlp',
  pddlp_src,
  version : meson.project_version(),
)

pddlp_dep = declare_dependency(
  link_with           : pddlp_lib,
  include_directories : pddlp_inc,
  version             : meson.project_version(),
)

subdir('bin')
subdir('tests')

install_headers('pddlp/pddlp.h')

pkg = import('pkgconfig')
pkg.generate(pddlp_lib,
  description : 'pddl parser and tokenizer',
)
