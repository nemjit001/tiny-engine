#include <gtest/gtest.h>
#include <tiny_engine/core/commandline_args.hpp>

namespace tiny_engine_core
{
	TEST(CommandlineArgsTests, TestEmptyArgs)
	{
		tiny_engine::core::CommandlineArgs empty(0, nullptr);
		ASSERT_FALSE(empty.isSet("foo"));
		ASSERT_STREQ(empty.argValue("bar"), "");
	}

	TEST(CommandlineArgsTests, TestFlagArgs)
	{
		char const* argv[] = {
			"program",
			"foo",
			"bar",
			"baz"
		};

		tiny_engine::core::CommandlineArgs args(sizeof(argv) / sizeof(argv[0]), argv);
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

		tiny_engine::core::CommandlineArgs args(sizeof(argv) / sizeof(argv[0]), argv);
		ASSERT_STREQ(args.argValue("foo"), "bar");
	}

	TEST(CommandlineArgsTests, TestEndOfOptions)
	{
		char const* argv[] = {
			"program",
			"bar",
			"--", // Marks end of commands
			"foo",
		};

		tiny_engine::core::CommandlineArgs args(sizeof(argv) / sizeof(argv[0]), argv);
		ASSERT_FALSE(args.isSet("foo"));
	}

	TEST(CommandlineArgsTests, TestEmptyPostOptionValues)
	{
		char const* argv[] = {
			"program",
			"bar",
			"--", // Marks end of commands
		};

		tiny_engine::core::CommandlineArgs args(sizeof(argv) / sizeof(argv[0]), argv);
		uint32_t valCount = 0;
		char const** vals = args.getPostOptionsValues(valCount);
		ASSERT_EQ(valCount, 0);
		ASSERT_EQ(vals, nullptr);
	}

	TEST(CommandlineArgsTests, TestPostOptionValues)
	{
		char const* argv[] = {
			"program",
			"bar",
			"--", // Marks end of commands
			"foo",
			"foo2"
		};

		tiny_engine::core::CommandlineArgs args(sizeof(argv) / sizeof(argv[0]), argv);
		uint32_t valCount = 0;
		auto const& vals = args.getPostOptionsValues(valCount);
		ASSERT_EQ(valCount, 2);
		ASSERT_NE(vals, nullptr);
		ASSERT_STREQ(vals[0], "foo");
		ASSERT_STREQ(vals[1], "foo2");
	}
}
