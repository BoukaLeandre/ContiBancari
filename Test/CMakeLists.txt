
include(GoogleTest)
gtest_discover_tests(test_conti)
enable_testing()

add_executable(test_conti Test/test_conti.cpp)
target_link_libraries(test_conti gtest gtest_main)

add_test(NAME ContoTest COMMAND test_conti)
