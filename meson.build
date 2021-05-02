project('cocktail_bar', 'c',
  version : '0.1',
  default_options : ['warning_level=3'])

dep = [dependency('libcyaml'), dependency('gtk+-3.0')]

executable('cocktail_bar',
           'cocktail_bar.c',
           dependencies: dep,
           install : true)
