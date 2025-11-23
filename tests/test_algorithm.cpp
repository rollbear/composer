#include <composer/algorithm.hpp>
#include <composer/functional.hpp>
#include <composer/ranges.hpp>
#include <composer/transform_args.hpp>

#include <catch2/catch_test_macros.hpp>

#include <array>
#include <cmath>

namespace {
struct numname {
    int num;
    std::string_view name;
};

constexpr std::array<numname, 5> values = {
    { { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } }
};

} // namespace

SCENARIO("all_of is back binding")
{
    SECTION("when called with range, predicate and projection, it calls "
            "ranges::all_of directly")
    {
        STATIC_REQUIRE(
            composer::all_of(values, composer::greater_than(0), &numname::num));
        REQUIRE(
            composer::all_of(values, composer::greater_than(0), &numname::num));
        STATIC_REQUIRE_FALSE(
            composer::all_of(values, composer::greater_than(1), &numname::num));
        REQUIRE_FALSE(
            composer::all_of(values, composer::greater_than(1), &numname::num));
    }
    SECTION("when called with iterator pair, predicate and projection, it "
            "calls ranges::all_of directly")
    {
        STATIC_REQUIRE(composer::all_of(std::begin(values),
                                        std::end(values),
                                        composer::greater_than(0),
                                        &numname::num));
        REQUIRE(composer::all_of(std::begin(values),
                                 std::end(values),
                                 composer::greater_than(0),
                                 &numname::num));
        STATIC_REQUIRE_FALSE(composer::all_of(std::begin(values),
                                              std::end(values),
                                              composer::greater_than(1),
                                              &numname::num));
        REQUIRE_FALSE(composer::all_of(std::begin(values),
                                       std::end(values),
                                       composer::greater_than(1),
                                       &numname::num));
    }
    SECTION("when called with range and composed predicate, it calls "
            "ranges::all_of directly")
    {
        STATIC_REQUIRE(composer::all_of(
            values, &numname::num | composer::greater_than(0)));
        REQUIRE(composer::all_of(values,
                                 &numname::num | composer::greater_than(0)));
        STATIC_REQUIRE_FALSE(composer::all_of(
            values, &numname::num | composer::greater_than(1)));
        REQUIRE_FALSE(composer::all_of(
            values, &numname::num | composer::greater_than(1)));
    }
    SECTION("when called with iterator pair and composed predicate, it calls "
            "ranges::all_of directly")
    {
        STATIC_REQUIRE(
            composer::all_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater_than(0)));
        REQUIRE(composer::all_of(std::begin(values),
                                 std::end(values),
                                 &numname::num | composer::greater_than(0)));
        STATIC_REQUIRE_FALSE(
            composer::all_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater_than(1)));
        REQUIRE_FALSE(
            composer::all_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater_than(1)));
    }
    SECTION("the result of all_of(projection) is callable with range and "
            "predicate for call to ranges::all_of")
    {
        constexpr auto all_nums = composer::all_of(&numname::num);
        STATIC_REQUIRE(all_nums(values, composer::greater_than(0)));
        REQUIRE(all_nums(values, composer::greater_than(0)));
        STATIC_REQUIRE_FALSE(all_nums(values, composer::greater_than(1)));
        REQUIRE_FALSE(all_nums(values, composer::greater_than(1)));
    }
    SECTION(
        "the result of all_of(projection) is callable with iterator pair and "
        "predicate for call to ranges::all_of")
    {
        constexpr auto all_nums = composer::all_of(&numname::num);
        STATIC_REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(0)));
        REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(0)));
        STATIC_REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(1)));
        REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(1)));
    }
    SECTION("the result of all_of(composed predicate) is callable with range "
            "for call to ranges::all_of")
    {
        constexpr auto all_pos_ints
            = composer::all_of(&numname::num | composer::greater_than(0));
        constexpr auto all_small_ints
            = composer::all_of(&numname::num | composer::less_than(5));
        STATIC_REQUIRE(all_pos_ints(values));
        REQUIRE(all_pos_ints(values));
        STATIC_REQUIRE_FALSE(all_small_ints(values));
        REQUIRE_FALSE(all_small_ints(values));
    }
    SECTION("the result of all_of(composed predicate) is callable with "
            "iterator pair for call to ranges::all_of")
    {
        constexpr auto all_pos_ints
            = composer::all_of(&numname::num | composer::greater_than(0));
        STATIC_REQUIRE(all_pos_ints(std::begin(values), std::end(values)));
        REQUIRE(all_pos_ints(std::begin(values), std::end(values)));
    }
}

SCENARIO("any_of is back binding")
{
    SECTION("when called with range, predicate and projection, it calls "
            "ranges::any_of directly")
    {
        STATIC_REQUIRE(
            composer::any_of(values, composer::greater_than(4), &numname::num));
        REQUIRE(
            composer::any_of(values, composer::greater_than(4), &numname::num));
        STATIC_REQUIRE_FALSE(
            composer::any_of(values, composer::greater_than(5), &numname::num));
        REQUIRE_FALSE(
            composer::any_of(values, composer::greater_than(5), &numname::num));
    }
    SECTION("when called with iterator pair, predicate and projection, it "
            "calls ranges::any_of directly")
    {
        STATIC_REQUIRE(composer::any_of(std::begin(values),
                                        std::end(values),
                                        composer::greater_than(4),
                                        &numname::num));
        REQUIRE(composer::any_of(std::begin(values),
                                 std::end(values),
                                 composer::greater_than(4),
                                 &numname::num));
        STATIC_REQUIRE_FALSE(composer::any_of(std::begin(values),
                                              std::end(values),
                                              composer::greater_than(5),
                                              &numname::num));
        REQUIRE_FALSE(composer::any_of(std::begin(values),
                                       std::end(values),
                                       composer::greater_than(5),
                                       &numname::num));
    }
    SECTION("when called with range and composed predicate, it calls "
            "ranges::any_of directly")
    {
        STATIC_REQUIRE(composer::any_of(
            values, &numname::num | composer::greater_than(4)));
        REQUIRE(composer::any_of(values,
                                 &numname::num | composer::greater_than(4)));
        STATIC_REQUIRE_FALSE(composer::any_of(
            values, &numname::num | composer::greater_than(5)));
        REQUIRE_FALSE(composer::any_of(
            values, &numname::num | composer::greater_than(5)));
    }
    SECTION("when called with iterator pair and composed predicate, it calls "
            "ranges::any_of directly")
    {
        STATIC_REQUIRE(
            composer::any_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater_than(4)));
        REQUIRE(composer::any_of(std::begin(values),
                                 std::end(values),
                                 &numname::num | composer::greater_than(4)));
        STATIC_REQUIRE_FALSE(
            composer::any_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater_than(5)));
        REQUIRE_FALSE(
            composer::any_of(std::begin(values),
                             std::end(values),
                             &numname::num | composer::greater_than(5)));
    }
    SECTION("the result of any_of(projection) is callable with range and "
            "predicate for call to ranges::any_of")
    {
        constexpr auto all_nums = composer::any_of(&numname::num);
        STATIC_REQUIRE(all_nums(values, composer::greater_than(4)));
        REQUIRE(all_nums(values, composer::greater_than(4)));
        STATIC_REQUIRE_FALSE(all_nums(values, composer::greater_than(5)));
        REQUIRE_FALSE(all_nums(values, composer::greater_than(5)));
    }
    SECTION(
        "the result of any_of(projection) is callable with iterator pair and "
        "predicate for call to ranges::any_of")
    {
        constexpr auto all_nums = composer::any_of(&numname::num);
        STATIC_REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(4)));
        REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(4)));
        STATIC_REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(5)));
        REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(5)));
    }
    SECTION("the result of any_of(composed predicate) is callable with range "
            "for call to ranges::any_of")
    {
        constexpr auto any_gt4
            = composer::any_of(&numname::num | composer::greater_than(4));
        constexpr auto any_gt5
            = composer::any_of(&numname::num | composer::greater_than(5));
        STATIC_REQUIRE(any_gt4(values));
        REQUIRE(any_gt4(values));
        STATIC_REQUIRE_FALSE(any_gt5(values));
        REQUIRE_FALSE(any_gt5(values));
    }
    SECTION("the result of any_of(composed predicate) is callable with "
            "iterator pair for call to ranges::any_of")
    {
        constexpr auto any_gt4
            = composer::any_of(&numname::num | composer::greater_than(4));
        constexpr auto any_gt5
            = composer::any_of(&numname::num | composer::greater_than(5));
        STATIC_REQUIRE(any_gt4(std::begin(values), std::end(values)));
        REQUIRE(any_gt4(std::begin(values), std::end(values)));
        STATIC_REQUIRE_FALSE(any_gt5(std::begin(values), std::end(values)));
        REQUIRE_FALSE(any_gt5(std::begin(values), std::end(values)));
    }
}

