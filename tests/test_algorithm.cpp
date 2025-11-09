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

SCENARIO("any_of is right curried")
{
    SECTION("when called with range, predicate and projection, it calls "
            "ranges::any_of directly")
    {
        STATIC_REQUIRE(
            composer::any_of(values, composer::greater(4), &numname::num));
        REQUIRE(composer::any_of(values, composer::greater(4), &numname::num));
        STATIC_REQUIRE_FALSE(
            composer::any_of(values, composer::greater(5), &numname::num));
        REQUIRE_FALSE(
            composer::any_of(values, composer::greater(5), &numname::num));
    }
    SECTION("when called with iterator pair, predicate and projection, it "
            "calls ranges::any_of directly")
    {
        STATIC_REQUIRE(composer::any_of(std::begin(values),
                                        std::end(values),
                                        composer::greater(4),
                                        &numname::num));
        REQUIRE(composer::any_of(std::begin(values),
                                 std::end(values),
                                 composer::greater(4),
                                 &numname::num));
        STATIC_REQUIRE_FALSE(composer::any_of(std::begin(values),
                                              std::end(values),
                                              composer::greater(5),
                                              &numname::num));
        REQUIRE_FALSE(composer::any_of(std::begin(values),
                                       std::end(values),
                                       composer::greater(5),
                                       &numname::num));
    }
    SECTION("when called with range and composed predicate, it calls "
            "ranges::any_of directly")
    {
        STATIC_REQUIRE(
            composer::any_of(values, &numname::num | composer::greater(4)));
        REQUIRE(composer::any_of(values, &numname::num | composer::greater(4)));
        STATIC_REQUIRE_FALSE(
            composer::any_of(values, &numname::num | composer::greater(5)));
        REQUIRE_FALSE(
            composer::any_of(values, &numname::num | composer::greater(5)));
    }
    SECTION("when called with iterator pair and composed predicate, it calls "
            "ranges::any_of directly")
    {
        STATIC_REQUIRE(composer::any_of(std::begin(values),
                                        std::end(values),
                                        &numname::num | composer::greater(4)));
        REQUIRE(composer::any_of(std::begin(values),
                                 std::end(values),
                                 &numname::num | composer::greater(4)));
        STATIC_REQUIRE_FALSE(
            composer::any_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater(5)));
        REQUIRE_FALSE(composer::any_of(std::begin(values),
                                       std::end(values),
                                       &numname::num | composer::greater(5)));
    }
    SECTION("the result of any_of(projection) is callable with range and "
            "predicate for call to ranges::any_of")
    {
        constexpr auto all_nums = composer::any_of(&numname::num);
        STATIC_REQUIRE(all_nums(values, composer::greater(4)));
        REQUIRE(all_nums(values, composer::greater(4)));
        STATIC_REQUIRE_FALSE(all_nums(values, composer::greater(5)));
        REQUIRE_FALSE(all_nums(values, composer::greater(5)));
    }
    SECTION(
        "the result of any_of(projection) is callable with iterator pair and "
        "predicate for call to ranges::any_of")
    {
        constexpr auto all_nums = composer::any_of(&numname::num);
        STATIC_REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater(4)));
        REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater(4)));
        STATIC_REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater(5)));
        REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater(5)));
    }
    SECTION("the result of any_of(composed predicate) is callable with range "
            "for call to ranges::any_of")
    {
        constexpr auto any_gt4
            = composer::any_of(&numname::num | composer::greater(4));
        constexpr auto any_gt5
            = composer::any_of(&numname::num | composer::greater(5));
        STATIC_REQUIRE(any_gt4(values));
        REQUIRE(any_gt4(values));
        STATIC_REQUIRE_FALSE(any_gt5(values));
        REQUIRE_FALSE(any_gt5(values));
    }
    SECTION("the result of any_of(composed predicate) is callable with "
            "iterator pair for call to ranges::any_of")
    {
        constexpr auto any_gt4
            = composer::any_of(&numname::num | composer::greater(4));
        constexpr auto any_gt5
            = composer::any_of(&numname::num | composer::greater(5));
        STATIC_REQUIRE(any_gt4(std::begin(values), std::end(values)));
        REQUIRE(any_gt4(std::begin(values), std::end(values)));
        STATIC_REQUIRE_FALSE(any_gt5(std::begin(values), std::end(values)));
        REQUIRE_FALSE(any_gt5(std::begin(values), std::end(values)));
    }
}

