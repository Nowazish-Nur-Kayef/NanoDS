cmake_minimum_required(VERSION 3.10)
project(NanoDS VERSION 1.0.0 LANGUAGES C)

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
    $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Hard safety mode
if(NANODS_HARD_SAFETY)
    target_compile_definitions(nanods INTERFACE NANODS_HARD_SAFETY)
endif()

# Version information
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/nanods.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/nanods.pc"
    @ONLY
)

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
    
    add_executable(bench_list2 benchmarks/bench_list2.c)
    target_link_libraries(bench_list2 PRIVATE nanods)
    target_compile_options(bench_list2 PRIVATE -O3 -march=native)
    
    add_executable(bench_ring benchmarks/bench_ring.c)
    target_link_libraries(bench_ring PRIVATE nanods)
    target_compile_options(bench_ring PRIVATE -O3 -march=native)
endif()

# Examples
if(NANODS_BUILD_EXAMPLES)
    add_executable(word_frequency examples/word_frequency.c)
    target_link_libraries(word_frequency PRIVATE nanods)
    target_compile_options(word_frequency PRIVATE -Wall -Wextra)
    
    add_executable(command_history examples/command_history.c)
    target_link_libraries(command_history PRIVATE nanods)
    target_compile_options(command_history PRIVATE -Wall -Wextra)
    
    add_executable(ring_buffer_example examples/ring_buffer_example.c)
    target_link_libraries(ring_buffer_example PRIVATE nanods)
    target_compile_options(ring_buffer_example PRIVATE -Wall -Wextra)
    
    add_executable(iterator_example examples/iterator_example.c)
    target_link_libraries(iterator_example PRIVATE nanods)
    target_compile_options(iterator_example PRIVATE -Wall -Wextra)
endif()

# Installation
install(FILES nanods.h DESTINATION include)
install(DIRECTORY src/ DESTINATION include/nanods/src
        FILES_MATCHING PATTERN "*.h")
install(DIRECTORY docs/ DESTINATION share/doc/nanods
        FILES_MATCHING PATTERN "*.md")

# pkg-config
install(FILES ${CMAKE_BINARY_DIR}/nanods.pc
        DESTINATION lib/pkgconfig)

# Export targets for downstream projects
install(TARGETS nanods
        EXPORT nanods-targets
        INCLUDES DESTINATION include)

# Generate and install config files
install(EXPORT nanods-targets
        FILE nanods-targets.cmake
        NAMESPACE nanods::
        DESTINATION lib/cmake/nanods)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/nanods-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/nanods-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/nanods-config.cmake"
    INSTALL_DESTINATION lib/cmake/nanods
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/nanods-config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/nanods-config-version.cmake"
    DESTINATION lib/cmake/nanods
)

# Print configuration
message(STATUS "==========================================")
message(STATUS "NanoDS v${PROJECT_VERSION} Configuration")
message(STATUS "==========================================")
message(STATUS "  Build tests:        ${NANODS_BUILD_TESTS}")
message(STATUS "  Build benchmarks:  ${NANODS_BUILD_BENCHMARKS}")
message(STATUS "  Build examples:    ${NANODS_BUILD_EXAMPLES}")
message(STATUS "  Hard safety:        ${NANODS_HARD_SAFETY}")
message(STATUS "  C Standard:        ${CMAKE_C_STANDARD}")
message(STATUS "  Install prefix:    ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==========================================")