SCENARIO("none_of is back binding")
{
    SECTION("when called with range, predicate and projection, it calls "
            "ranges::none_of directly")
    {
        STATIC_REQUIRE(composer::none_of(
            values, composer::greater_than(5), &numname::num));
        REQUIRE(composer::none_of(
            values, composer::greater_than(5), &numname::num));
        STATIC_REQUIRE_FALSE(composer::none_of(
            values, composer::greater_than(4), &numname::num));
        REQUIRE_FALSE(composer::none_of(
            values, composer::greater_than(4), &numname::num));
    }
    SECTION("when called with iterator pair, predicate and projection, it "
            "calls ranges::none_of directly")
    {
        STATIC_REQUIRE(composer::none_of(std::begin(values),
                                         std::end(values),
                                         composer::greater_than(5),
                                         &numname::num));
        REQUIRE(composer::none_of(std::begin(values),
                                  std::end(values),
                                  composer::greater_than(5),
                                  &numname::num));
        STATIC_REQUIRE_FALSE(composer::none_of(std::begin(values),
                                               std::end(values),
                                               composer::greater_than(4),
                                               &numname::num));
        REQUIRE_FALSE(composer::none_of(std::begin(values),
                                        std::end(values),
                                        composer::greater_than(4),
                                        &numname::num));
    }
    SECTION("when called with range and composed predicate, it calls "
            "ranges::none_of directly")
    {
        STATIC_REQUIRE(composer::none_of(
            values, &numname::num | composer::greater_than(5)));
        REQUIRE(composer::none_of(values,
                                  &numname::num | composer::greater_than(5)));
        STATIC_REQUIRE_FALSE(composer::none_of(
            values, &numname::num | composer::greater_than(4)));
        REQUIRE_FALSE(composer::none_of(
            values, &numname::num | composer::greater_than(4)));
    }
    SECTION("when called with iterator pair and composed predicate, it calls "
            "ranges::none_of directly")
    {
        STATIC_REQUIRE(
            composer::none_of(std::begin(values),
                              std::end(values),
                              &numname::num | composer::greater_than(5)));
        REQUIRE(composer::none_of(std::begin(values),
                                  std::end(values),
                                  &numname::num | composer::greater_than(5)));
        STATIC_REQUIRE_FALSE(
            composer::none_of(std::begin(values),
                              std::end(values),
                              &numname::num | composer::greater_than(4)));
        REQUIRE_FALSE(
            composer::none_of(std::begin(values),
                              std::end(values),
                              &numname::num | composer::greater_than(4)));
    }
    SECTION("the result of none_of(projection) is callable with range and "
            "predicate for call to ranges::none_of")
    {
        constexpr auto all_nums = composer::none_of(&numname::num);
        STATIC_REQUIRE(all_nums(values, composer::greater_than(5)));
        REQUIRE(all_nums(values, composer::greater_than(5)));
        STATIC_REQUIRE_FALSE(all_nums(values, composer::greater_than(4)));
        REQUIRE_FALSE(all_nums(values, composer::greater_than(4)));
    }
    SECTION(
        "the result of none_of(projection) is callable with iterator pair and "
        "predicate for call to ranges::none_of")
    {
        constexpr auto all_nums = composer::none_of(&numname::num);
        STATIC_REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(5)));
        REQUIRE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(5)));
        STATIC_REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(4)));
        REQUIRE_FALSE(all_nums(
            std::begin(values), std::end(values), composer::greater_than(4)));
    }
    SECTION("the result of none_of(composed predicate) is callable with range "
            "for call to ranges::none_of")
    {
        constexpr auto any_gt5
            = composer::none_of(&numname::num | composer::greater_than(5));
        constexpr auto any_gt4
            = composer::none_of(&numname::num | composer::greater_than(4));
        STATIC_REQUIRE(any_gt5(values));
        REQUIRE(any_gt5(values));
        STATIC_REQUIRE_FALSE(any_gt4(values));
        REQUIRE_FALSE(any_gt4(values));
    }
    SECTION("the result of none_of(composed predicate) is callable with "
            "iterator pair for call to ranges::none_of")
    {
        constexpr auto any_gt5
            = composer::none_of(&numname::num | composer::greater_than(5));
        constexpr auto any_gt4
            = composer::none_of(&numname::num | composer::greater_than(4));
        STATIC_REQUIRE(any_gt5(std::begin(values), std::end(values)));
        REQUIRE(any_gt5(std::begin(values), std::end(values)));
        STATIC_REQUIRE_FALSE(any_gt4(std::begin(values), std::end(values)));
        REQUIRE_FALSE(any_gt4(std::begin(values), std::end(values)));
    }
}

SCENARIO("for_each is back binding")
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

SCENARIO("for_each_n is back binding")
{
    static constexpr auto append_to_string
        = [](auto& s) { return [&s](int x) { s += std::to_string(x); }; };
    SECTION("calling for_each_n with iterator, count, function and projection "
            "calls "
            "ranges::for_each_n directly")
    {
        std::string result;
        composer::for_each_n(std::next(values.begin()),
                             3,
                             append_to_string(result),
                             &numname::num);
        REQUIRE(result == "234");
    }
    SECTION(
        "calling for_each_n with iterator, count and composed function calls "
        "ranges::for_each_n directly")
    {
        std::string result;
        composer::for_each_n(std::next(values.begin()),
                             3,
                             composer::mem_fn(&numname::num)
                                 | append_to_string(result));
        REQUIRE(result == "234");
    }
    SECTION("calling for_each_n with composed function returns object which "
            "calls ranges::for_each_n on iterator and count")
    {
        std::string result;
        auto append_ints_from = composer::for_each_n(
            composer::mem_fn(&numname::num) | append_to_string(result));
        append_ints_from(std::next(values.begin()), 3);
        REQUIRE(result == "234");
    }
    SECTION("calling for_each_n with composed function returns object which "
            "calls ranges::for_each_n when called with count and then with "
            "iterator")
    {
        std::string result;
        auto append_ints_from = composer::for_each_n(
            composer::mem_fn(&numname::num) | append_to_string(result));
        auto append_3_ints_from = append_ints_from(3);
        append_3_ints_from(std::next(values.begin()));
        REQUIRE(result == "234");
    }
}

SCENARIO("count is back binding")
{
    SECTION("calling count with range, value and projection calls "
            "ranges::count directly")
    {
        STATIC_REQUIRE(
            composer::count(values, 3, composer::mem_fn(&numname::num)) == 1);
        STATIC_REQUIRE(
            composer::count(values, 6, composer::mem_fn(&numname::num)) == 0);
        REQUIRE(composer::count(values, 3, composer::mem_fn(&numname::num))
                == 1);
        REQUIRE(composer::count(values, 6, composer::mem_fn(&numname::num))
                == 0);
    }
    SECTION("calling count with iterator_pair, value and projection calls "
            "ranges::count directly")
    {
        STATIC_REQUIRE(composer::count(std::begin(values),
                                       std::end(values),
                                       3,
                                       composer::mem_fn(&numname::num))
                       == 1);
        STATIC_REQUIRE(composer::count(std::begin(values),
                                       std::end(values),
                                       6,
                                       composer::mem_fn(&numname::num))
                       == 0);
        REQUIRE(composer::count(std::begin(values),
                                std::end(values),
                                3,
                                composer::mem_fn(&numname::num))
                == 1);
        REQUIRE(composer::count(std::begin(values),
                                std::end(values),
                                6,
                                composer::mem_fn(&numname::num))
                == 0);
    }
    SECTION(
        "calling count wtih value and projection returns callable with range")
    {
        constexpr auto num_3s = composer::count(3, &numname::num);
        constexpr auto num_6s = composer::count(6, &numname::num);
        STATIC_REQUIRE(num_3s(values) == 1);
        STATIC_REQUIRE(num_6s(values) == 0);
        REQUIRE(num_3s(values) == 1);
        REQUIRE(num_6s(values) == 0);
    }
    SECTION("calling count wtih value and projection returns callable with "
            "iterator pair")
    {
        constexpr auto num_3s = composer::count(3, &numname::num);
        constexpr auto num_6s = composer::count(6, &numname::num);
        STATIC_REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        STATIC_REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
        REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
    }
    SECTION(
        "calling count with projection, returns callable for number and range")
    {
        constexpr auto num_ints = composer::count(&numname::num);
        constexpr auto num_3s = num_ints(3);
        constexpr auto num_6s = num_ints(6);
        STATIC_REQUIRE(num_3s(values) == 1);
        STATIC_REQUIRE(num_6s(values) == 0);
        REQUIRE(num_3s(values) == 1);
        REQUIRE(num_6s(values) == 0);
    }
    SECTION("calling count with projection, returns callable for number and "
            "iterator pair")
    {
        constexpr auto num_ints = composer::count(&numname::num);
        constexpr auto num_3s = num_ints(3);
        constexpr auto num_6s = num_ints(6);
        STATIC_REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        STATIC_REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
        REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
    }
}

SCENARIO("count_if is back binding")
{
    SECTION("calling count_if with range, predicate and projection calls "
            "ranges::count_if directly")
    {
        STATIC_REQUIRE(composer::count_if(values,
                                          composer::greater_than(3),
                                          composer::mem_fn(&numname::num))
                       == 2);
        STATIC_REQUIRE(composer::count_if(values,
                                          composer::equal_to(0),
                                          composer::mem_fn(&numname::num))
                       == 0);
        REQUIRE(composer::count_if(values,
                                   composer::greater_than(3),
                                   composer::mem_fn(&numname::num))
                == 2);
        REQUIRE(composer::count_if(values,
                                   composer::greater_than(5),
                                   composer::mem_fn(&numname::num))
                == 0);
    }
    SECTION(
        "calling count_if with iterator_pair, predicate and projection calls "
        "ranges::count_if directly")
    {
        STATIC_REQUIRE(composer::count_if(std::begin(values),
                                          std::end(values),
                                          composer::greater_than(3),
                                          composer::mem_fn(&numname::num))
                       == 2);
        STATIC_REQUIRE(composer::count_if(std::begin(values),
                                          std::end(values),
                                          composer::greater_than(5),
                                          composer::mem_fn(&numname::num))
                       == 0);
        REQUIRE(composer::count_if(std::begin(values),
                                   std::end(values),
                                   composer::greater_than(3),
                                   composer::mem_fn(&numname::num))
                == 2);
        REQUIRE(composer::count_if(std::begin(values),
                                   std::end(values),
                                   composer::greater_than(5),
                                   composer::mem_fn(&numname::num))
                == 0);
    }
    SECTION("calling count_if wtih predicate and projection returns callable "
            "with range")
    {
        constexpr auto num_3s
            = composer::count_if(composer::equal_to(3), &numname::num);
        constexpr auto num_6s
            = composer::count_if(composer::equal_to(6), &numname::num);
        STATIC_REQUIRE(num_3s(values) == 1);
        STATIC_REQUIRE(num_6s(values) == 0);
        REQUIRE(num_3s(values) == 1);
        REQUIRE(num_6s(values) == 0);
    }
    SECTION(
        "calling count_if wtih predicate and projection returns callable with "
        "iterator pair")
    {
        constexpr auto num_3s
            = composer::count_if(composer::equal_to(3), &numname::num);
        constexpr auto num_6s
            = composer::count_if(composer::equal_to(6), &numname::num);
        STATIC_REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        STATIC_REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
        REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
    }
    SECTION("calling count_if with projection, returns callable for predicate "
            "and range")
    {
        constexpr auto num_ints = composer::count_if(&numname::num);
        constexpr auto num_3s = num_ints(composer::equal_to(3));
        constexpr auto num_6s = num_ints(composer::equal_to(6));
        STATIC_REQUIRE(num_3s(values) == 1);
        STATIC_REQUIRE(num_6s(values) == 0);
        REQUIRE(num_3s(values) == 1);
        REQUIRE(num_6s(values) == 0);
    }
    SECTION("calling count_if with projection, returns callable for number and "
            "iterator pair")
    {
        constexpr auto num_ints = composer::count_if(&numname::num);
        constexpr auto num_3s = num_ints(composer::equal_to(3));
        constexpr auto num_6s = num_ints(composer::equal_to(6));
        STATIC_REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        STATIC_REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
        REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
    }
    SECTION(
        "calling count_if with composed predicate returns callable for range")
    {
        constexpr auto num_3s
            = composer::count_if(&numname::num | composer::equal_to(3));
        constexpr auto num_6s
            = composer::count_if(&numname::num | composer::equal_to(6));
        STATIC_REQUIRE(num_3s(values) == 1);
        STATIC_REQUIRE(num_6s(values) == 0);
        REQUIRE(num_3s(values) == 1);
        REQUIRE(num_6s(values) == 0);
    }
    SECTION("calling count_if with composed predicate returns callable for "
            "iterator pair")
    {
        constexpr auto num_3s
            = composer::count_if(&numname::num | composer::equal_to(3));
        constexpr auto num_6s
            = composer::count_if(&numname::num | composer::equal_to(6));
        STATIC_REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        STATIC_REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
        REQUIRE(num_3s(std::begin(values), std::end(values)) == 1);
        REQUIRE(num_6s(std::begin(values), std::end(values)) == 0);
    }
}

