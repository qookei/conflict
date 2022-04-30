#include <gtest/gtest.h>

#include <conflict/option.hpp>
#include <conflict/parser.hpp>

TEST(Option, Options) {
	uint64_t flags = 0;

	const auto parser = conflict::parser{
		conflict::option{{'1', "opt1", "Option 1"}, flags, 1},
		conflict::option{{'2', "opt2", "Option 2"}, flags, 2},
		conflict::option{{'3', "opt3", "Option 3"}, flags, 4}
	};

	auto st = parser.parse({"--opt1", "--opt3"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(flags & 1, 1);
	ASSERT_EQ(flags & 2, 0);
	ASSERT_EQ(flags & 4, 4);
}

TEST(Option, Invalid) {
	const auto parser = conflict::parser{};

	auto st = parser.parse({"-1", "--opt3"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::invalid_option);
	ASSERT_EQ(st.what1, "-1");
}
