#pragma once

#include <string_view>

#include <iostream>

namespace conflict {

enum class error {
	ok,
	invalid_option,
	invalid_argument,
	missing_argument
};

struct status {
	error err = error::ok;
	std::string_view what1 = {};
	std::string_view what2 = {};

	operator bool() const {
		return err == error::ok;
	}
};

inline void default_report(status st) {
	if (st.err == error::ok)
		return;

	switch (st.err) {
		case error::invalid_option:
			std::cerr << "Invalid option \"" << st.what1 << "\"\n";
			break;
		case error::invalid_argument:
			std::cerr << "Invalid argument \"" << st.what1
				<< "\" for \"" << st.what2 << "\"\n";
			break;
		case error::missing_argument:
			std::cerr << "Missing argument for \"" << st.what1 << "\"\n";
			break;
		default:
			std::cerr << "Invalid error in conflict::default_report\n";
	}

	std::exit(1);
}

} // namespace conflict
