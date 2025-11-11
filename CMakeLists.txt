cmake_minimum_required(VERSION 3.31)
project(composer)

option(unittest "Enable unittest" no)

add_library(composer INTERFACE)
add_library(composer::composer ALIAS composer)
target_include_directories(
        composer
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if (unittest)
    add_subdirectory(tests)
endif()
