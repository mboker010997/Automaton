cmake_minimum_required(VERSION 3.5)

project(AutTests)

find_package(GTest REQUIRED CONFIG)

add_custom_command(
    OUTPUT mbo.txt
    COMMAND cp -r ${CMAKE_SOURCE_DIR}/tests/txt ${CMAKE_CURRENT_BINARY_DIR}/txt
)

add_executable(Tests main_test.cpp tests_imp.cpp)
add_dependencies(Tests AutLib)

target_link_libraries(
    Tests
    PUBLIC
    AutLib
    GTest::gtest GTest::gtest_main
)

# target_include_directories(
#     Tests
#     PUBLIC
#     ../lib
# )

include(GoogleTest)
gtest_discover_tests(Tests)