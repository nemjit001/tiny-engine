#include <gtest/gtest.h>
#include <tiny_engine/core/dylib_loading.hpp>

namespace tiny_engine_core
{
	TEST(DyLibLoadingTests, TestBadLib)
	{
		tiny_engine::core::DyLibHandle* pHandle = tiny_engine::core::DyLib::open("badlibname");
		ASSERT_NE(pHandle, nullptr);
		ASSERT_FALSE(tiny_engine::core::DyLib::loaded(pHandle));
		tiny_engine::core::DyLib::close(pHandle);
	}

	TEST(DyLibLoadingTests, TestLibLoading)
	{
		// TODO(nemjit001): Load test library with known name & function pointers
		tiny_engine::core::DyLibHandle* pHandle = tiny_engine::core::DyLib::open("TinyEngine-shared");
		ASSERT_NE(pHandle, nullptr);
		ASSERT_TRUE(tiny_engine::core::DyLib::loaded(pHandle));
		tiny_engine::core::DyLib::close(pHandle);
	}
} // namespace tiny_engine_core
