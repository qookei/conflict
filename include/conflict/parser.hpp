#pragma once

#include <tuple>
#include <vector>
#include <cstddef>
#include <iostream>
#include <string_view>
#include <variant>

#include <conflict/error.hpp>
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

	status parse(int argc, const char **argv, std::vector<std::string_view> &positional) const {
		std::vector<std::string_view> args{argv, argv + argc};
		return parse(args, positional);
	}

	status parse(const std::vector<std::string_view> &args, std::vector<std::string_view> &positional) const {
		auto parse_arg = [&] (size_t &i) -> status {
			auto arg = args[i];

			if (arg.size() < 2)
				return status{error::invalid_option, arg};

			bool is_long = arg[1] == '-';

			auto process_opt = [&] (const auto &opt) -> std::variant<bool, status> {
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
							return status{error::missing_argument, arg};
						} else {
							next_arg = args[++i];
						}
					}

					return opt.process_arg(next_arg);
				} else {
					return arg.back() != '=';
				}

				return true;
			};

			return std::apply([&] (const auto &...ts) -> status {
				status st{};
				std::variant<bool, status> ret{};

				bool ok = (((ret = process_opt(ts)),
					std::holds_alternative<status>(ret)
						? ((st = std::get<status>(ret)), false)
						: !std::get<bool>(ret)
					) && ...);

				if (ok && st)
					st = status{error::invalid_option, arg};

				return st;
			}, options);
		};

		for (size_t i = 0; i < args.size(); i++) {
			auto arg = args[i];
			if (arg.size() > 1 && arg[0] == '-') {
				if (auto st = parse_arg(i); !st) {
					return st;
				}

			} else {
				positional.push_back(arg);
			}
		}

		return status{};
	}

	std::tuple<Ts...> options;
};

template <typename ...Ts>
parser(Ts ...) -> parser<Ts...>;

} // namespace conflict