SCENARIO("find is back binding")
{
    SECTION("calling find with a range, a value and a projection calls "
            "ranges::find if directly")
    {
        STATIC_REQUIRE(composer::find(values, 2, &numname::num)->name == "two");
        REQUIRE(composer::find(values, 4, &numname::num)->name == "four");
    }
    SECTION("calling find with an iterator pair, a value and a projection "
            "calls ranges::find if directly")
    {
        STATIC_REQUIRE(
            composer::find(
                std::begin(values), std::end(values), 2, &numname::num)
                ->name
            == "two");
        REQUIRE(composer::find(
                    std::begin(values), std::end(values), 4, &numname::num)
                    ->name
                == "four");
    }
    SECTION("calling find with a value and a projection returns a callable for "
            "a range")
    {
        constexpr auto num_is_2 = composer::find(2, &numname::num);
        STATIC_REQUIRE(num_is_2(values)->name == "two");
        REQUIRE(num_is_2(values)->name == "two");
    }
    SECTION("calling find with a value and a projection returns a callable for "
            "an iterator pair")
    {
        constexpr auto num_is_2 = composer::find(2, &numname::num);
        STATIC_REQUIRE(num_is_2(std::begin(values), std::end(values))->name
                       == "two");
        REQUIRE(num_is_2(std::begin(values), std::end(values))->name == "two");
    }
}

SCENARIO("find_if is back binding")
{
    SECTION("calling find_if with a range, a predicate and a projection calls "
            "ranges::find_if if directly")
    {
        STATIC_REQUIRE(
            composer::find_if(values, composer::equal_to(2), &numname::num)
                ->name
            == "two");
        REQUIRE(composer::find_if(values, composer::equal_to(4), &numname::num)
                    ->name
                == "four");
    }
    SECTION(
        "calling find_if with an iterator pair, a predicate and a projection "
        "calls ranges::find_if if directly")
    {
        STATIC_REQUIRE(composer::find_if(std::begin(values),
                                         std::end(values),
                                         composer::equal_to(2),
                                         &numname::num)
                           ->name
                       == "two");
        REQUIRE(composer::find_if(std::begin(values),
                                  std::end(values),
                                  composer::equal_to(4),
                                  &numname::num)
                    ->name
                == "four");
    }
    SECTION("calling find_if with a range, and a composed predicate calls "
            "ranges::find_if if directly")
    {
        STATIC_REQUIRE(
            composer::find_if(values, &numname::num | composer::equal_to(2))
                ->name
            == "two");
        REQUIRE(composer::find_if(values,
                                  composer::mem_fn(&numname::num)
                                      | composer::equal_to(4))
                    ->name
                == "four");
    }
    SECTION("calling find_if with an iterator pair, and a composed predicate "
            "calls ranges::find_if if directly")
    {
        STATIC_REQUIRE(composer::find_if(std::begin(values),
                                         std::end(values),
                                         &numname::num | composer::equal_to(2))
                           ->name
                       == "two");
        REQUIRE(composer::find_if(std::begin(values),
                                  std::end(values),
                                  composer::mem_fn(&numname::num)
                                      | composer::equal_to(4))
                    ->name
                == "four");
    }
    SECTION("calling find_if with a predicate and a projection returns a "
            "callable for "
            "a range")
    {
        constexpr auto num_is_2
            = composer::find_if(composer::equal_to(2), &numname::num);
        STATIC_REQUIRE(num_is_2(values)->name == "two");
        REQUIRE(num_is_2(values)->name == "two");
    }
    SECTION("calling find_if with a predicate and a projection returns a "
            "callable for "
            "an iterator pair")
    {
        constexpr auto num_is_2
            = composer::find_if(composer::equal_to(2), &numname::num);
        STATIC_REQUIRE(num_is_2(std::begin(values), std::end(values))->name
                       == "two");
        REQUIRE(num_is_2(std::begin(values), std::end(values))->name == "two");
    }
    SECTION("calling find_if with a composed predicatereturns a callable for "
            "a range")
    {
        constexpr auto num_is_2
            = composer::find_if(&numname::num | composer::equal_to(2));
        STATIC_REQUIRE(num_is_2(values)->name == "two");
        REQUIRE(num_is_2(values)->name == "two");
    }
    SECTION("calling find_if with a composed predicate returns a callable for "
            "an iterator pair")
    {
        constexpr auto num_is_2 = composer::find_if(
            composer::mem_fn(&numname::num) | composer::equal_to(2));
        STATIC_REQUIRE(num_is_2(std::begin(values), std::end(values))->name
                       == "two");
        REQUIRE(num_is_2(std::begin(values), std::end(values))->name == "two");
    }
}

SCENARIO("find_if_not is back binding")
{
    SECTION(
        "calling find_if_not with a range, a predicate and a projection calls "
        "ranges::find_if_not if directly")
    {
        STATIC_REQUIRE(composer::find_if_not(
                           values, composer::not_equal_to(2), &numname::num)
                           ->name
                       == "two");
        REQUIRE(composer::find_if_not(
                    values, composer::not_equal_to(4), &numname::num)
                    ->name
                == "four");
    }
    SECTION("calling find_if_not with an iterator pair, a predicate and a "
            "projection "
            "calls ranges::find_if_not if directly")
    {
        STATIC_REQUIRE(composer::find_if_not(std::begin(values),
                                             std::end(values),
                                             composer::not_equal_to(2),
                                             &numname::num)
                           ->name
                       == "two");
        REQUIRE(composer::find_if_not(std::begin(values),
                                      std::end(values),
                                      composer::not_equal_to(4),
                                      &numname::num)
                    ->name
                == "four");
    }
    SECTION("calling find_if_not with a range, and a composed predicate calls "
            "ranges::find_if_not if directly")
    {
        STATIC_REQUIRE(composer::find_if_not(
                           values, &numname::num | composer::not_equal_to(2))
                           ->name
                       == "two");
        REQUIRE(composer::find_if_not(values,
                                      composer::mem_fn(&numname::num)
                                          | composer::not_equal_to(4))
                    ->name
                == "four");
    }
    SECTION(
        "calling find_if_not with an iterator pair, and a composed predicate "
        "calls ranges::find_if_not if directly")
    {
        STATIC_REQUIRE(
            composer::find_if_not(std::begin(values),
                                  std::end(values),
                                  &numname::num | composer::not_equal_to(2))
                ->name
            == "two");
        REQUIRE(composer::find_if_not(std::begin(values),
                                      std::end(values),
                                      composer::mem_fn(&numname::num)
                                          | composer::not_equal_to(4))
                    ->name
                == "four");
    }
    SECTION("calling find_if_not with a predicate and a projection returns a "
            "callable for "
            "a range")
    {
        constexpr auto num_is_2
            = composer::find_if_not(composer::not_equal_to(2), &numname::num);
        STATIC_REQUIRE(num_is_2(values)->name == "two");
        REQUIRE(num_is_2(values)->name == "two");
    }
    SECTION("calling find_if_not with a predicate and a projection returns a "
            "callable for "
            "an iterator pair")
    {
        constexpr auto num_is_2
            = composer::find_if_not(composer::not_equal_to(2), &numname::num);
        STATIC_REQUIRE(num_is_2(std::begin(values), std::end(values))->name
                       == "two");
        REQUIRE(num_is_2(std::begin(values), std::end(values))->name == "two");
    }
    SECTION(
        "calling find_if_not with a composed predicatereturns a callable for "
        "a range")
    {
        constexpr auto num_is_2
            = composer::find_if_not(&numname::num | composer::not_equal_to(2));
        STATIC_REQUIRE(num_is_2(values)->name == "two");
        REQUIRE(num_is_2(values)->name == "two");
    }
    SECTION(
        "calling find_if_not with a composed predicate returns a callable for "
        "an iterator pair")
    {
        constexpr auto num_is_2 = composer::find_if_not(
            composer::mem_fn(&numname::num) | composer::not_equal_to(2));
        STATIC_REQUIRE(num_is_2(std::begin(values), std::end(values))->name
                       == "two");
        REQUIRE(num_is_2(std::begin(values), std::end(values))->name == "two");
    }
}

