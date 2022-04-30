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
	// ... and options which consume a combination of flags, allowing for advanced selections like all,-feat2,exp1 ...
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

	if (flags & (1 << 1)) {
		std::cout << "Conflict v0.1\n";
		return 0;
	}

	if (flags & (1 << 2))
		std::cout << "Verbosity increased\n";

	std::cout << "Output: \"" << output << "\"\n";

	std::cout << "Features: ";
	if (feature_flags & (1 << 0))
		std::cout << "\"Feature 1\" ";

	if (feature_flags & (1 << 1))
		std::cout << "\"Feature 2\" ";

	if (feature_flags & (1 << 2))
		std::cout << "\"Feature 3\" ";

	if (feature_flags & (1 << 3))
		std::cout << "\"Experimental feature 1\" ";
	std::cout << "\n";

	std::cout << "Language: ";
	switch (language) {
		case 1: std::cout << "System"; break;
		case 2: std::cout << "English"; break;
		case 3: std::cout << "Polish"; break;
		case 4: std::cout << "German"; break;
		case 5: std::cout << "Spanish"; break;
		default: std::cout << "Other (unreachable)";
	}
	std::cout << "\n";

	std::cout << "Files: " << (files.size() ? "" : "(none)");
	for (auto f : files)
		std::cout << "\"" << f << "\" ";
	std::cout << "\n";
}
