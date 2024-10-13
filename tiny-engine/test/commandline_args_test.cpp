#include <gtest/gtest.h>
#include <tiny_engine/core/commandline_args.hpp>

namespace tiny_engine_core
{
	TEST(CommandlineArgsTests, TestEmptyArgs)
	{
		tiny_engine::CommandlineArgs empty(0, nullptr);
		ASSERT_FALSE(empty.isSet("foo"));
		ASSERT_TRUE(empty.argValue("bar").empty());
	}

	TEST(CommandlineArgsTests, TestFlagArgs)
	{
		char const* argv[] = {
			"program",
			"foo",
			"bar",
			"baz"
		};

		tiny_engine::CommandlineArgs args(sizeof(argv) / sizeof(argv[0]), argv);
		ASSERT_TRUE(args.isSet("foo"));
		ASSERT_TRUE(args.isSet("bar"));
	}

	TEST(CommandlineArgsTests, TestValueArgs)
	{
		char const* argv[] = {
			"program",
			"foo",
			"bar",
			"baz"
		};

		tiny_engine::CommandlineArgs args(sizeof(argv) / sizeof(argv[0]), argv);
		ASSERT_EQ(args.argValue("foo"), "bar");
	}
}
