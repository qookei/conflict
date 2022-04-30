#pragma once

#include <string_view>
#include <cstdint>
#include <array>
#include <cassert>

// TODO: Replace with error return
#include <iostream>

#include <conflict/info.hpp>
#include <conflict/detail.hpp>

namespace conflict {

struct flag {
	static constexpr bool is_default = true;

	option_info info;

	uint64_t flag_bit;
	bool set_by_default = false;
};

enum class choice_mode {
	combine,
	replace
};

template <size_t N>
struct choice {
	static constexpr bool needs_arg = true;

	template <typename ...Ts>
	choice(option_info info, choice_mode mode, uint64_t &target, Ts ...ts)
	: info{info}, mode{mode}, target{target}, flags{ts...} { }

	option_info info;

	choice_mode mode;

	uint64_t &target;
	std::array<flag, N> flags;

	void print_argument_placeholder(detail::line_printer &pr) const {
		if (mode == choice_mode::combine) {
			pr.print(" a,-c,...");
		} else {
			pr.print(" {");

			for (size_t i = 0; i < flags.size(); i++) {
				pr.print(flags[i].info.long_opt);

				if (i < flags.size() - 1) {
					pr.print("|");
				}
			}

			pr.print("}");
		}
	}

	void print_extra(detail::line_printer &pr) const {
		for (auto &flg : flags) {
			if (mode == choice_mode::replace && !flg.info.desc.size())
				continue;

			pr.start_extra_line();
			pr.print("    ", flg.info.long_opt);
			pr.pad();
			pr.print(flg.info.desc, '\n');
		}
	}

	void apply_default() const {
		for (auto &flg : flags) {
			assert(!(flg.set_by_default && target
					&& mode == choice_mode::replace));
			target |= flg.set_by_default ? flg.flag_bit : 0;
		}
	}

	void try_process_arg(std::string_view arg) const {
		size_t j = 0;

		while (j < arg.size()) {
			auto comma = arg.find(',', j);

			if (comma == std::string_view::npos)
				comma = arg.size();

			auto part = arg.substr(j, comma - j);
			auto flg = part;

			bool unset = flg.size() && flg[0] == '-';
			if (unset)
				flg = flg.substr(1);

			bool found = false;

			for (const auto &flag : flags) {
				if (flg == flag.info.long_opt) {
					found = true;

					if (mode == choice_mode::replace) {
						if (unset) {
							found = false;
							break;
						}

						target = flag.flag_bit;
					} else {
						assert(mode == choice_mode::combine);

						if (unset)
							target &= ~flag.flag_bit;
						else
							target |= flag.flag_bit;
					}

					break;
				}
			}

			if (!found) {
				std::cerr << "Invalid flag " << part << "\n";
				std::exit(1);
			}

			j = comma + 1;
		}
	}
};

template <typename ...Ts>
choice(option_info, choice_mode, uint64_t &, Ts ...) -> choice<sizeof...(Ts)>;

} // namespace conflict
