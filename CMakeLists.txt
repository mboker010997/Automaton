cmake_minimum_required(VERSION 3.5)

project(Automaton)

add_subdirectory(lib)
add_subdirectory(tests)

add_executable(main main.cpp)
add_dependencies(main AutLib)

target_link_libraries(
    main
    PUBLIC
    AutLib
)

target_include_directories(
    main
    PUBLIC
    lib
)

# set_target_properties(
#     main AutLib Tests
#     PROPERTIES
#     LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
# )