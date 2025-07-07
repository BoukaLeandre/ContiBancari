cmake_minimum_required(VERSION 3.10)
project(Untitled1)

set(CMAKE_CXX_STANDARD 17)


add_executable(main main.cpp)


add_subdirectory(lib/googletest)


add_executable(test_conti Test/test_conti.cpp)
target_link_libraries(test_conti gtest gtest_main)

enable_testing()
add_test(NAME ContoTest COMMAND test_conti)
