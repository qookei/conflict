#include <gtest/gtest.h>

#include <conflict/string.hpp>
#include <conflict/parser.hpp>

namespace {

std::string_view opt1_arg;
std::vector<std::string_view> opt2_args;

const auto parser = conflict::parser{
	conflict::string_option{{'1', "opt1", "Option 1"}, "placeholder", opt1_arg},
	conflict::strings_option{{'2', "opt2", "Option 2"}, "placeholder", opt2_args}
};

} // namespace anonymous

TEST(String, SingleShort) {
	parser.apply_defaults();
	auto st = parser.parse({"-1", "foo"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "foo");
}

TEST(String, SingleLongSpace) {
	parser.apply_defaults();
	auto st = parser.parse({"--opt1", "foo"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "foo");
}

TEST(String, SingleLongEq) {
	parser.apply_defaults();
	auto st = parser.parse({"--opt1=foo"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "foo");
}

TEST(String, SingleReplace) {
	parser.apply_defaults();
	auto st = parser.parse({"--opt1=foo", "--opt1=bar"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "bar");
}

TEST(String, SingleMissing) {
	parser.apply_defaults();
	auto st = parser.parse({"--opt1"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::missing_argument);
	ASSERT_EQ(st.what1, "--opt1");
}

TEST(String, MultipleShort) {
	parser.apply_defaults();
	auto st = parser.parse({"-2", "foo", "-2", "bar"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt2_args.size(), 2);
	ASSERT_EQ(opt2_args[0], "foo");
	ASSERT_EQ(opt2_args[1], "bar");
}

TEST(String, MultipleLongSpace) {
	parser.apply_defaults();
	auto st = parser.parse({"--opt2", "foo", "--opt2", "bar"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt2_args.size(), 2);
	ASSERT_EQ(opt2_args[0], "foo");
	ASSERT_EQ(opt2_args[1], "bar");
}

TEST(String, MultipleLongEq) {
	parser.apply_defaults();
	auto st = parser.parse({"--opt2=foo", "--opt2=bar"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt2_args.size(), 2);
	ASSERT_EQ(opt2_args[0], "foo");
	ASSERT_EQ(opt2_args[1], "bar");
}

TEST(String, MultiplMissing) {
	parser.apply_defaults();
	auto st = parser.parse({"--opt2"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::missing_argument);
	ASSERT_EQ(st.what1, "--opt2");
}
