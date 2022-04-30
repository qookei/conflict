#pragma once

#include <cstdint>
#include <conflict/info.hpp>
#include <conflict/detail.hpp>

namespace conflict {

struct option {
	static constexpr bool is_default = true;
	static constexpr bool needs_arg = false;

	option_info info;

	uint64_t &target;

	uint64_t flag_bit;
	bool set_by_default = false;

	void print_argument_placeholder(detail::line_printer &) const { }
	void print_extra(detail::line_printer &) const { }

	void apply_default() const {
		target |= set_by_default ? flag_bit : 0;
	}

	void set() const {
		target |= flag_bit;
	}
};

} // namespace conflict
