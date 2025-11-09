#include <composer/functional.hpp>
#include <composer/tuple.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("apply_to", "[tuple]")
{
    SECTION("called with a function and a tuple, it forwards the call in "
            "constexpr context")
    {
        STATIC_REQUIRE(composer::apply_to(composer::minus, std::tuple(5, 2))
                       == 3);
    }
    SECTION("called with a tuple, it binds it to the right")
    {
        constexpr auto apply = composer::apply_to(std::tuple(5, 2));
        STATIC_REQUIRE(apply(composer::minus) == 3);
    }
}

TEST_CASE("apply_using", "[tuple]")
{
    SECTION("called with a function and a tuple, it forwards the call in "
            "constexpr context")
    {
        STATIC_REQUIRE(composer::apply_using(composer::minus, std::tuple(5, 2))
                       == 3);
    }
    SECTION("called with a tuple, it binds it to the right")
    {
        constexpr auto apply = composer::apply_using(composer::minus);
        STATIC_REQUIRE(apply(std::tuple(5, 2)) == 3);
    }
}

TEST_CASE("get", "[tuple]")
{
    SECTION("called with a tuple, it returns the Ith member")
    {
        constexpr std::tuple tup(5, 2);
        STATIC_REQUIRE(composer::get<0>(tup) == 5);
        STATIC_REQUIRE(composer::get<1>(tup) == 2);
    }
    SECTION("piped from a tuple, it returns the Ith member")
    {
        constexpr std::tuple tup(5, 2);
        STATIC_REQUIRE((tup | composer::get<0>) == 5);
        STATIC_REQUIRE((tup | composer::get<1>) == 2);
    }
}
