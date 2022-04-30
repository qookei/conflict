#include <gtest/gtest.h>

#include <conflict/choice.hpp>
#include <conflict/parser.hpp>

namespace {

uint64_t combine_flags;

const auto combine_parser = conflict::parser{
	conflict::choice{{'c', "combine", "Combine"}, conflict::choice_mode::combine, combine_flags,
		conflict::flag{{"a", "Flag A"}, 1},
		conflict::flag{{"b", "Flag B"}, 2},
		conflict::flag{{"c", "Flag C"}, 4},
		conflict::flag{{"d", "Flag D"}, 8},
		conflict::flag{{"abc", "Flags A, B and C"}, 1 | 2 | 4}
	}
};

} // namespace anonymous

TEST(Choice, CombineShort) {
	combine_parser.apply_defaults();
	auto st = combine_parser.parse({"-c", "abc"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(combine_flags & 1, 1);
	ASSERT_EQ(combine_flags & 2, 2);
	ASSERT_EQ(combine_flags & 4, 4);
	ASSERT_EQ(combine_flags & 8, 0);
}

TEST(Choice, CombineLongSpace) {
	combine_parser.apply_defaults();
	auto st = combine_parser.parse({"--combine", "d"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(combine_flags & 1, 0);
	ASSERT_EQ(combine_flags & 2, 0);
	ASSERT_EQ(combine_flags & 4, 0);
	ASSERT_EQ(combine_flags & 8, 8);
}

TEST(Choice, CombineLongEq) {
	combine_parser.apply_defaults();
	auto st = combine_parser.parse({"--combine=c"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(combine_flags & 1, 0);
	ASSERT_EQ(combine_flags & 2, 0);
	ASSERT_EQ(combine_flags & 4, 4);
	ASSERT_EQ(combine_flags & 8, 0);
}

TEST(Choice, CombineMultiple) {
	combine_parser.apply_defaults();
	auto st = combine_parser.parse({"-c", "abc,d"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(combine_flags & 1, 1);
	ASSERT_EQ(combine_flags & 2, 2);
	ASSERT_EQ(combine_flags & 4, 4);
	ASSERT_EQ(combine_flags & 8, 8);
}

TEST(Choice, CombineMultipleDisable) {
	combine_parser.apply_defaults();
	auto st = combine_parser.parse({"-c", "abc,-c,d"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(combine_flags & 1, 1);
	ASSERT_EQ(combine_flags & 2, 2);
	ASSERT_EQ(combine_flags & 4, 0);
	ASSERT_EQ(combine_flags & 8, 8);
}

TEST(Choice, CombineMissing) {
	combine_parser.apply_defaults();
	auto st = combine_parser.parse({"-c"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::missing_argument);
	ASSERT_EQ(st.what1, "-c");
}

TEST(Choice, CombineInvalid) {
	combine_parser.apply_defaults();
	auto st = combine_parser.parse({"-c", "e"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::invalid_argument);
	ASSERT_EQ(st.what1, "e");
	ASSERT_EQ(st.what2, "combine");
}

namespace {

uint64_t replace_flags;

const auto replace_parser = conflict::parser{
	conflict::choice{{'r', "replace", "Replace"}, conflict::choice_mode::replace, replace_flags,
		conflict::flag{{"a", "Flag A"}, 1},
		conflict::flag{{"b", "Flag B"}, 2},
		conflict::flag{{"c", "Flag C"}, 4},
		conflict::flag{{"d", "Flag D"}, 8}
	}
};

} // namespace anonymous

TEST(Choice, ReplaceShort) {
	replace_parser.apply_defaults();
	auto st = replace_parser.parse({"-r", "a"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(replace_flags, 1);
}

TEST(Choice, ReplaceLongSpace) {
	replace_parser.apply_defaults();
	auto st = replace_parser.parse({"--replace", "b"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(replace_flags, 2);
}

TEST(Choice, ReplaceLongEq) {
	replace_parser.apply_defaults();
	auto st = replace_parser.parse({"--replace=c"});

	ASSERT_TRUE(st);
	ASSERT_EQ(st.err, conflict::error::ok);
	ASSERT_EQ(replace_flags, 4);
}

TEST(Choice, ReplaceMultiple) {
	replace_parser.apply_defaults();
	auto st = replace_parser.parse({"-r", "a,b"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::invalid_argument);
	ASSERT_EQ(st.what1, "b");
	ASSERT_EQ(st.what2, "replace");
}

TEST(Choice, ReplaceMultipleDisable) {
	replace_parser.apply_defaults();
	auto st = replace_parser.parse({"-r", "-a,b"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::invalid_argument);
	ASSERT_EQ(st.what1, "-a");
	ASSERT_EQ(st.what2, "replace");
}

TEST(Choice, ReplaceMissing) {
	replace_parser.apply_defaults();
	auto st = replace_parser.parse({"-r"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::missing_argument);
	ASSERT_EQ(st.what1, "-r");
}

TEST(Choice, ReplaceInvalid) {
	replace_parser.apply_defaults();
	auto st = replace_parser.parse({"-r", "e"});

	ASSERT_FALSE(st);
	ASSERT_EQ(st.err, conflict::error::invalid_argument);
	ASSERT_EQ(st.what1, "e");
	ASSERT_EQ(st.what2, "replace");
}
