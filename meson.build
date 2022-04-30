project('conflict', 'cpp', version : '0.1', license : 'zlib', default_options : [ 'cpp_std=c++17' ])

conflict_inc = include_directories('include')
conflict_dep = declare_dependency(include_directories : conflict_inc)

if get_option('install_headers')
	install_headers(
		'include/conflict/conflict.hpp',
		'include/conflict/info.hpp',
		'include/conflict/error.hpp',
		'include/conflict/choice.hpp',
		'include/conflict/option.hpp',
		'include/conflict/string.hpp',
		'include/conflict/parser.hpp',
		'include/conflict/detail.hpp',
		subdir : 'conflict')
endif

