#include <composer/arity_function.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

TEST_CASE("an arity function object is called with all provided arguments")
{
    constexpr auto minus = composer::arity_function<std::minus<>, 2>{};
    STATIC_REQUIRE(minus(5, 2) == 3);
}

TEST_CASE("an arity function is not callable with too many arguments")
{
    constexpr auto minus = composer::arity_function<std::minus<>, 2>{};
    STATIC_REQUIRE_FALSE(std::is_invocable_v<decltype(minus), int, int, int>);
}

TEST_CASE("an arity function is not callable with too few arguments")
{
    constexpr auto minus = composer::arity_function<std::minus<>, 2>{};
    STATIC_REQUIRE_FALSE(std::is_invocable_v<decltype(minus), int>);
}

TEST_CASE("an arity function is callable with fewer arguments than the stated "
          "arity, if the underlying function allows it")
{
    constexpr auto decrement = composer::make_arity_function<2>(
        [](int a, int b = 1) { return a - b; });
    STATIC_REQUIRE(decrement(5) == 4);
}

TEST_CASE(
    "an arity function calls with the same qualifiers as the function object")
{
    SECTION("call on const l-value reference is a called as a a const l-value "
            "reference")
    {
        auto f = composer::make_arity_function<0>(
            []<typename Self>(this Self&&) {
                STATIC_REQUIRE(std::is_lvalue_reference_v<Self&&>);
                STATIC_REQUIRE(std::is_const_v<std::remove_reference_t<Self>>);
            });
        std::as_const(f)();
    }
    SECTION("call on non-const l-value reference is a called as a a non-const "
            "l-value reference")
    {
        auto f
            = composer::make_arity_function<0>([]<typename Self>(this Self&&) {
                  STATIC_REQUIRE(std::is_lvalue_reference_v<Self&&>);
                  STATIC_REQUIRE_FALSE(
                      std::is_const_v<std::remove_reference_t<Self>>);
              });
        f();
    }
    SECTION("call on const r-value reference is a called as a a const r-value "
            "reference")
    {
        auto f = composer::make_arity_function<0>(
            []<typename Self>(this Self&&) {
                STATIC_REQUIRE(std::is_rvalue_reference_v<Self&&>);
                STATIC_REQUIRE(std::is_const_v<std::remove_reference_t<Self>>);
            });
        std::move(std::as_const(f))();
    }
    SECTION("call on non-const r-value reference is a called as a a non-const "
            "r-value reference")
    {
        auto f
            = composer::make_arity_function<0>([]<typename Self>(this Self&&) {
                  STATIC_REQUIRE(std::is_rvalue_reference_v<Self&&>);
                  STATIC_REQUIRE_FALSE(
                      std::is_const_v<std::remove_reference_t<Self>>);
              });
        std::move(f)();
    }
}

TEST_CASE("a piped expressions calls the right hand side with the result of "
          "the left hand side")
{
    constexpr auto to_string = composer::make_arity_function<1>(
        [](auto v) { return std::to_string(v); });
    constexpr auto minus = composer::arity_function<std::minus<>, 2>{};
    auto sub_to_str = minus | to_string;
    REQUIRE(sub_to_str(5, 3) == "2");
}
