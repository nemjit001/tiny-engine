project("Tiny Engine Tests")

include(GoogleTest)

add_executable(TinyEngineTests "commandline_args_test.cpp")
target_link_libraries(TinyEngineTests GTest::gtest_main TinyEngine)

gtest_discover_tests(TinyEngineTests)