SCENARIO("none_of is right curried")
{
    SECTION("when called with range, predicate and projection, it calls "
            "ranges::none_of directly")
    {
        STATIC_REQUIRE(
            composer::none_of(values, composer::greater(5), &numname::num));
        REQUIRE(composer::none_of(values, composer::greater(5), &numname::num));
        STATIC_REQUIRE_FALSE(
            composer::none_of(values, composer::greater(4), &numname::num));
        REQUIRE_FALSE(
            composer::none_of(values, composer::greater(4), &numname::num));
    }
    SECTION("when called with iterator pair, predicate and projection, it "
            "calls ranges::none_of directly")
    {
        STATIC_REQUIRE(composer::none_of(std::begin(values),
                                         std::end(values),
                                         composer::greater(5),
                                         &numname::num));
        REQUIRE(composer::none_of(std::begin(values),
                                  std::end(values),
                                  composer::greater(5),
                                  &numname::num));
        STATIC_REQUIRE_FALSE(composer::none_of(std::begin(values),
                                               std::end(values),
                                               composer::greater(4),
                                               &numname::num));
        REQUIRE_FALSE(composer::none_of(std::begin(values),
                                        std::end(values),
                                        composer::greater(4),
                                        &numname::num));
    }
    SECTION("when called with range and composed predicate, it calls "
            "ranges::none_of directly")
    {
        STATIC_REQUIRE(
            composer::none_of(values, &numname::num | composer::greater(5)));
        REQUIRE(
            composer::none_of(values, &numname::num | composer::greater(5)));
        STATIC_REQUIRE_FALSE(
            composer::none_of(values, &numname::num | composer::greater(4)));
        REQUIRE_FALSE(
            composer::none_of(values, &numname::num | composer::greater(4)));
    }
    SECTION("when called with iterator pair and composed predicate, it calls "
            "ranges::none_of directly")
    {
        STATIC_REQUIRE(composer::none_of(std::begin(values),
                                         std::end(values),
                                         &numname::num | composer::greater(5)));
        REQUIRE(composer::none_of(std::begin(values),
                                  std::end(values),
                                  &numname::num | composer::greater(5)));
        STATIC_REQUIRE_FALSE(
            composer::none_of(std::begin(values),
                              std::end(values),
                              &numname::num | composer::greater(4)));
        REQUIRE_FALSE(composer::none_of(std::begin(values),
                                        std::end(values),
                                        &numname::num | composer::greater(4)));
    }
    SECTION("the result of none_of(projection) is callable with range and "
            "predicate for call to ranges::none_of")
    {
        constexpr auto all_nums = composer::none_of(&numname::num);
        STATIC_REQUIRE(all_nums(values, composer::greater(5)));
        REQUIRE(all_nums(values, composer::greater(5)));
        STATIC_REQUIRE_FALSE(all_nums(values, composer::greater(4)));
        REQUIRE_FALSE(all_nums(values, composer::greater(4)));
    }
    SECTION(
        "the result of none_of(projection) is callable with iterator pair and "
        "predicate for call to ranges::none_of")
    {
        constexpr auto all_nums = composer::none_of(&numname::num);
        STATIC_REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater(5)));
        REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater(5)));
        STATIC_REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater(4)));
        REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater(4)));
    }
    SECTION("the result of none_of(composed predicate) is callable with range "
            "for call to ranges::none_of")
    {
        constexpr auto any_gt5
            = composer::none_of(&numname::num | composer::greater(5));
        constexpr auto any_gt4
            = composer::none_of(&numname::num | composer::greater(4));
        STATIC_REQUIRE(any_gt5(values));
        REQUIRE(any_gt5(values));
        STATIC_REQUIRE_FALSE(any_gt4(values));
        REQUIRE_FALSE(any_gt4(values));
    }
    SECTION("the result of none_of(composed predicate) is callable with "
            "iterator pair for call to ranges::none_of")
    {
        constexpr auto any_gt5
            = composer::none_of(&numname::num | composer::greater(5));
        constexpr auto any_gt4
            = composer::none_of(&numname::num | composer::greater(4));
        STATIC_REQUIRE(any_gt5(std::begin(values), std::end(values)));
        REQUIRE(any_gt5(std::begin(values), std::end(values)));
        STATIC_REQUIRE_FALSE(any_gt4(std::begin(values), std::end(values)));
        REQUIRE_FALSE(any_gt4(std::begin(values), std::end(values)));
    }
}

SCENARIO("for_each is right curried")
{
    static constexpr auto append_to_string
        = [](auto& s) { return [&s](int x) { s += std::to_string(x); }; };
    SECTION("calling for_each with range, function and projection calls "
            "ranges::for_each directly")
    {
        std::string result;
        composer::for_each(values, append_to_string(result), &numname::num);
        REQUIRE(result == "12345");
    }
    SECTION("calling for_each with iterator pair, function and projection "
            "calls ranges::for_each directly")
    {
        std::string result;
        composer::for_each(std::begin(values),
                           std::end(values),
                           append_to_string(result),
                           &numname::num);
        REQUIRE(result == "12345");
    }
    SECTION("calling for_each with range and composed function calls "
            "ranges::for_each directly")
    {
        std::string result;
        composer::for_each(
            values, composer::mem_fn(&numname::num) | append_to_string(result));
        REQUIRE(result == "12345");
    }
    SECTION("calling for_each with iterator pair and composed function calls "
            "ranges::for_each directly")
    {
        std::string result;
        composer::for_each(
            values, composer::mem_fn(&numname::num) | append_to_string(result));
        REQUIRE(result == "12345");
    }
    SECTION("calling for_each with composed function returns object which "
            "calls ranges::for_each on range")
    {
        std::string result;
        auto append_ints_from = composer::for_each(
            composer::mem_fn(&numname::num) | append_to_string(result));
        append_ints_from(values);
        REQUIRE(result == "12345");
    }
    SECTION("calling for_each with composed function returns object which "
            "calls ranges::for_each on iterator pair")
    {
        std::string result;
        auto append_ints_from = composer::for_each(
            composer::mem_fn(&numname::num) | append_to_string(result));
        append_ints_from(std::begin(values), std::end(values));
        REQUIRE(result == "12345");
    }
}
