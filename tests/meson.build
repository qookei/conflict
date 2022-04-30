gtest_dep = dependency('gtest', main : true)

test_exe = executable('tests',
	'option.cpp',
	'choice.cpp',
	'string.cpp',
	'positional.cpp',
	dependencies : [ conflict_dep, gtest_dep ])

test('tests', test_exe)

example_exe = executable('example',
	'example.cpp', dependencies : [ conflict_dep ])
