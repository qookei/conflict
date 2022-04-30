#pragma once

#include <string_view>

namespace conflict {

struct option_info {
	constexpr option_info(char short_opt, std::string_view long_opt, std::string_view desc = {})
	: short_opt{short_opt}, long_opt{long_opt}, desc{desc} { }

	constexpr option_info(std::string_view long_opt, std::string_view desc = {})
	: short_opt{0}, long_opt{long_opt}, desc{desc} { }

	char short_opt;
	std::string_view long_opt;
	std::string_view desc;
};

} // namespace conflict
