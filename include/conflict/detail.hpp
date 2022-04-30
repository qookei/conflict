#pragma once

#include <iostream>
#include <cstddef>

namespace conflict::detail {
	struct line_printer {
		void print_item(std::string_view str) {
			std::cout << str;
			n_printed += str.size();
		}

		void print_item(char c) {
			std::cout << c;
			n_printed++;
		}

		template <typename ...Ts>
		void print(Ts ...args) {
			(print_item(args), ...);
		}

		void start_extra_line() {
			n_printed = 0;
			std::cout << "    ";
		}

		void pad() {
			size_t n = width_until_break;

			if ((n_printed + 1) > width_until_break)
				std::cout << '\n';
			else
				n = width_until_break - n_printed;

			for (size_t i = 0; i < n; i++)
				std::cout << ' ';
		}

		size_t width_until_break = 32;
		size_t n_printed = 0;
	};
} // namespace conflict::detail
