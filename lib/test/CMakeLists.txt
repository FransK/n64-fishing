cmake_minimum_required(VERSION 3.14)
project(n64fishing_lib_tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fetch Catch2
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# Add the library
add_subdirectory(.. ${CMAKE_BINARY_DIR}/lib)

# Create test executable
add_executable(tests
    StringHashTest.cpp
    JsonParserTest.cpp
)

target_link_libraries(tests PRIVATE
    n64fishing_lib
    Catch2::Catch2WithMain
)

# Register tests with CTest
include(CTest)
include(Catch)
catch_discover_tests(tests)
