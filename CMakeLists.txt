cmake_minimum_required(VERSION 3.10)
project(NanoDS VERSION 0.1.1 LANGUAGES C)

# C Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Options
option(NANODS_BUILD_TESTS "Build test suite" ON)
option(NANODS_BUILD_BENCHMARKS "Build benchmarks" ON)
option(NANODS_BUILD_EXAMPLES "Build examples" ON)
option(NANODS_HARD_SAFETY "Enable hard safety mode" OFF)

# Header-only library
add_library(nanods INTERFACE)
target_include_directories(nanods INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Hard safety mode
if(NANODS_HARD_SAFETY)
    target_compile_definitions(nanods INTERFACE NANODS_HARD_SAFETY)
endif()

# Test executable
if(NANODS_BUILD_TESTS)
    add_executable(nanods_test test.c)
    target_link_libraries(nanods_test PRIVATE nanods)
    target_compile_options(nanods_test PRIVATE -Wall -Wextra -Wpedantic)
    
    enable_testing()
    add_test(NAME nanods_test COMMAND nanods_test)
endif()

# Benchmarks
if(NANODS_BUILD_BENCHMARKS)
    add_executable(bench_vector benchmarks/bench_vector.c)
    target_link_libraries(bench_vector PRIVATE nanods)
    target_compile_options(bench_vector PRIVATE -O3 -march=native)
    
    add_executable(bench_map benchmarks/bench_map.c)
    target_link_libraries(bench_map PRIVATE nanods)
    target_compile_options(bench_map PRIVATE -O3 -march=native)
    
    add_executable(bench_comparison benchmarks/bench_comparison.c)
    target_link_libraries(bench_comparison PRIVATE nanods)
    target_compile_options(bench_comparison PRIVATE -O3 -march=native)
endif()

# Examples
if(NANODS_BUILD_EXAMPLES)
    add_executable(word_frequency examples/word_frequency.c)
    target_link_libraries(word_frequency PRIVATE nanods)
    target_compile_options(word_frequency PRIVATE -Wall -Wextra)
    
    add_executable(command_history examples/command_history.c)
    target_link_libraries(command_history PRIVATE nanods)
    target_compile_options(command_history PRIVATE -Wall -Wextra)
endif()

# Installation
install(FILES nanods.h DESTINATION include)
install(DIRECTORY src/ DESTINATION include/nanods/src
        FILES_MATCHING PATTERN "*. h")

# pkg-config
configure_file(nanods.pc.in nanods. pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/nanods.pc
        DESTINATION lib/pkgconfig)

# Export targets
install(TARGETS nanods
        EXPORT nanods-targets
        INCLUDES DESTINATION include)

install(EXPORT nanods-targets
        FILE nanods-config.cmake
        NAMESPACE nanods::
        DESTINATION lib/cmake/nanods)

# Print configuration
message(STATUS "NanoDS v${PROJECT_VERSION}")
message(STATUS "  Build tests:       ${NANODS_BUILD_TESTS}")
message(STATUS "  Build benchmarks: ${NANODS_BUILD_BENCHMARKS}")
message(STATUS "  Build examples:    ${NANODS_BUILD_EXAMPLES}")
message(STATUS "  Hard safety:       ${NANODS_HARD_SAFETY}")
