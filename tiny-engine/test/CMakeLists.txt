project("Tiny Engine Tests")

include(GoogleTest)

add_library(TestDyLib SHARED "testdylib.cpp")
target_link_libraries(TestDyLib PRIVATE TinyEngine::TinyEngine)
target_compile_definitions(TestDyLib PRIVATE TINY_ENGINE_EXPORT_SYMBOLS=1)

add_executable(TinyEngineTests "commandline_args_test.cpp" "dylib_loading_test.cpp")
target_link_libraries(TinyEngineTests PRIVATE GTest::gtest_main TinyEngine::TinyEngine)

gtest_discover_tests(TinyEngineTests)
