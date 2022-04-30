#pragma once

#include <vector>
#include <string_view>
#include <conflict/info.hpp>
#include <conflict/detail.hpp>

namespace conflict {

struct string_option {
	static constexpr bool needs_arg = true;

	option_info info;

	std::string_view placeholder;

	std::string_view &target;

	void print_argument_placeholder(detail::line_printer &pr) const {
		pr.print(" <", placeholder, ">");
	}

	void print_extra(detail::line_printer &) const { }

	void apply_default() const {}

	void try_process_arg(std::string_view arg) const {
		target = arg;
	}
};

struct strings_option {
	static constexpr bool needs_arg = true;

	option_info info;

	std::string_view placeholder;

	std::vector<std::string_view> &target;

	void print_argument_placeholder(detail::line_printer &pr) {
		pr.print(" <", placeholder, ">");
	}

	void print_extra(detail::line_printer &) const { }

	void apply_default() const {}

	void try_process_arg(std::string_view arg) const {
		target.push_back(arg);
	}
};

} // namespace conflict
