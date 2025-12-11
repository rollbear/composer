set(CMAKE_CXX_STANDARD_REQUIRED yes)
set(CMAKE_CXX_EXTENSIONS no)

find_package(Catch2 3 QUIET)
if (NOT Catch2_FOUND)
    Include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_PROGRESS TRUE
            GIT_SHALLOW TRUE
            GIT_TAG v3.11.0)
    FetchContent_MakeAvailable(Catch2) # find_package equivalent
endif()

add_executable(
        test_composer
        test_composable_function.cpp
        test_back_binding.cpp
        test_front_binding.cpp
        test_functional.cpp
        test_tuple.cpp
        test_transform_args.cpp
        test_ranges.cpp
        test_algorithm.cpp
)

target_link_libraries(test_composer composer::composer Catch2::Catch2WithMain)
