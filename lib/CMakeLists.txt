cmake_minimum_required(VERSION 3.14)
project(n64fishing_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(n64fishing_lib STATIC
    source/JsonParser.cpp
    source/StringHash.cpp
)

target_include_directories(n64fishing_lib PUBLIC include)
