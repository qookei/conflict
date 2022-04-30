#pragma once

#include <tuple>
#include <vector>
#include <cstddef>
#include <iostream>
#include <string_view>

#include <conflict/detail.hpp>

namespace conflict {

template <typename ...Ts>
struct parser {
	parser(Ts ...ts)
	: options{std::make_tuple(ts...)} { }

	static constexpr size_t space = 32;

	void print_help() const {
		auto print_option = [&] (const auto &opt) {
			detail::line_printer pr{parser::space};

			pr.print("    ");
			if (opt.info.short_opt) {
				pr.print('-', opt.info.short_opt);

				if (opt.info.long_opt.size()) {
					pr.print('/');
				}
			}

			if (opt.info.long_opt.size()) {
				pr.print("--", opt.info.long_opt);
			}

			opt.print_argument_placeholder(pr);

			if (opt.info.desc.size()) {
				pr.pad();
				std::cout << opt.info.desc;
			}

			std::cout << "\n";

			opt.print_extra(pr);
		};

		std::cout << "Options:\n";

		std::apply([&] (const auto &...ts) {
			(print_option(ts), ...);
		}, options);
	}

	void apply_defaults() const {
		std::apply([&] (const auto &...ts) {
			((ts.target = std::decay_t<decltype(ts.target)>{}), ...);
		}, options);

		std::apply([&] (const auto &...ts) {
			(ts.apply_default(), ...);
		}, options);
	}

	void parse(int argc, const char **argv, std::vector<std::string_view> &positional) const {
		std::vector<std::string_view> args{argv, argv + argc};
		return parse(args, positional);
	}

	void parse(const std::vector<std::string_view> &args, std::vector<std::string_view> &positional) const {
		auto try_parse_arg = [&] (size_t &i) -> bool {
			auto arg = args[i];

			if (arg.size() < 2)
				return false;

			bool is_long = arg[1] == '-';

			auto try_process_opt = [&] (const auto &opt) -> bool {
				if (!is_long && !opt.info.short_opt)
					return false;

				if (!is_long && arg[1] != opt.info.short_opt)
					return false;

				if (is_long && arg.substr(2, opt.info.long_opt.size()) != opt.info.long_opt)
					return false;

				if (is_long && arg.size() != opt.info.long_opt.size() + 2
						&& (arg.size() <= opt.info.long_opt.size() + 2
							&& arg[2 + opt.info.long_opt.size()] != '='))
					return false;

				if constexpr (std::decay_t<decltype(opt)>::needs_arg) {
					std::string_view next_arg{};

					if (is_long && arg[2 + opt.info.long_opt.size()] == '=') {
						next_arg = arg.substr(3 + opt.info.long_opt.size());
					} else {
						if (i == args.size() - 1) {
							// TODO: better error message
							std::cerr << "Missing argument\n";
							std::exit(1);
						} else {
							next_arg = args[++i];
						}
					}

					opt.try_process_arg(next_arg);
				} else {
					return arg.back() != '=';
				}

				return true;
			};

			return std::apply([&] (const auto &...ts) {
				return (try_process_opt(ts) || ...);
			}, options);
		};

		for (size_t i = 0; i < args.size(); i++) {
			auto arg = args[i];
			if (arg.size() > 1 && arg[0] == '-') {
				if (!try_parse_arg(i)) {
					//std::cerr << "Unknown option: " << arg << "\n";
					//std::exit(1);
					positional.push_back(arg);
				}

			} else {
				positional.push_back(arg);
			}
		}
	}

	std::tuple<Ts...> options;
};

template <typename ...Ts>
parser(Ts ...) -> parser<Ts...>;

} // namespace conflict