SCENARIO("find_last")
{
    SECTION("calling find_last with a range, a value and a projection calls "
            "ranges::find_last directly")
    {
        constexpr auto tail = composer::find_last(values, 3, &numname::num);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last with an iterator pair, a value and a projection "
            "calls ranges::find_last directly")
    {
        constexpr auto tail = composer::find_last(
            std::begin(values), std::end(values), 3, &numname::num);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last with a value and a projection returns a "
            "callable that takes a range")
    {
        constexpr auto last_num_3 = composer::find_last(3, &numname::num);
        constexpr auto tail = last_num_3(values);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last with a value and a projection returns a "
            "callable that takes an iterator pair")
    {
        constexpr auto last_num_3 = composer::find_last(3, &numname::num);
        constexpr auto tail = last_num_3(std::begin(values), std::end(values));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
}

SCENARIO("find_last_if")
{
    SECTION(
        "calling find_last_if with a range, a predicate and a projection calls "
        "ranges::find_last_if directly")
    {
        constexpr auto tail = composer::find_last_if(
            values, composer::equal_to(3), &numname::num);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if with an iterator pair, a predicate and a "
            "projection "
            "calls ranges::find_last_if directly")
    {
        constexpr auto tail = composer::find_last_if(std::begin(values),
                                                     std::end(values),
                                                     composer::equal_to(3),
                                                     &numname::num);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if with a range and a composed predicate calls "
            "ranges::find_last_if directly")
    {
        constexpr auto tail = composer::find_last_if(
            values, &numname::num | composer::equal_to(3));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION(
        "calling find_last_if with an iterator pair and a composed predicate "
        "calls ranges::find_last_if directly")
    {
        constexpr auto tail
            = composer::find_last_if(std::begin(values),
                                     std::end(values),
                                     &numname::num | composer::equal_to(3));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if with a predicate and a projection returns a "
            "callable that takes a range")
    {
        constexpr auto last_num_3
            = composer::find_last_if(composer::equal_to(3), &numname::num);
        constexpr auto tail = last_num_3(values);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if with a predicate and a projection returns a "
            "callable that takes an iterator pair")
    {
        constexpr auto last_num_3
            = composer::find_last_if(composer::equal_to(3), &numname::num);
        constexpr auto tail = last_num_3(std::begin(values), std::end(values));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if with a composed predicate returns a "
            "callable that takes a range")
    {
        constexpr auto last_num_3
            = composer::find_last_if(&numname::num | composer::equal_to(3));
        constexpr auto tail = last_num_3(values);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if with a composed predicate returns a "
            "callable that takes an iterator pair")
    {
        constexpr auto last_num_3
            = composer::find_last_if(&numname::num | composer::equal_to(3));
        constexpr auto tail = last_num_3(std::begin(values), std::end(values));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
}

SCENARIO("find_last_if_not")
{
    SECTION("calling find_last_if_not with a range, a predicate and a "
            "projection calls "
            "ranges::find_last_if_not directly")
    {
        constexpr auto tail = composer::find_last_if_not(
            values, composer::not_equal_to(3), &numname::num);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if_not with an iterator pair, a predicate and a "
            "projection "
            "calls ranges::find_last_if_not directly")
    {
        constexpr auto tail
            = composer::find_last_if_not(std::begin(values),
                                         std::end(values),
                                         composer::not_equal_to(3),
                                         &numname::num);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION(
        "calling find_last_if_not with a range and a composed predicate calls "
        "ranges::find_last_if_not directly")
    {
        constexpr auto tail = composer::find_last_if_not(
            values, &numname::num | composer::not_equal_to(3));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if_not with an iterator pair and a composed "
            "predicate "
            "calls ranges::find_last_if_not directly")
    {
        constexpr auto tail = composer::find_last_if_not(
            std::begin(values),
            std::end(values),
            &numname::num | composer::not_equal_to(3));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION(
        "calling find_last_if_not with a predicate and a projection returns a "
        "callable that takes a range")
    {
        constexpr auto last_num_3 = composer::find_last_if_not(
            composer::not_equal_to(3), &numname::num);
        constexpr auto tail = last_num_3(values);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION(
        "calling find_last_if_not with a predicate and a projection returns a "
        "callable that takes an iterator pair")
    {
        constexpr auto last_num_3 = composer::find_last_if_not(
            composer::not_equal_to(3), &numname::num);
        constexpr auto tail = last_num_3(std::begin(values), std::end(values));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if_not with a composed predicate returns a "
            "callable that takes a range")
    {
        constexpr auto last_num_3 = composer::find_last_if_not(
            &numname::num | composer::not_equal_to(3));
        constexpr auto tail = last_num_3(values);
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
    SECTION("calling find_last_if_not with a composed predicate returns a "
            "callable that takes an iterator pair")
    {
        constexpr auto last_num_3 = composer::find_last_if_not(
            &numname::num | composer::not_equal_to(3));
        constexpr auto tail = last_num_3(std::begin(values), std::end(values));
        STATIC_REQUIRE(std::begin(tail)->num == 3);
        STATIC_REQUIRE(std::end(tail) == std::end(values));
        REQUIRE(std::begin(tail)->num == 3);
        REQUIRE(std::end(tail) == std::end(values));
    }
}

SCENARIO("find_end is back binding")
{
    SECTION("calling find_end with two ranges calls ranges::find_end directly")
    {
        static constexpr std::array haystack = { 1, 2, 3, 4, 1, 2, 5 };
        static constexpr std::array needle = { 1, 2 };
        STATIC_REQUIRE(composer::find_end(haystack, needle).begin()
                       == haystack.begin() + 4);
        STATIC_REQUIRE(composer::find_end(haystack, needle).end()
                       == haystack.begin() + 6);
        REQUIRE(composer::find_end(haystack, needle).begin()
                == haystack.begin() + 4);
        REQUIRE(composer::find_end(haystack, needle).end()
                == haystack.begin() + 6);
    }
    SECTION("calling find_end with a needle range returns a callable for a "
            "haystack")
    {
        static constexpr std::array haystack = { 1, 2, 3, 4, 1, 2, 5 };
        static constexpr std::array needle = { 1, 2 };
        constexpr auto find_end_12 = composer::find_end(needle);

        STATIC_REQUIRE(find_end_12(haystack).begin() == haystack.begin() + 4);
        STATIC_REQUIRE(find_end_12(haystack).end() == haystack.begin() + 6);
        REQUIRE(find_end_12(haystack).begin() == haystack.begin() + 4);
        REQUIRE(find_end_12(haystack).end() == haystack.begin() + 6);
    }
    SECTION(
        "callning find_end with a predicate returns a callable for two ranges")
    {
        static constexpr std::array haystack = { 1, 2, 3, 4, 1, 2, 5 };
        static constexpr std::array needle = { -1, -2 };
        constexpr auto find_end_neg
            = composer::find_end([](auto a, auto b) { return a == -b; });
        STATIC_REQUIRE(find_end_neg(haystack, needle).begin()
                       == haystack.begin() + 4);
        STATIC_REQUIRE(find_end_neg(haystack, needle).end()
                       == haystack.begin() + 6);
        REQUIRE(find_end_neg(haystack, needle).begin() == haystack.begin() + 4);
        REQUIRE(find_end_neg(haystack, needle).end() == haystack.begin() + 6);
    }
    SECTION("callning find_end with a needle range and a predicate returns a "
            "callable for a haystack range")
    {
        static constexpr std::array haystack = { 1, 2, 3, 4, 1, 2, 5 };
        static constexpr std::array needle = { -1, -2 };
        constexpr auto find_end_neg_needle = composer::find_end(
            needle, [](auto a, auto b) { return a == -b; });
        STATIC_REQUIRE(find_end_neg_needle(haystack).begin()
                       == haystack.begin() + 4);
        STATIC_REQUIRE((haystack | find_end_neg_needle).begin()
                       == haystack.begin() + 4);
        STATIC_REQUIRE(find_end_neg_needle(haystack).end()
                       == haystack.begin() + 6);
        STATIC_REQUIRE((haystack | find_end_neg_needle).end()
                       == haystack.begin() + 6);
        REQUIRE(find_end_neg_needle(haystack).begin() == haystack.begin() + 4);
        REQUIRE((haystack | find_end_neg_needle).begin()
                == haystack.begin() + 4);
        REQUIRE((haystack | find_end_neg_needle).end() == haystack.begin() + 6);
    }
}

SCENARIO("find_first_of is back binding")
{
    SECTION("calling find_first_of with two ranges calls ranges::find_first_of "
            "directly")
    {
        static constexpr std::array intvalues = { 1, 2, 3, 4, 5, 6 };
        STATIC_REQUIRE(composer::find_first_of(intvalues, std::array{ 4, 5, 6 })
                       == intvalues.begin() + 3);
        REQUIRE(composer::find_first_of(intvalues, std::array{ 4, 5, 6 })
                == intvalues.begin() + 3);
    }
    SECTION("calling find_first_of with two ranges and a predicate calls "
            "ranges::find_first_of directly")
    {
        static constexpr std::array needles = { 5, 6, 4 };
        STATIC_REQUIRE(composer::find_first_of(
                           values,
                           needles,
                           [](const auto& a, auto b) { return a.num == b; })
                       == values.begin() + 3);
        REQUIRE(composer::find_first_of(
                    values,
                    needles,
                    [](const auto& a, auto b) { return a.num == b; })
                == values.begin() + 3);
    }
    SECTION("calling find_first_of with a predicate returns a callable for two "
            "ranges")
    {
        constexpr auto find_first_by_num = composer::find_first_of(
            [](auto& a, auto b) { return a.num == b; });
        static constexpr std::array needles = { 5, 6, 4 };
        STATIC_REQUIRE((find_first_by_num(values, needles)
                        | composer::dereference
                        | composer::mem_fn(&numname::name))
                       == "four");
        REQUIRE((find_first_by_num(values, needles) | composer::dereference
                 | composer::mem_fn(&numname::name))
                == "four");
    }
    SECTION("calling find_first_of with a predicate and then with a range, "
            "returns a callable with a range")
    {
        using composer::operator|;
        constexpr auto find_first_by_num = composer::find_first_of(
            [](auto& a, auto b) { return a.num == b; });
        static constexpr std::array needles = { 5, 6, 4 };
        STATIC_REQUIRE((values | find_first_by_num(needles)
                        | composer::dereference | &numname::name)
                       == "four");
        REQUIRE((values | find_first_by_num(needles) | composer::dereference
                 | &numname::name)
                == "four");
    }
}

SCENARIO("adjacent_find is back binding")
{
    static constexpr std::array numbers = { 1, 2, -2, 3, 3, 4, 5 };
    SECTION("adjacent_find called with a range calls ranges::adjacent_find "
            "immediately")
    {
        STATIC_REQUIRE(composer::adjacent_find(numbers) == numbers.begin() + 3);
        REQUIRE(composer::adjacent_find(numbers) == numbers.begin() + 3);
    }
    SECTION("adjacent_find called with a range and a predicate calls "
            "ranges::adjacent_find immediately")
    {
        STATIC_REQUIRE(composer::adjacent_find(
                           numbers, [](auto a, auto b) { return -a == b; })
                       == numbers.begin() + 1);
        REQUIRE(composer::adjacent_find(numbers,
                                        [](auto a, auto b) { return -a == b; })
                == numbers.begin() + 1);
    }
    SECTION("adjacent_find called with a range, a predicate and a projection "
            "calls ranges::adjacent_find immediately")
    {
        STATIC_REQUIRE(
            composer::adjacent_find(numbers,
                                    composer::equal_to,
                                    [](auto a) { return a > 0 ? a : -a; })
            == numbers.begin() + 1);
        REQUIRE(composer::adjacent_find(numbers,
                                        composer::equal_to,
                                        [](auto a) { return a > 0 ? a : -a; })
                == numbers.begin() + 1);
    }
    SECTION(
        "adjacent_find called with a predicate returns a callable for a range")
    {
        constexpr auto abs_adjacent
            = composer::adjacent_find(composer::equal_to);
        STATIC_REQUIRE((numbers | abs_adjacent) == numbers.begin() + 3);
        REQUIRE((numbers | abs_adjacent) == numbers.begin() + 3);
    }
}

SCENARIO("search is back binding")
{
    SECTION("search called with 2 ranges calls ranges::search immediately")
    {
        static constexpr std::array haystack = { 1, 2, 3, 4, 5 };
        static constexpr std::array needle = { 3, 4 };
        STATIC_REQUIRE(composer::search(haystack, needle).begin()
                       == haystack.begin() + 2);
        STATIC_REQUIRE(composer::search(haystack, needle).end()
                       == haystack.begin() + 4);
        REQUIRE(composer::search(haystack, needle).begin()
                == haystack.begin() + 2);
        REQUIRE(composer::search(haystack, needle).end()
                == haystack.begin() + 4);
    }
    SECTION("search called with a needle range returns a callable for a "
            "haystack range")
    {
        static constexpr std::array haystack = { 1, 2, 3, 4, 5 };
        static constexpr std::array needle = { 3, 4 };
        STATIC_REQUIRE((haystack | composer::search(needle)).begin()
                       == haystack.begin() + 2);
        STATIC_REQUIRE((haystack | composer::search(needle)).end()
                       == haystack.begin() + 4);
        REQUIRE((haystack | composer::search(needle)).begin()
                == haystack.begin() + 2);
        REQUIRE((haystack | composer::search(needle)).end()
                == haystack.begin() + 4);
    }
    SECTION("search called with a predicate and a projection returns a "
            "callable for two ranges")
    {
        constexpr auto search_in_numname
            = composer::search(composer::equal_to, &numname::num);

        static constexpr std::array needle = { 3, 4 };
        STATIC_REQUIRE(search_in_numname(values, needle).begin()
                       == values.begin() + 2);
        STATIC_REQUIRE(search_in_numname(values, needle).end()
                       == values.begin() + 4);
        REQUIRE(search_in_numname(values, needle).begin()
                == values.begin() + 2);
        REQUIRE(search_in_numname(values, needle).end() == values.begin() + 4);
    }
    SECTION("search called with a predicate and a projection returns a "
            "callable, when called with a range is callable witha ranges")
    {
        constexpr auto search_in_numname
            = composer::search(composer::equal_to, &numname::num);

        static constexpr std::array needle = { 3, 4 };

        STATIC_REQUIRE((values | search_in_numname(needle)).begin()
                       == values.begin() + 2);
        STATIC_REQUIRE((values | search_in_numname(needle)).end()
                       == values.begin() + 4);
        REQUIRE((values | search_in_numname(needle)).begin()
                == values.begin() + 2);
        REQUIRE((values | search_in_numname(needle)).end()
                == values.begin() + 4);
    }
}

SCENARIO("search_n is back binding")
{
    static constexpr std::array ints = { 1, 2, 2, -3, -4, 2, 2, 2, 5, 6 };

    SECTION("search_n called with a range, a count and a value calls "
            "ranges::search_n immediately")
    {
        STATIC_REQUIRE(composer::search_n(ints, 3, 2).begin()
                       == ints.begin() + 5);
        STATIC_REQUIRE(composer::search_n(ints, 3, 2).end()
                       == ints.begin() + 8);
        REQUIRE(composer::search_n(ints, 3, 2).begin() == ints.begin() + 5);
        REQUIRE(composer::search_n(ints, 3, 2).end() == ints.begin() + 8);
    }
    SECTION("search_n called with a value, a predicate and a projection "
            "returns a callable with a range and a count")
    {
        constexpr auto gt2_numname_range
            = composer::search_n(2, composer::greater_than, &numname::num);
        STATIC_REQUIRE(gt2_numname_range(values, 2).begin()
                       == values.begin() + 2);
        STATIC_REQUIRE(gt2_numname_range(values, 2).end()
                       == values.begin() + 4);
        REQUIRE(gt2_numname_range(values, 2).begin() == values.begin() + 2);
        REQUIRE(gt2_numname_range(values, 2).end() == values.begin() + 4);
    }
    SECTION("search_n called with a value, a predicate and a projection, "
            "called with a count is callable with a range")
    {
        constexpr auto gt2_numname_range
            = composer::search_n(2, composer::greater_than, &numname::num);
        STATIC_REQUIRE((values | gt2_numname_range(2)).begin()
                       == values.begin() + 2);
        STATIC_REQUIRE((values | gt2_numname_range(2)).end()
                       == values.begin() + 4);
        REQUIRE((values | gt2_numname_range(2)).begin() == values.begin() + 2);
        REQUIRE((values | gt2_numname_range(2)).end() == values.begin() + 4);
    }
    SECTION("search_n called with a value, called with a count is callable "
            "with a range")
    {
        constexpr auto search_2s = composer::search_n(2);
        STATIC_REQUIRE((ints | search_2s(3)).begin() == ints.begin() + 5);
        STATIC_REQUIRE((ints | search_2s(3)).end() == ints.begin() + 8);
        REQUIRE((ints | search_2s(3)).begin() == ints.begin() + 5);
        REQUIRE((ints | search_2s(3)).end() == ints.begin() + 8);
    }
}

SCENARIO("contains is back binding")
{
    SECTION("contains called with a range, a value and a projection calls "
            "ranges::contains immediately")
    {
        STATIC_REQUIRE(composer::contains(values, 3, &numname::num));
        STATIC_REQUIRE_FALSE(composer::contains(values, 0, &numname::num));
        REQUIRE(composer::contains(values, 3, &numname::num));
        REQUIRE_FALSE(composer::contains(values, 0, &numname::num));
    }
    SECTION("contains called with a values returns a callable with a range")
    {
        static constexpr std::array ints = { 1, 2, 3, 4, 5 };
        STATIC_REQUIRE(ints | composer::contains(3));
        STATIC_REQUIRE_FALSE(ints | composer::contains(0));
        REQUIRE(ints | composer::contains(3));
        REQUIRE_FALSE(ints | composer::contains(0));
    }
    SECTION("contains called with a projection returns a callable with a value "
            "and a range")
    {
        constexpr auto contains_num = composer::contains(&numname::num);
        STATIC_REQUIRE(values | contains_num(3));
        STATIC_REQUIRE_FALSE(values | contains_num(0));
        REQUIRE(values | contains_num(3));
        REQUIRE_FALSE(values | contains_num(0));
    }
}

SCENARIO("contains_subrange is back binding")
{
    SECTION("calling contains_subrange with two ranges, a predicate and a "
            "projection calls ranges::contains_subrange immediately")
    {
        STATIC_REQUIRE(composer::contains_subrange(
            values, std::array{ 2, 3, 4 }, composer::equal_to, &numname::num));
        STATIC_REQUIRE_FALSE(composer::contains_subrange(
            values, std::array{ 2, 3, 5 }, composer::equal_to, &numname::num));
        REQUIRE(composer::contains_subrange(
            values, std::array{ 2, 3, 4 }, composer::equal_to, &numname::num));
        REQUIRE_FALSE(composer::contains_subrange(
            values, std::array{ 2, 3, 5 }, composer::equal_to, &numname::num));
    }
    SECTION("calling contains_subrange with a predicate and a projection "
            "returns a callable for two ranges")
    {
        constexpr auto contains_subrange_num
            = composer::contains_subrange(composer::equal_to, &numname::num);
        STATIC_REQUIRE(values | contains_subrange_num(std::array{ 2, 3, 4 }));
        STATIC_REQUIRE_FALSE(values
                             | contains_subrange_num(std::array{ 2, 3, 5 }));
        REQUIRE(values | contains_subrange_num(std::array{ 2, 3, 4 }));
        REQUIRE_FALSE(values | contains_subrange_num(std::array{ 2, 3, 5 }));
    }
}

#if defined(__cpp_lib_ranges_starts_ends_with)

SCENARIO("starts_with is back binding")
{
    SECTION("starts_with called with two ranges calls ranges::starts_with "
            "immediately")
    {
        static constexpr std::array ints = { 1, 2, 3, 4, 5 };
        STATIC_REQUIRE(composer::starts_with(ints, std::array{ 1, 2, 3 }));
        STATIC_REQUIRE_FALSE(
            composer::starts_with(ints, std::array{ 1, 2, 4 }));
        REQUIRE(composer::starts_with(ints, std::array{ 1, 2, 3 }));
        REQUIRE_FALSE(composer::starts_with(ints, std::array{ 1, 2, 4 }));
    }
    SECTION("starts_with called with two ranges and a composed predicate calls "
            "ranges::starts_with immediately")
    {
        STATIC_REQUIRE(composer::starts_with(
            values, std::array{ 1, 2, 3 }, [](auto& a, auto b) {
                return a.num == b;
            }));
        STATIC_REQUIRE_FALSE(composer::starts_with(
            values, std::array{ 1, 2, 4 }, [](auto& a, auto b) {
                return a.num == b;
            }));
        REQUIRE(composer::starts_with(
            values, std::array{ 1, 2, 3 }, [](auto& a, auto b) {
                return a.num == b;
            }));
        REQUIRE_FALSE(composer::starts_with(
            values, std::array{ 1, 2, 4 }, [](auto& a, auto b) {
                return a.num == b;
            }));
    }
    SECTION("starts_with called with two ranges, a predicate and a projection "
            "calls ranges::starts_with immediately")
    {
        STATIC_REQUIRE(composer::starts_with(
            values, std::array{ 1, 2, 3 }, composer::equal_to, &numname::num));
        STATIC_REQUIRE_FALSE(composer::starts_with(
            values, std::array{ 1, 2, 4 }, composer::equal_to, &numname::num));
        REQUIRE(composer::starts_with(
            values, std::array{ 1, 2, 3 }, composer::equal_to, &numname::num));
        REQUIRE_FALSE(composer::starts_with(
            values, std::array{ 1, 2, 4 }, composer::equal_to, &numname::num));
    }
    SECTION("starts_with called with a predicate and a projection returns a "
            "callable for two ranges")
    {
        constexpr auto starts_with_num
            = composer::starts_with(composer::equal_to, &numname::num);
        STATIC_REQUIRE(starts_with_num(values, std::array{ 1, 2, 3 }));
        STATIC_REQUIRE_FALSE(starts_with_num(values, std::array{ 1, 2, 4 }));
        REQUIRE(starts_with_num(values, std::array{ 1, 2, 3 }));
        REQUIRE_FALSE(starts_with_num(values, std::array{ 1, 2, 4 }));
    }
    SECTION("starts_with called with a precidace and a projection, called with "
            "a needle range is pipeaaple from a range")
    {
        constexpr auto starts_with_num
            = composer::starts_with(composer::equal_to, &numname::num);
        STATIC_REQUIRE(values | starts_with_num(std::array{ 1, 2, 3 }));
        STATIC_REQUIRE_FALSE(values | starts_with_num(std::array{ 1, 2, 4 }));
        REQUIRE(values | starts_with_num(std::array{ 1, 2, 3 }));
        REQUIRE_FALSE(values | starts_with_num(std::array{ 1, 2, 4 }));
    }
}

SCENARIO("ends_with is back binding")
{
    SECTION("ends_with called with two ranges calls ranges::ends_with "
            "immediately")
    {
        static constexpr std::array ints = { 1, 2, 3, 4, 5 };
        STATIC_REQUIRE(composer::ends_with(ints, std::array{ 3, 4, 5 }));
        STATIC_REQUIRE_FALSE(composer::ends_with(ints, std::array{ 3, 4, 6 }));
        REQUIRE(composer::ends_with(ints, std::array{ 3, 4, 5 }));
        REQUIRE_FALSE(composer::ends_with(ints, std::array{ 3, 4, 6 }));
    }
    SECTION("ends_with called with two ranges and a composed predicate calls "
            "ranges::ends_with immediately")
    {
        STATIC_REQUIRE(composer::ends_with(
            values, std::array{ 3, 4, 5 }, [](auto& a, auto b) {
                return a.num == b;
            }));
        STATIC_REQUIRE_FALSE(composer::ends_with(
            values, std::array{ 3, 4, 6 }, [](auto& a, auto b) {
                return a.num == b;
            }));
        REQUIRE(composer::ends_with(
            values, std::array{ 3, 4, 5 }, [](auto& a, auto b) {
                return a.num == b;
            }));
        REQUIRE_FALSE(composer::ends_with(
            values, std::array{ 3, 4, 6 }, [](auto& a, auto b) {
                return a.num == b;
            }));
    }
    SECTION("ends_with called with two ranges, a predicate and a projection "
            "calls ranges::ends_with immediately")
    {
        STATIC_REQUIRE(composer::ends_with(
            values, std::array{ 3, 4, 5 }, composer::equal_to, &numname::num));
        STATIC_REQUIRE_FALSE(composer::ends_with(
            values, std::array{ 3, 4, 6 }, composer::equal_to, &numname::num));
        REQUIRE(composer::ends_with(
            values, std::array{ 3, 4, 5 }, composer::equal_to, &numname::num));
        REQUIRE_FALSE(composer::ends_with(
            values, std::array{ 3, 4, 6 }, composer::equal_to, &numname::num));
    }
    SECTION("ends_with called with a predicate and a projection returns a "
            "callable for two ranges")
    {
        constexpr auto ends_with_num
            = composer::ends_with(composer::equal_to, &numname::num);
        STATIC_REQUIRE(ends_with_num(values, std::array{ 3, 4, 5 }));
        STATIC_REQUIRE_FALSE(ends_with_num(values, std::array{ 3, 4, 6 }));
        REQUIRE(ends_with_num(values, std::array{ 3, 4, 5 }));
        REQUIRE_FALSE(ends_with_num(values, std::array{ 3, 4, 6 }));
    }
    SECTION("ends_with called with a precidace and a projection, called with "
            "a needle range is pipeaaple from a range")
    {
        constexpr auto ends_with_num
            = composer::ends_with(composer::equal_to, &numname::num);
        STATIC_REQUIRE(values | ends_with_num(std::array{ 3, 4, 5 }));
        STATIC_REQUIRE_FALSE(values | ends_with_num(std::array{ 3, 4, 6 }));
        REQUIRE(values | ends_with_num(std::array{ 3, 4, 5 }));
        REQUIRE_FALSE(values | ends_with_num(std::array{ 3, 4, 6 }));
    }
}
#endif // __cpp_lib_starts_ends_with

SCENARIO("is_partitioned is back binding")
{
    SECTION("is_partitioned called with a range, a predicate and a projection "
            "calls ranges::is_partitioned immediately")
    {
        STATIC_REQUIRE(composer::is_partitioned(
            values, composer::less_than(3), &numname::num));
        STATIC_REQUIRE_FALSE(composer::is_partitioned(
            values, composer::less_than("three"), &numname::name));
        REQUIRE(composer::is_partitioned(
            values, composer::less_than(3), &numname::num));
        REQUIRE_FALSE(composer::is_partitioned(
            values, composer::less_than("three"), &numname::name));
    }
    SECTION("is_partitioned called with a range and a composed predicate calls "
            "ranges::is_partitioned immediately")
    {
        STATIC_REQUIRE(composer::is_partitioned(
            values, &numname::num | composer::less_than(3)));
        STATIC_REQUIRE_FALSE(composer::is_partitioned(
            values, &numname::name | composer::less_than("three")));
        REQUIRE(composer::is_partitioned(
            values, &numname::num | composer::less_than(3)));
        REQUIRE_FALSE(composer::is_partitioned(
            values, &numname::name | composer::less_than("three")));
    }
    SECTION("is_partitioned called with a predicate and a projection can be "
            "piped from a range")
    {
        STATIC_REQUIRE(
            values
            | composer::is_partitioned(composer::less_than(3), &numname::num));
        STATIC_REQUIRE_FALSE(values
                             | composer::is_partitioned(
                                 composer::less_than("three"), &numname::name));
        REQUIRE(
            values
            | composer::is_partitioned(composer::less_than(3), &numname::num));
        REQUIRE_FALSE(values
                      | composer::is_partitioned(composer::less_than("three"),
                                                 &numname::name));
    }
    SECTION("is_partitioned called with a projection, can be called with a "
            "predicate and piped from a range")
    {
        constexpr auto is_partitioned_by_num
            = composer::is_partitioned(&numname::num);
        constexpr auto is_partitioned_by_name
            = composer::is_partitioned(&numname::name);
        STATIC_REQUIRE(values | is_partitioned_by_num(composer::less_than(3)));
        STATIC_REQUIRE_FALSE(
            values | is_partitioned_by_name(composer::less_than("three")));
        REQUIRE(values | is_partitioned_by_num(composer::less_than(3)));
        REQUIRE_FALSE(values
                      | is_partitioned_by_name(composer::less_than("three")));
    }
}

SCENARIO("partition_point is back binding")
{
    SECTION("calling partition_point with a range, a predicate and a "
            "projection calls ranges::partition_point immediately")
    {
        STATIC_REQUIRE(
            composer::partition_point(
                values, composer::size | composer::less_than(4), &numname::name)
            == values.begin() + 2);
        REQUIRE(
            composer::partition_point(
                values, composer::size | composer::less_than(4), &numname::name)
            == values.begin() + 2);
    }
    SECTION("partition_point called with a composed predicate can be piped "
            "from a range")
    {
        STATIC_REQUIRE(
            (values
             | composer::partition_point(&numname::name | composer::size
                                         | composer::less_than(4)))
            == values.begin() + 2);
        REQUIRE((values
                 | composer::partition_point(&numname::name | composer::size
                                             | composer::less_than(4)))
                == values.begin() + 2);
    }
    SECTION("partition_point called with a projection is callable with a range "
            "and a predicate")
    {
        constexpr auto pp_by_name = composer::partition_point(&numname::name);
        STATIC_REQUIRE(
            (values | pp_by_name(composer::size | composer::less_than(4)))
            == values.begin() + 2);
        REQUIRE((values | pp_by_name(composer::size | composer::less_than(4)))
                == values.begin() + 2);
    }
}

SCENARIO("is_sorted is back binding")
{
    constexpr auto by_num = composer::transform_args(&numname::num);
    constexpr auto by_name = composer::transform_args(&numname::name);
    SECTION("is_sorted called with a range, a predicate and a projection calls "
            "ranges::is_sorted immediately")
    {
        STATIC_REQUIRE(
            composer::is_sorted(values, composer::less_than, &numname::num));
        STATIC_REQUIRE_FALSE(
            composer::is_sorted(values, composer::less_than, &numname::name));
        REQUIRE(
            composer::is_sorted(values, composer::less_than, &numname::num));
        REQUIRE_FALSE(
            composer::is_sorted(values, composer::less_than, &numname::name));
    }
    SECTION("is_sorted called with a range and a composed predicate calls "
            "ranges::is_sorted immediately")
    {
        STATIC_REQUIRE(
            composer::is_sorted(values, by_num(composer::less_than)));
        STATIC_REQUIRE_FALSE(
            composer::is_sorted(values, by_name(composer::less_than)));
        REQUIRE(composer::is_sorted(values, by_num(composer::less_than)));
        REQUIRE_FALSE(
            composer::is_sorted(values, by_name(composer::less_than)));
    }
    SECTION(
        "is_sorted called with a composed predicate is pipeable from a range")
    {
        STATIC_REQUIRE(values
                       | composer::is_sorted(by_num(composer::less_than)));
        STATIC_REQUIRE_FALSE(
            values | composer::is_sorted(by_name(composer::less_than)));
        REQUIRE(values | composer::is_sorted(by_num(composer::less_than)));
        REQUIRE_FALSE(values
                      | composer::is_sorted(by_name(composer::less_than)));
    }
}

SCENARIO("is_sorted_until is back binding")
{
    constexpr auto by_num = composer::transform_args(&numname::num);
    constexpr auto by_name = composer::transform_args(&numname::name);
    SECTION("is_sorted_until called with a range, a predicate and a projection "
            "calls ranges::is_sorted_until directly")
    {
        STATIC_REQUIRE(composer::is_sorted_until(
                           values, composer::less_than, &numname::num)
                       == values.end());
        STATIC_REQUIRE(composer::is_sorted_until(
                           values, composer::less_than, &numname::name)
                       == values.begin() + 2);
        REQUIRE(composer::is_sorted_until(
                    values, composer::less_than, &numname::num)
                == values.end());
        REQUIRE(composer::is_sorted_until(
                    values, composer::less_than, &numname::name)
                == values.begin() + 2);
    }
    SECTION("is_sorted_until called with a predicate and a projection, is "
            "callable pipeable from a range")
    {
        STATIC_REQUIRE(
            (values
             | composer::is_sorted_until(composer::less_than, &numname::num))
            == values.end());
        STATIC_REQUIRE(
            (values
             | composer::is_sorted_until(composer::less_than, &numname::name))
            == values.begin() + 2);
        REQUIRE(
            (values
             | composer::is_sorted_until(composer::less_than, &numname::num))
            == values.end());
        REQUIRE(
            (values
             | composer::is_sorted_until(composer::less_than, &numname::name))
            == values.begin() + 2);
    }
    SECTION("is_sorted_until called with a composed predicate is pipeable from "
            "a range")
    {
        STATIC_REQUIRE(
            (values | composer::is_sorted_until(by_num(composer::less_than)))
            == values.end());
        STATIC_REQUIRE(
            (values | composer::is_sorted_until(by_name(composer::less_than)))
            == values.begin() + 2);
        REQUIRE(
            (values | composer::is_sorted_until(by_num(composer::less_than)))
            == values.end());
        REQUIRE(
            (values | composer::is_sorted_until(by_name(composer::less_than)))
            == values.begin() + 2);
    }
}

SCENARIO("lower_bound")
{
    static constexpr std::array<numname, 6> even_numbers
        = { { { 0, "zero" },
              { 2, "two" },
              { 4, "four" },
              { 6, "six" },
              { 8, "eight" },
              { 10, "ten" } } };

    SECTION("lower_bound called with a range, a value, a predicate and a "
            "projection calls ranges::lower_bound immediately")
    {
        STATIC_REQUIRE(composer::lower_bound(
                           even_numbers, 5, composer::less_than, &numname::num)
                       == even_numbers.begin() + 3);
        REQUIRE(composer::lower_bound(
                    even_numbers, 5, composer::less_than, &numname::num)
                == even_numbers.begin() + 3);
    }
    SECTION("lower_bound called with a predicate and a projection, called with "
            "a value, is pipeable from a range")
    {
        constexpr auto lower_bound_num
            = composer::lower_bound(composer::less_than, &numname::num);
        STATIC_REQUIRE((even_numbers | lower_bound_num(8))
                       == even_numbers.begin() + 4);
        REQUIRE((even_numbers | lower_bound_num(8))
                == even_numbers.begin() + 4);
    }
}

SCENARIO("upper_bound")
{
    static constexpr std::array<numname, 6> even_numbers
        = { { { 0, "zero" },
              { 2, "two" },
              { 4, "four" },
              { 6, "six" },
              { 8, "eight" },
              { 10, "ten" } } };

    SECTION("upper_bound called with a range, a value, a predicate and a "
            "projection calls ranges::lower_bound immediately")
    {
        STATIC_REQUIRE(composer::upper_bound(
                           even_numbers, 5, composer::less_than, &numname::num)
                       == even_numbers.begin() + 3);
        REQUIRE(composer::upper_bound(
                    even_numbers, 5, composer::less_than, &numname::num)
                == even_numbers.begin() + 3);
    }
    SECTION("upper_bound called with a predicate and a projection, called with "
            "a value, is pipeable from a range")
    {
        constexpr auto upper_bound_num
            = composer::upper_bound(composer::less_than, &numname::num);
        STATIC_REQUIRE((even_numbers | upper_bound_num(8))
                       == even_numbers.begin() + 5);
        REQUIRE((even_numbers | upper_bound_num(8))
                == even_numbers.begin() + 5);
    }
}

SCENARIO("binary_search")
{
    static constexpr std::array<numname, 6> even_numbers
        = { { { 0, "zero" },
              { 2, "two" },
              { 4, "four" },
              { 6, "six" },
              { 8, "eight" },
              { 10, "ten" } } };

    SECTION("binary called with a range, a value, a predicate and a "
            "projection calls ranges::lower_bound immediately")
    {
        STATIC_REQUIRE_FALSE(composer::binary_search(
            even_numbers, 5, composer::less_than, &numname::num));
        REQUIRE_FALSE(composer::binary_search(
            even_numbers, 5, composer::less_than, &numname::num));
        STATIC_REQUIRE(composer::binary_search(
            even_numbers, 6, composer::less_than, &numname::num));
        REQUIRE(composer::binary_search(
            even_numbers, 6, composer::less_than, &numname::num));
    }
    SECTION(
        "binary_search called with a predicate and a projection, called with "
        "a value, is pipeable from a range")
    {
        constexpr auto binary_search_num
            = composer::binary_search(composer::less_than, &numname::num);
        STATIC_REQUIRE(even_numbers | binary_search_num(8));
        REQUIRE(even_numbers | binary_search_num(8));
        STATIC_REQUIRE_FALSE(even_numbers | binary_search_num(7));
        REQUIRE_FALSE(even_numbers | binary_search_num(7));
    }
}

SCENARIO("equal_range")
{
    SECTION("equal_range called with a range, a value, a predicate and a "
            "projection calls ranges::equal_range immediately")
    {
        constexpr auto sr = composer::equal_range(
            values, 3, composer::less_than, &numname::num);
        STATIC_REQUIRE(sr.begin() == values.begin() + 2);
        STATIC_REQUIRE(sr.end() == values.begin() + 3);
        auto r = composer::equal_range(
            values, 3, composer::less_than, &numname::num);
        REQUIRE(r.begin() == values.begin() + 2);
        REQUIRE(r.end() == values.begin() + 3);
    }
    SECTION("equal_range called with a predicate and a projection, called with "
            "a value is pipeable from a range")
    {
        static constexpr auto equal_by_num
            = composer::equal_range(composer::less_than, &numname::num);
        STATIC_REQUIRE((values | equal_by_num(3)).begin()
                       == values.begin() + 2);
        STATIC_REQUIRE((values | equal_by_num(3)).end() == values.begin() + 3);
        REQUIRE((values | equal_by_num(3)).begin() == values.begin() + 2);
        REQUIRE((values | equal_by_num(3)).end() == values.begin() + 3);
    }
}

SCENARIO("includes")
{
    SECTION("includes called with a two ranges, a predicate and a projection "
            "calls ranges::includes immediately")
    {
        STATIC_REQUIRE(composer::includes(
            values, std::array{ 2, 3, 5 }, composer::less_than, &numname::num));
        STATIC_REQUIRE_FALSE(composer::includes(
            values, std::array{ 2, 3, 7 }, composer::less_than, &numname::num));
        REQUIRE(composer::includes(
            values, std::array{ 2, 3, 5 }, composer::less_than, &numname::num));
        REQUIRE_FALSE(composer::includes(
            values, std::array{ 2, 3, 7 }, composer::less_than, &numname::num));
    }
    SECTION("includes called with a predicate and a projection, called with a "
            "range, is pipeable from a range")
    {
        static constexpr auto includes_nums
            = composer::includes(composer::less_than, &numname::num);
        STATIC_REQUIRE(values | includes_nums(std::array{ 2, 3, 5 }));
        STATIC_REQUIRE_FALSE(values | includes_nums(std::array{ 2, 3, 7 }));
        REQUIRE(values | includes_nums(std::array{ 2, 3, 5 }));
        REQUIRE_FALSE(values | includes_nums(std::array{ 2, 3, 7 }));
    }
}

SCENARIO("is_heap")
{
    static constexpr std::array<numname, 6> nonheap{ { { 1, "one" },
                                                       { 2, "two" },
                                                       { 6, "six" },
                                                       { 3, "three" },
                                                       { 5, "five" },
                                                       { 4, "four" } } };
    SECTION("is_heap called with a range, a predicate and a projection calls "
            "ranges::is_heap immediately")
    {
        STATIC_REQUIRE(
            composer::is_heap(values, composer::greater_than, &numname::num));
        STATIC_REQUIRE_FALSE(
            composer::is_heap(nonheap, composer::greater_than, &numname::num));
        REQUIRE(
            composer::is_heap(values, composer::greater_than, &numname::num));
        REQUIRE_FALSE(
            composer::is_heap(nonheap, composer::greater_than, &numname::num));
    }
    SECTION("is_heap called with a range and a composed predicate calls "
            "ranges::is_heap immediately")
    {
        STATIC_REQUIRE(composer::is_heap(
            values,
            composer::transform_args(&numname::num, composer::greater_than)));
        STATIC_REQUIRE_FALSE(composer::is_heap(
            nonheap,
            composer::transform_args(&numname::num, composer::greater_than)));
        REQUIRE(composer::is_heap(
            values,
            composer::transform_args(&numname::num, composer::greater_than)));
        REQUIRE_FALSE(composer::is_heap(
            nonheap,
            composer::transform_args(&numname::num, composer::greater_than)));
    }
    SECTION("is_heap called with a composed predicate is pipeable from a range")
    {
        constexpr auto is_heap_num = composer::is_heap(
            composer::transform_args(&numname::num, composer::greater_than));
        STATIC_REQUIRE(values | is_heap_num);
        STATIC_REQUIRE_FALSE(nonheap | is_heap_num);
        REQUIRE(values | is_heap_num);
        REQUIRE_FALSE(nonheap | is_heap_num);
    }
}

SCENARIO("is_heap_until")
{
    static constexpr std::array<numname, 6> nonheap{ { { 1, "one" },
                                                       { 2, "two" },
                                                       { 6, "six" },
                                                       { 3, "three" },
                                                       { 5, "five" },
                                                       { 4, "four" } } };
    SECTION(
        "is_heap_until called with a range, a predicate and a projection calls "
        "ranges::is_heap_until immediately")
    {
        STATIC_REQUIRE(composer::is_heap_until(
                           values, composer::greater_than, &numname::num)
                       == values.end());
        STATIC_REQUIRE(composer::is_heap_until(
                           nonheap, composer::greater_than, &numname::num)
                       == std::prev(nonheap.end()));
        REQUIRE(composer::is_heap_until(
                    values, composer::greater_than, &numname::num)
                == values.end());
        REQUIRE(composer::is_heap_until(
                    nonheap, composer::greater_than, &numname::num)
                == std::prev(nonheap.end()));
    }
    SECTION("is_heap_until called with a range and a composed predicate calls "
            "ranges::is_heap_until immediately")
    {
        STATIC_REQUIRE(
            composer::is_heap_until(
                values,
                composer::transform_args(&numname::num, composer::greater_than))
            == values.end());
        STATIC_REQUIRE(
            composer::is_heap_until(
                nonheap,
                composer::transform_args(&numname::num, composer::greater_than))
            == std::prev(nonheap.end()));
        REQUIRE(
            composer::is_heap_until(
                values,
                composer::transform_args(&numname::num, composer::greater_than))
            == values.end());
        REQUIRE(
            composer::is_heap_until(
                nonheap,
                composer::transform_args(&numname::num, composer::greater_than))
            == std::prev(nonheap.end()));
    }
    SECTION("is_heap_until called with a composed predicate is pipeable from a "
            "range")
    {
        constexpr auto is_heap_num = composer::is_heap_until(
            composer::transform_args(&numname::num, composer::greater_than));
        STATIC_REQUIRE((values | is_heap_num) == values.end());
        STATIC_REQUIRE((nonheap | is_heap_num) == std::prev(nonheap.end()));
        REQUIRE((values | is_heap_num) == values.end());
        REQUIRE((nonheap | is_heap_num) == std::prev(nonheap.end()));
    }
}

SCENARIO("max")
{
    static constexpr numname a{ 1, "one" };
    static constexpr numname b{ 2, "two" };
    SECTION("calling max with two values, a predicate and a projection calls "
            "ranges::max immediately")
    {
        STATIC_REQUIRE(
            composer::max(a, b, composer::less_than, &numname::num).num == 2);
        REQUIRE(composer::max(a, b, composer::less_than, &numname::num).num
                == 2);
    }
    SECTION("calling max with two values and a composed predicate calls "
            "ranges::max immediately")
    {
        STATIC_REQUIRE(composer::max(a,
                                     b,
                                     composer::transform_args(
                                         &numname::num, composer::less_than))
                           .num
                       == 2);
        REQUIRE(composer::max(a,
                              b,
                              composer::transform_args(&numname::num,
                                                       composer::less_than))
                    .num
                == 2);
    }
    SECTION("calling max with a range, a predicate and a projection calls "
            "ranges::max immediately")
    {
        STATIC_REQUIRE(
            composer::max(values, composer::less_than, &numname::num).num == 5);
        REQUIRE(composer::max(values, composer::less_than, &numname::num).num
                == 5);
    }
    SECTION("calling max with a range and a composed predicate calls "
            "ranges::max immediately")
    {
        STATIC_REQUIRE(composer::max(values,
                                     composer::transform_args(
                                         &numname::num, composer::less_than))
                           .num
                       == 5);
        REQUIRE(composer::max(values,
                              composer::transform_args(&numname::num,
                                                       composer::less_than))
                    .num
                == 5);
    }
    SECTION("max called with a composed predicate is callable with two values")
    {
        constexpr auto maxnum = composer::max(
            composer::transform_args(&numname::num, composer::less_than));
        STATIC_REQUIRE(maxnum(a, b).num == 2);
        REQUIRE(maxnum(a, b).num == 2);
    }
    SECTION("max called with a composed predicate is pipeable from a range")
    {
        constexpr auto maxnum = composer::max(
            composer::transform_args(&numname::num, composer::less_than));
        STATIC_REQUIRE((values | maxnum).num == 5);
        REQUIRE((values | maxnum).num == 5);
    }
}

SCENARIO("max_element")
{
    SECTION("calling max_element with a range, a predicate and a projection "
            "calls ranges::max_element immediately")
    {
        STATIC_REQUIRE(
            composer::max_element(values, composer::less_than, &numname::num)
            == std::prev(values.end()));
        REQUIRE(
            composer::max_element(values, composer::less_than, &numname::num)
            == std::prev(values.end()));
    }
    SECTION(
        "max_element called with a composed predicate is pipeable from a range")
    {
        constexpr auto maxnum = composer::max_element(
            composer::transform_args(&numname::num, composer::less_than));
        STATIC_REQUIRE((values | maxnum) == std::prev(values.end()));
        REQUIRE((values | maxnum) == std::prev(values.end()));
    }
}

SCENARIO("min")
{
    static constexpr numname a{ 1, "one" };
    static constexpr numname b{ 2, "two" };
    SECTION("calling min with two values, a predicate and a projection calls "
            "ranges::min immediately")
    {
        STATIC_REQUIRE(
            composer::min(a, b, composer::less_than, &numname::num).num == 1);
        REQUIRE(composer::min(a, b, composer::less_than, &numname::num).num
                == 1);
    }
    SECTION("calling min with two values and a composed predicate calls "
            "ranges::min immediately")
    {
        STATIC_REQUIRE(composer::min(a,
                                     b,
                                     composer::transform_args(
                                         &numname::num, composer::less_than))
                           .num
                       == 1);
        REQUIRE(composer::min(a,
                              b,
                              composer::transform_args(&numname::num,
                                                       composer::less_than))
                    .num
                == 1);
    }
    SECTION("calling min with a range, a predicate and a projection calls "
            "ranges::min immediately")
    {
        STATIC_REQUIRE(
            composer::min(values, composer::less_than, &numname::num).num == 1);
        REQUIRE(composer::min(values, composer::less_than, &numname::num).num
                == 1);
    }
    SECTION("calling min with a range and a composed predicate calls "
            "ranges::min immediately")
    {
        STATIC_REQUIRE(composer::min(values,
                                     composer::transform_args(
                                         &numname::num, composer::less_than))
                           .num
                       == 1);
        REQUIRE(composer::min(values,
                              composer::transform_args(&numname::num,
                                                       composer::less_than))
                    .num
                == 1);
    }
    SECTION("min called with a composed predicate is callable with two values")
    {
        constexpr auto minnum = composer::min(
            composer::transform_args(&numname::num, composer::less_than));
        STATIC_REQUIRE(minnum(a, b).num == 1);
        REQUIRE(minnum(a, b).num == 1);
    }
    SECTION("min called with a composed predicate is pipeable from a range")
    {
        constexpr auto minnum = composer::min(
            composer::transform_args(&numname::num, composer::less_than));
        STATIC_REQUIRE((values | minnum).num == 1);
        REQUIRE((values | minnum).num == 1);
    }
}

SCENARIO("min_element")
{
    SECTION("calling min_element with a range, a predicate and a projection "
            "calls ranges::min_element immediately")
    {
        STATIC_REQUIRE(
            composer::min_element(values, composer::less_than, &numname::num)
            == values.begin());
        REQUIRE(
            composer::min_element(values, composer::less_than, &numname::num)
            == values.begin());
    }
    SECTION(
        "min_element called with a composed predicate is pipeable from a range")
    {
        constexpr auto minnum = composer::min_element(
            composer::transform_args(&numname::num, composer::less_than));
        STATIC_REQUIRE((values | minnum) == values.begin());
        REQUIRE((values | minnum) == values.begin());
    }
}

SCENARIO("minmax_element")
{
    SECTION("calling minmax_element with a range, a predicate and a projection "
            "calls ranges::minmax_element immediately")
    {
        const auto [min, max] = composer::minmax_element(
            values, composer::less_than, &numname::num);
        REQUIRE(min->num == 1);
        REQUIRE(max->num == 5);
    }
    SECTION("minmax_element called with a composed predicate is pipeable from "
            "a rannge")
    {
        const auto [min, max]
            = values
            | composer::minmax_element(
                  composer::transform_args(&numname::num, composer::less_than));
        REQUIRE(min->num == 1);
        REQUIRE(max->num == 5);
    }
}

SCENARIO("clamp")
{
    SECTION("calling clamp with three values, a predicate and a projection "
            "calls ranges::clamp immediately")
    {
        STATIC_REQUIRE(composer::clamp(values[0],
                                       values[2],
                                       values[4],
                                       composer::less_than,
                                       &numname::num)
                           .num
                       == 3);
        REQUIRE(composer::clamp(values[0],
                                values[2],
                                values[4],
                                composer::less_than,
                                &numname::num)
                    .num
                == 3);
    }
    SECTION("calling clamp with three values and a composed predicate calls "
            "ranges::clamp immediately")
    {
        STATIC_REQUIRE(composer::clamp(values[0],
                                       values[2],
                                       values[4],
                                       composer::transform_args(
                                           &numname::num, composer::less_than))
                           .num
                       == 3);
        REQUIRE(composer::clamp(values[0],
                                values[2],
                                values[4],
                                composer::transform_args(&numname::num,
                                                         composer::less_than))
                    .num
                == 3);
    }
    SECTION("clamp called with a composed predicate, called by two values is "
            "callable with one value")
    {
        constexpr auto clamp_num = composer::clamp(
            composer::transform_args(&numname::num, composer::less_than));
        constexpr auto clamp24 = clamp_num(values[2], values[4]);
        STATIC_REQUIRE(clamp24(values[0]).num == 3);
        REQUIRE((values[0] | clamp24).num == 3);
    }
}
