#include <composer/algorithm.hpp>
#include <composer/functional.hpp>

#include <catch2/catch_test_macros.hpp>

namespace {
struct numname {
    int num;
    std::string_view name;
};

constexpr numname values[]{
    { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" }
};

} // namespace

SCENARIO("all_of is right curried")
{
    SECTION("when called with range, predicate and projection, it calls "
            "ranges::all_of directly")
    {
        STATIC_REQUIRE(
            composer::all_of(values, composer::greater(0), &numname::num));
        REQUIRE(composer::all_of(values, composer::greater(0), &numname::num));
        STATIC_REQUIRE_FALSE(
            composer::all_of(values, composer::greater(1), &numname::num));
        REQUIRE_FALSE(
            composer::all_of(values, composer::greater(1), &numname::num));
    }
    SECTION("when called with iterator pair, predicate and projection, it "
            "calls ranges::all_of directly")
    {
        STATIC_REQUIRE(composer::all_of(std::begin(values),
                                        std::end(values),
                                        composer::greater(0),
                                        &numname::num));
        REQUIRE(composer::all_of(std::begin(values),
                                 std::end(values),
                                 composer::greater(0),
                                 &numname::num));
        STATIC_REQUIRE_FALSE(composer::all_of(std::begin(values),
                                              std::end(values),
                                              composer::greater(1),
                                              &numname::num));
        REQUIRE_FALSE(composer::all_of(std::begin(values),
                                       std::end(values),
                                       composer::greater(1),
                                       &numname::num));
    }
    SECTION("when called with range and composed predicate, it calls "
            "ranges::all_of directly")
    {
        STATIC_REQUIRE(
            composer::all_of(values, &numname::num | composer::greater(0)));
        REQUIRE(composer::all_of(values, &numname::num | composer::greater(0)));
        STATIC_REQUIRE_FALSE(
            composer::all_of(values, &numname::num | composer::greater(1)));
        REQUIRE_FALSE(
            composer::all_of(values, &numname::num | composer::greater(1)));
    }
    SECTION("when called with iterator pair and composed predicate, it calls "
            "ranges::all_of directly")
    {
        STATIC_REQUIRE(composer::all_of(std::begin(values),
                                        std::end(values),
                                        &numname::num | composer::greater(0)));
        REQUIRE(composer::all_of(std::begin(values),
                                 std::end(values),
                                 &numname::num | composer::greater(0)));
        STATIC_REQUIRE_FALSE(
            composer::all_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater(1)));
        REQUIRE_FALSE(composer::all_of(std::begin(values),
                                       std::end(values),
                                       &numname::num | composer::greater(1)));
    }
    SECTION("the result of all_of(projection) is callable with range and "
            "predicate for call to ranges::all_of")
    {
        constexpr auto all_nums = composer::all_of(&numname::num);
        STATIC_REQUIRE(all_nums(values, composer::greater(0)));
        REQUIRE(all_nums(values, composer::greater(0)));
        STATIC_REQUIRE_FALSE(all_nums(values, composer::greater(1)));
        REQUIRE_FALSE(all_nums(values, composer::greater(1)));
    }
    SECTION(
        "the result of all_of(projection) is callable with iterator pair and "
        "predicate for call to ranges::all_of")
    {
        constexpr auto all_nums = composer::all_of(&numname::num);
        STATIC_REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater(0)));
        REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater(0)));
        STATIC_REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater(1)));
        REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater(1)));
    }
    SECTION("the result of all_of(composed predicate) is callable with range "
            "for call to ranges::all_of")
    {
        constexpr auto all_pos_ints
            = composer::all_of(&numname::num | composer::greater(0));
        constexpr auto all_small_ints
            = composer::all_of(&numname::num | composer::less(5));
        STATIC_REQUIRE(all_pos_ints(values));
        REQUIRE(all_pos_ints(values));
        STATIC_REQUIRE_FALSE(all_small_ints(values));
        REQUIRE_FALSE(all_small_ints(values));
    }
    SECTION("the result of all_of(composed predicate) is callable with "
            "iterator pair for call to ranges::all_of")
    {
        constexpr auto all_pos_ints
            = composer::all_of(&numname::num | composer::greater(0));
        STATIC_REQUIRE(all_pos_ints(std::begin(values), std::end(values)));
        REQUIRE(all_pos_ints(std::begin(values), std::end(values)));
    }
}
