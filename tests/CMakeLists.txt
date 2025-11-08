set(CMAKE_CXX_STANDARD_REQUIRED yes)
set(CMAKE_CXX_EXTENSIONS no)

find_package(Catch2 3 REQUIRED)

add_executable(
        test_composer
        test_arity_function.cpp
        test_right_curry.cpp
        test_left_curry.cpp
        test_functional.cpp
        test_tuple.cpp
)

target_link_libraries(test_composer composer::composer Catch2::Catch2WithMain)
