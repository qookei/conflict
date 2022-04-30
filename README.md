# Conflict

C++17 command-line argument parser with no extra dependencies.

## Features

 - Generating help messages based on available options,
 - Support for simple options, string options, and single and multiple choice options, with default value support,
 - Support for custom option types.

## Requirements
 - C++17 compiler,
 - Meson and Ninja (for installing and running tests).

## Using

Conflict is a header-only library, and as such adding it's `include/` directory
to the include path is sufficient.

You can also use one of the following alternatives.

### Installing

To install the library, you should use the Meson project.

Configure the project with `-Dinstall_headers=true`, and an appropriate prefix
(e.g. `--prefix=/usr`), then install it with `ninja install` (setting the `DESTDIR`
environment variable as needed).

In addition to the headers, a pkg-config file is also installed, to allow for detecting the library.

### Meson subproject

You can use the following wrap file to add Conflict as a dependency:

```ini
[wrap-git]
url = https://github.com/qookei/conflict.git
revision = master

[provide]
dependency_names = conflict
```

Using this file, you can now bring in Conflict into your project:

```meson
conflict_dep = dependency('conflict')
```

## Example

Here is an example showcasing all the basic features of Conflict. A fully fleshed
out version of this program can also be found here: [tests/example.cpp](tests/example.cpp)
([build tests](#running) to build it as well).

```cpp
#include <conflict/conflict.hpp>

uint64_t flags;
uint64_t feature_flags;
uint64_t language;
std::string_view output;
std::vector<std::string_view> files;

const auto parser = conflict::parser{
	// You can define simple command line option that only set a value ...
	conflict::option{{'h', "help", "Show help"}, flags, (1 << 0)},
	conflict::option{{'V', "version", "Show version"}, flags, (1 << 1)},
	// Options may not have a short variant.
	conflict::option{{"verbose", "Be verbose"}, flags, (1 << 2)},
	// ... and options which consume a string ...
	conflict::string_option{{'o', "output", "Output file name"}, "filename", output},
	// ... and options which consume a combination of flags, and which allow for complex selections like all,-feat2,exp1 ...
	conflict::choice{{'f', "features", "Feature settings"}, conflict::choice_mode::combine, feature_flags,
		conflict::flag{{"all", "All features"}, 0b111, conflict::flag::is_default},
		conflict::flag{{"feat1", "Feature 1"}, 1 << 0},
		conflict::flag{{"feat2", "Feature 2"}, 1 << 1},
		conflict::flag{{"feat3", "Feature 3"}, 1 << 2},
		conflict::flag{{"exp1", "Experimental feature 1"}, 1 << 3},
	},
	// ... and options which pick one out of a list of flags.
	conflict::choice{{'l', "language", "Language to use"}, conflict::choice_mode::replace, language,
		conflict::flag{{"system", "System language"}, 1, conflict::flag::is_default},
		// Descriptions are also optional, the only non-optional part is the long string.
		conflict::flag{{"english"}, 2},
		conflict::flag{{"polish"}, 3},
		conflict::flag{{"german"}, 4},
		conflict::flag{{"spanish"}, 5},
	}
};

int main(int argc, char **argv) {
	// Reset all flags to their default values.
	parser.apply_defaults();

	// Skip the program name, and collect any other arguments into `files`.
	auto st = parser.parse(argc - 1, argv + 1, files);

	// Report any errors and exit
	conflict::default_report(st);

	if (flags & (1 << 0)) {
		parser.print_help();
		return 0;
	}

	// ...
}
```

## Tests

Conflict comes with a test suite which aims to test the functionality of the whole library.

### Additional requirements

 - GTest,
 - Lcov and GenHTML, or Gcovr (for coverage reports only).

### Running

To build and run the test suite, run the following command:

```
$ meson builddir -Dbuild_tests=true
$ ninja -C builddir test
```

### Coverage

You may also wish to pass `-Db_coverage=true` to the Meson invocation to enable coverage reports.
To generate them, after running the tests, run the following:

```
$ ninja -C builddir coverage-html # or text, xml, sonarqube
```

## License

This project is licensed under the Zlib license. Check [LICENSE.md](LICENSE.md) for more information.
