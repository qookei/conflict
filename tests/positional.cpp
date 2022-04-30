#include <gtest/gtest.h>

#include <conflict/string.hpp>
#include <conflict/parser.hpp>

namespace {

std::string_view opt1_arg;

const auto parser = conflict::parser{
	conflict::string_option{{'1', "opt1", "Option 1"}, "placeholder", opt1_arg},
};

} // namespace anonymous

TEST(Positional, NoneGiven) {
	std::vector<std::string_view> pos;

	parser.apply_defaults();
	auto st = parser.parse({"-1", "foo"}, pos);

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "foo");
	ASSERT_EQ(pos.size(), 0);
}

TEST(Positional, OneGiven) {
	std::vector<std::string_view> pos;

	parser.apply_defaults();
	auto st = parser.parse({"bar", "-1", "foo"}, pos);

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "foo");
	ASSERT_EQ(pos.size(), 1);
	ASSERT_EQ(pos[0], "bar");
}

TEST(Positional, TwoGiven) {
	std::vector<std::string_view> pos;

	parser.apply_defaults();
	auto st = parser.parse({"bar", "-1", "foo", "baz"}, pos);

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "foo");
	ASSERT_EQ(pos.size(), 2);
	ASSERT_EQ(pos[0], "bar");
	ASSERT_EQ(pos[1], "baz");
}

TEST(Positional, NothingGiven) {
	std::vector<std::string_view> pos;

	parser.apply_defaults();
	auto st = parser.parse({}, pos);

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(opt1_arg, "");
	ASSERT_EQ(pos.size(), 0);
}
