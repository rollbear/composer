#include <composer/left_curry.hpp>
#include <composer/transform_args.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("transform_args called wit all args makes the call directly via the "
          "transformation of all args")
{
    constexpr auto minus = composer::make_arity_function<2>(
        [](auto a, auto b) -> decltype(a - b) { return a - b; });
    constexpr auto deref_minus = composer::transform_args(
        [](const auto& p) -> decltype(*p) { return *p; }, minus);
    static constexpr int a = 5;
    static constexpr int b = 2;
    STATIC_REQUIRE(deref_minus(&a, &b) == 3);
    REQUIRE(deref_minus(&a, &b) == 3);
}

TEST_CASE(
    "transform_args of a left curried function is a left curried function")
{
    constexpr auto f = composer::make_arity_function<2, composer::left_curry>(
        [](auto a, auto b) { return a - b; });
    constexpr auto deref_f = composer::transform_args(
        [](auto p) -> decltype(*p) { return *p; }, f);
    static constexpr int a = 5;
    constexpr auto sub_from_5 = deref_f(&a);
    static constexpr int b = 2;
    STATIC_REQUIRE(sub_from_5(&b) == 3);
    REQUIRE(sub_from_5(&b) == 3);
}

TEST_CASE("transform_args of a partially bound left curried function is "
          "callable with one arg through the transformation")
{
    constexpr auto minus
        = composer::make_arity_function<2, composer::left_curry>(
            [](auto a, auto b) -> decltype(a - b) { return a - b; });
    constexpr auto sub_from_5 = minus(5);
    constexpr auto dsub = composer::transform_args(
        [](auto p) -> decltype(*p) { return *p; }, sub_from_5);
    static constexpr int b = 2;
    STATIC_REQUIRE(dsub(&b) == 3);
    REQUIRE(dsub(&b) == 3);
}
