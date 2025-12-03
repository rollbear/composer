#include <composer/algorithm.hpp>
#include <composer/functional.hpp>
#include <composer/ranges.hpp>
#include <composer/transform_args.hpp>

#include "test_utils.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_range_equals.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>

#include <array>
#include <cmath>

namespace {
struct numname {
    int num;
    std::string_view name;

    friend std::ostream& operator<<(std::ostream& os, const numname& n)
    {
        return os << "{ " << n.num << ", " << n.name << " }";
    }
};

std::ostream& operator<<(std::ostream& os, std::span<const numname> s)
{
    os << '{';
    const char* separator = " ";
    for (const auto& n : s) {
        os << std::exchange(separator, ", ") << n;
    }
    return os << " }";
}

constexpr std::array<numname, 5> values = {
    { { 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 5, "five" } }
};

template <typename T>
constexpr auto dup(T&& t)
{
    return std::forward<T>(t);
}
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
    SECTION("all_of is allowed on an r-value range")
    {
        REQUIRE(composer::all_of(dup(values),
                                 &numname::num | composer::greater_than(0)));
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
    SECTION("any_of is allowed on an r-value range")
    {
        REQUIRE(composer::any_of(dup(values),
                                 &numname::num | composer::less_than(3)));
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
    SECTION("none_of is allowed on an r-value range")
    {
        REQUIRE(composer::any_of(dup(values),
                                 &numname::num | composer::greater_than(3)));
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
    SECTION("for_each is not allowed on an r-value range")
    {
        std::string result;
        STATIC_REQUIRE_FALSE(can_call(composer::for_each,
                                      dup(values),
                                      composer::mem_fn(&numname::num)
                                          | append_to_string(result)));
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
    SECTION("count is allowed on an r-value range")
    {
        REQUIRE(composer::count(dup(values), 3, &numname::num) == 1);
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
    SECTION("count_if is allowed on an r-value range")
    {
        REQUIRE(composer::count_if(dup(values),
                                   &numname::num | composer::equal_to(3))
                == 1);
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
    SECTION("find is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(
            can_call(composer::find, dup(values), 2, &numname::num));
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
    SECTION("find_if is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::find_if,
                                      dup(values),
                                      &numname::num | composer::equal_to(2)));
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
    SECTION("find_if_not is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::find_if_not,
                                      dup(values),
                                      &numname::num | composer::equal_to(2)));
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
    SECTION("find_last is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(
            can_call(composer::find_last, dup(values), 3, &numname::num));
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
    SECTION("find_last_if is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::find_last_if,
                                      dup(values),
                                      &numname::num | composer::equal_to(3)));
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
    SECTION("find_last_if_not is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::find_last_if_not,
                                      dup(values),
                                      &numname::num | composer::equal_to(2)));
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
    SECTION("find_end is not allowed on an r-value range")
    {
        std::vector haystack = { 1, 2, 3, 4, 1, 2, 5 };
        static constexpr std::array needle = { -1, -2 };
        STATIC_REQUIRE_FALSE(
            can_call(composer::find_end, std::move(haystack), needle));
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
    SECTION("find_first_of is not allowed on an r-value range")
    {
        std::vector haystack = { 1, 2, 3, 4, 1, 2, 5 };
        static constexpr std::array needles = { 6, 4 };
        STATIC_REQUIRE_FALSE(
            can_call(composer::find_first_of, std::move(haystack), needles));
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
    SECTION("adjacent_find is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(
            can_call(composer::adjacent_find, std::array(numbers)));
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
    SECTION("search is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::search,
                                      dup(values),
                                      std::array{ 3, 4 },
                                      composer::equal_to,
                                      &numname::num));
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
    SECTION("search_n is not allowed on an r-value range")
    {
        STATIC_REQUIRE(returns_callable(composer::search_n, dup(ints), 2));
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
    SECTION("contains can be called with an r-value range")
    {
        REQUIRE(composer::contains(std::array{ 1, 2, 3, 4, 5 }, 3));
        REQUIRE_FALSE(composer::contains(std::array{ 1, 2, 3, 4, 5 }, 8));
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
    SECTION("contains_subrange can be called with an r-value range")
    {
        constexpr auto contains_subrange_num
            = composer::contains_subrange(composer::equal_to, &numname::num);

        REQUIRE(contains_subrange_num(dup(values), std::array{ 2, 3, 4 }));
        REQUIRE_FALSE(
            contains_subrange_num(dup(values), std::array{ 2, 3, 5 }));
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
    SECTION("starts_with can be called with an r-value range")
    {
        constexpr auto starts_with_num
            = composer::starts_with(composer::equal_to, &numname::num);

        REQUIRE(starts_with_num(dup(values), std::array{ 1, 2, 3 }));
        REQUIRE_FALSE(starts_with_num(dup(values), std::array{ 1, 2, 5 }));
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
    SECTION("ends_with can be called with an r-value range")
    {
        constexpr auto ends_with_num
            = composer::ends_with(composer::equal_to, &numname::num);
        REQUIRE(ends_with_num(dup(values), std::array{ 3, 4, 5 }));
        REQUIRE_FALSE(ends_with_num(dup(values), std::array{ 3, 4, 6 }));
    }
}
#endif // __cpp_lib_starts_ends_with

SCENARIO("fill")
{
    SECTION(
        "fill called with a range and a value calls ranges::fill immediately")
    {
        std::array ints = { 1, 2, 3, 4, 5 };

        composer::fill(ints, -1);

        REQUIRE_THAT(ints,
                     Catch::Matchers::RangeEquals({ -1, -1, -1, -1, -1 }));
    }
    SECTION("fill called with a values is callable with a range")
    {
        auto make_zero = composer::fill(0);

        std::array ints = { 1, 2, 3, 4, 5 };

        make_zero(ints);

        REQUIRE_THAT(ints, Catch::Matchers::RangeEquals({ 0, 0, 0, 0, 0 }));
    }
    SECTION("fill is not callable with an r-value range")
    {
        auto make_zero = composer::fill(0);

        STATIC_REQUIRE(returns_callable(make_zero, std::array{ 1, 2, 3 }));
    }
    SECTION("fill is not pipeable")
    {
        std::array ints = { 1, 2, 3, 4, 5 };
        auto make_zero = composer::fill(0);
        STATIC_REQUIRE_FALSE(can_pipe(ints, make_zero));
    }
}

SCENARIO("fill_n")
{
    std::vector<int> result;
    auto inserter = std::back_inserter(result);
    SECTION("fill_n called with an iterator, a count and a value calls "
            "ranges::fill_n immediately")
    {
        composer::fill_n(inserter, 3, -1);

        REQUIRE_THAT(result, Catch::Matchers::RangeEquals({ -1, -1, -1 }));
    }
    SECTION(
        "fill called with a values is callable with an iterator and a count")
    {
        auto make_zero = composer::fill_n(0);

        make_zero(inserter, 5);

        REQUIRE_THAT(result, Catch::Matchers::RangeEquals({ 0, 0, 0, 0, 0 }));
    }
    SECTION("fill called with a count and a value is callable with an iterator")
    {
        auto make_zero = composer::fill_n(5, 0);

        make_zero(inserter);

        REQUIRE_THAT(result, Catch::Matchers::RangeEquals({ 0, 0, 0, 0, 0 }));
    }
}

SCENARIO("generate")
{
    static constexpr auto incrementer
        = [](auto x) { return [x]() mutable { return x++; }; };
    SECTION("generate called with a range and a generator calls "
            "ranges::generate immediately")
    {
        std::array ints = { 0, 0, 0, 0 };
        composer::generate(ints, incrementer(1));
        REQUIRE_THAT(ints, Catch::Matchers::RangeEquals({ 1, 2, 3, 4 }));
    }
    SECTION("generate called with a generator is callable with a range")
    {
        std::array ints = { 0, 0, 0, 0 };
        auto iota = composer::generate(incrementer(0));
        iota(ints);
        REQUIRE_THAT(ints, Catch::Matchers::RangeEquals({ 0, 1, 2, 3 }));
    }
    SECTION("generate is not callable with an r-value range")
    {
        auto iota = composer::generate(incrementer(1));
        STATIC_REQUIRE(returns_callable(iota, std::array{ 1, 2, 3 }));
    }
    SECTION("generate is not pipeable")
    {
        std::array ints = { 0, 0, 0, 0 };
        auto iota = composer::generate(incrementer(0));
        STATIC_REQUIRE_FALSE(can_pipe(ints, iota));
    }
}

SCENARIO("generate_n")
{
    static constexpr auto incrementer
        = [](auto x) { return [x]() mutable { return x++; }; };
    std::vector<int> result;
    auto inserter = std::back_inserter(result);

    SECTION("generate_n called with an iterator, a count and a generator calls "
            "ranges::generate_n immediately")
    {
        composer::generate_n(inserter, 4, incrementer(1));
        REQUIRE_THAT(result, Catch::Matchers::RangeEquals({ 1, 2, 3, 4 }));
    }
    SECTION("generate_n called with a count and a generator is callable with a "
            "range")
    {
        auto iota = composer::generate_n(4, incrementer(0));
        iota(inserter);
        REQUIRE_THAT(result, Catch::Matchers::RangeEquals({ 0, 1, 2, 3 }));
    }
    SECTION("generate_n called with a generator is callable with an iterator "
            "anda count")
    {
        auto iota = composer::generate_n(incrementer(0));
        iota(inserter, 4);
        REQUIRE_THAT(result, Catch::Matchers::RangeEquals({ 0, 1, 2, 3 }));
    }
}

SCENARIO("remove")
{
    auto local_values = values | std::ranges::to<std::vector>();

    SECTION("calling remove with a range, a value and a projection calls "
            "ranges::remove immediately")
    {
        composer::remove(local_values, 3, &numname::num);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 2, 4, 5, 5 }));
    }
    SECTION(
        "calling remove with a projection is callable with a range and a value")
    {
        constexpr auto remove_num = composer::remove(&numname::num);
        remove_num(local_values, 3);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 2, 4, 5, 5 }));
    }
    SECTION(
        "calling remove with a value and a projection is callable with a range")
    {
        constexpr auto remove_3 = composer::remove(3, &numname::num);
        remove_3(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 2, 4, 5, 5 }));
    }
    SECTION("remove is not callable with an r-value range")
    {
        constexpr auto remove_3 = composer::remove(3, &numname::num);
        STATIC_REQUIRE(returns_callable(remove_3, std::move(local_values)));
    }
    SECTION("remove is not pipeable")
    {
        constexpr auto remove_3 = composer::remove(3, &numname::num);
        STATIC_REQUIRE_FALSE(can_pipe(local_values, remove_3));
    }
}

SCENARIO("remove_if")
{
    auto local_values = values | std::ranges::to<std::vector>();

    SECTION(
        "calling remove_if with a range, a predicate and a projection calls "
        "ranges::remove_if immediately")
    {
        composer::remove_if(local_values, composer::equal_to(3), &numname::num);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 2, 4, 5, 5 }));
    }
    SECTION("calling remove_if with a projection is callable with a range and "
            "a predicate")
    {
        constexpr auto remove_num = composer::remove_if(&numname::num);
        remove_num(local_values, composer::equal_to(3));
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 2, 4, 5, 5 }));
    }
    SECTION("calling remove_if with a predicate and a projection is callable "
            "with a range")
    {
        constexpr auto remove_3
            = composer::remove_if(composer::equal_to(3), &numname::num);
        remove_3(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 2, 4, 5, 5 }));
    }
    SECTION(
        "calling remove_if with a composed predicate is callable with a range")
    {
        constexpr auto remove_3
            = composer::remove_if(&numname::num | composer::equal_to(3));
        remove_3(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 2, 4, 5, 5 }));
    }
    SECTION("remove is not callable with an r-value range")
    {
        constexpr auto remove_3
            = composer::remove_if(&numname::num | composer::equal_to(3));
        STATIC_REQUIRE(returns_callable(remove_3, std::move(local_values)));
    }
    SECTION("remove is not pipeable")
    {
        constexpr auto remove_3
            = composer::remove_if(&numname::num | composer::equal_to(3));
        STATIC_REQUIRE_FALSE(can_pipe(local_values, remove_3));
    }
}

SCENARIO("replace")
{
    auto local_values = values;
    SECTION("replace called with a range, two values and a projection calls "
            "ranges::replace immediately")
    {
        composer::replace(local_values, 1, numname{ 0, "zero" }, &numname::num);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 0, 2, 3, 4, 5 }));
        REQUIRE_THAT(local_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "zero", "two", "three", "four", "five" }));
    }
    SECTION("replace called with a projection, called with an old value and a "
            "replacement, is callable with a range")
    {
        auto replace_by_num = composer::replace(&numname::num);
        auto replace_1_0 = replace_by_num(1, numname{ 0, "zero" });
        replace_1_0(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 0, 2, 3, 4, 5 }));
        REQUIRE_THAT(local_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "zero", "two", "three", "four", "five" }));
    }
    SECTION("replace is not callable with an r-value range")
    {
        auto replace_by_num = composer::replace(&numname::num);
        auto replace_1_0 = replace_by_num(1, numname{ 0, "zero" });
        STATIC_REQUIRE(returns_callable(replace_1_0, std::move(local_values)));
    }
    SECTION("replace is not pipeable")
    {
        auto replace_by_num = composer::replace(&numname::num);
        auto replace_1_0 = replace_by_num(1, numname{ 0, "zero" });
        STATIC_REQUIRE_FALSE(can_pipe(std::move(local_values), replace_1_0));
    }
}

SCENARIO("replace_if")
{
    auto local_values = values;
    SECTION("replace_if called with a range, a predicate, a value and a "
            "projection calls "
            "ranges::replace immediately")
    {
        composer::replace_if(local_values,
                             composer::equal_to(1),
                             numname{ 0, "zero" },
                             &numname::num);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 0, 2, 3, 4, 5 }));
        REQUIRE_THAT(local_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "zero", "two", "three", "four", "five" }));
    }
    SECTION(
        "replace_if called with a projection, called with a predicate and a "
        "replacement, is callable with a range")
    {
        auto replace_by_num = composer::replace_if(&numname::num);
        auto replace_1_0
            = replace_by_num(composer::equal_to(1), numname{ 0, "zero" });
        replace_1_0(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 0, 2, 3, 4, 5 }));
        REQUIRE_THAT(local_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "zero", "two", "three", "four", "five" }));
    }
    SECTION("replace_if called with a composed predicate and a "
            "replacement, is callable with a range")
    {
        auto replace_1_0 = composer::replace_if(
            &numname::num | composer::equal_to(1), numname{ 0, "zero" });
        replace_1_0(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 0, 2, 3, 4, 5 }));
        REQUIRE_THAT(local_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "zero", "two", "three", "four", "five" }));
    }
    SECTION("replace_if is not callable with an r-value range")
    {
        auto replace_1_0 = composer::replace_if(
            &numname::num | composer::equal_to(1), numname{ 0, "zero" });
        STATIC_REQUIRE(returns_callable(replace_1_0, std::move(local_values)));
    }
    SECTION("replace_if is not pipeable")
    {
        auto replace_1_0 = composer::replace_if(
            &numname::num | composer::equal_to(1), numname{ 0, "zero" });
        STATIC_REQUIRE_FALSE(can_pipe(std::move(local_values), replace_1_0));
    }
}

SCENARIO("unique")
{
    auto local_values = values;
    SECTION("unique called with a range, a predicate and a projection calls "
            "ranges::unique immediately")
    {
        auto rest = composer::unique(
            local_values,
            composer::transform_args(composer::size, composer::equal_to),
            &numname::name);

        auto remaining
            = std::span(local_values).first(local_values.size() - rest.size());
        REQUIRE_THAT(remaining | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals({ "one", "three", "four" }));
        REQUIRE_THAT(remaining | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 3, 4 }));
    }
    SECTION("unique called with a composed predicate is callable with a range")
    {
        auto unique_by_name_len = composer::unique(composer::transform_args(
            &numname::name | composer::size, composer::equal_to));
        auto rest = unique_by_name_len(local_values);
        auto remaining
            = std::span(local_values).first(local_values.size() - rest.size());
        REQUIRE_THAT(remaining | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals({ "one", "three", "four" }));
        REQUIRE_THAT(remaining | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 1, 3, 4 }));
    }
    SECTION("unique is not callable with an r-value range")
    {
        auto unique_by_name_len = composer::unique(composer::transform_args(
            &numname::name | composer::size, composer::equal_to));
        STATIC_REQUIRE(
            returns_callable(unique_by_name_len, std::move(local_values)));
    }
    SECTION("unique is not pipeable")
    {
        auto unique_by_name_len = composer::unique(composer::transform_args(
            &numname::name | composer::size, composer::equal_to));
        STATIC_REQUIRE_FALSE(can_pipe(local_values, unique_by_name_len));
    }
}

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
    SECTION("is_partitioned can be called with an r-value range")
    {
        constexpr auto is_partitioned_by_num
            = composer::is_partitioned(&numname::num);
        constexpr auto is_partitioned_by_name
            = composer::is_partitioned(&numname::name);
        REQUIRE(is_partitioned_by_num(dup(values), composer::less_than(3)));
        REQUIRE_FALSE(
            is_partitioned_by_name(dup(values), composer::less_than("three")));
    }
}

SCENARIO("partition is back binding")
{
    SECTION("partition called with a range, a predicate and a projection calls "
            "ranges::partition immediately")
    {
        auto local_values = values;
        composer::partition(local_values,
                            composer::size | composer::less_than(4),
                            &numname::name);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num)
                         | std::views::take(2) | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 1, 2 }));
        REQUIRE_THAT(local_values | std::views::transform(&numname::num)
                         | std::views::drop(2) | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 3, 4, 5 }));
    }
    SECTION(
        "partition called with a composed predicate, is callable with a range")
    {
        auto local_values = values;
        auto short_strings_first = composer::partition(composer::transform_args(
            &numname::name | composer::size, composer::less_than(4)));
        short_strings_first(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num)
                         | std::views::take(2) | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 1, 2 }));
        REQUIRE_THAT(local_values | std::views::transform(&numname::num)
                         | std::views::drop(2) | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 3, 4, 5 }));
    }
    SECTION("partition is not callable with an r-value range")
    {
        auto short_strings_first = composer::partition(composer::transform_args(
            &numname::name | composer::size, composer::less_than(4)));
        STATIC_REQUIRE(returns_callable(short_strings_first, dup(values)));
    }
    SECTION("partition is not pipeable")
    {
        auto local_values = values;
        auto short_strings_first = composer::partition(composer::transform_args(
            &numname::name | composer::size, composer::less_than(4)));
        STATIC_REQUIRE_FALSE(can_pipe(local_values, short_strings_first));
    }
}

SCENARIO("partition_copy is back binding")
{
    SECTION("partition_copy called with a const range, two iterators, a "
            "predicate and a projection calls "
            "ranges::partition immediately")
    {
        std::vector<numname> short_names;
        std::vector<numname> long_names;
        composer::partition_copy(values,
                                 std::back_inserter(short_names),
                                 std::back_inserter(long_names),
                                 composer::size | composer::less_than(4),
                                 &numname::name);
        REQUIRE_THAT(short_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 1, 2 }));
        REQUIRE_THAT(long_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 3, 4, 5 }));
    }
    SECTION("partition_copy called with a composed predicate, is callable with "
            "a range")
    {
        auto by_string_length
            = composer::partition_copy(composer::transform_args(
                &numname::name | composer::size, composer::less_than(4)));
        std::vector<numname> short_names;
        std::vector<numname> long_names;
        by_string_length(values,
                         std::back_inserter(short_names),
                         std::back_inserter(long_names));
        REQUIRE_THAT(short_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 1, 2 }));
        REQUIRE_THAT(long_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 3, 4, 5 }));
    }
    SECTION("partition_copy called with two output iterators, a composed "
            "predicate, is callable with a range")
    {
        std::vector<numname> short_names;
        std::vector<numname> long_names;
        auto by_string_length = composer::partition_copy(
            std::back_inserter(short_names),
            std::back_inserter(long_names),
            composer::transform_args(&numname::name | composer::size,
                                     composer::less_than(4)));
        by_string_length(values);
        REQUIRE_THAT(short_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 1, 2 }));
        REQUIRE_THAT(long_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 3, 4, 5 }));
    }
    SECTION("partition_copy is not callable with an r-value range")
    {
        std::vector<numname> short_names;
        std::vector<numname> long_names;
        auto by_string_length = composer::partition_copy(
            std::back_inserter(short_names),
            std::back_inserter(long_names),
            composer::transform_args(&numname::name | composer::size,
                                     composer::less_than(4)));
        STATIC_REQUIRE(returns_callable(by_string_length, dup(values)));
    }
    SECTION("partition_copy is pipeable")
    {
        std::vector<numname> short_names;
        std::vector<numname> long_names;
        auto by_string_length = composer::partition_copy(
            std::back_inserter(short_names),
            std::back_inserter(long_names),
            composer::transform_args(&numname::name | composer::size,
                                     composer::less_than(4)));
        auto [in_it, short_it, long_it] = values | by_string_length;
        REQUIRE_THAT(short_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 1, 2 }));
        REQUIRE_THAT(long_names | std::views::transform(&numname::num)
                         | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 3, 4, 5 }));
        REQUIRE(in_it == values.end());
    }
}

SCENARIO("stable_partition is back binding")
{
    SECTION("stable_partition called with a range, a predicate and a "
            "projection calls "
            "ranges::stable_partition immediately")
    {
        auto local_values = values;
        composer::stable_partition(local_values,
                                   composer::size | composer::less_than(5),
                                   &numname::name);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals(std::array{ 1, 2, 4, 5, 3 }));
    }
    SECTION("stable_partition called with a composed predicate, is callable "
            "with a range")
    {
        auto local_values = values;
        auto short_strings_first
            = composer::stable_partition(composer::transform_args(
                &numname::name | composer::size, composer::less_than(5)));
        short_strings_first(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals(std::array{ 1, 2, 4, 5, 3 }));
    }
    SECTION("stable_partition is not callable with an r-value range")
    {
        auto short_strings_first
            = composer::stable_partition(composer::transform_args(
                &numname::name | composer::size, composer::less_than(4)));
        STATIC_REQUIRE(returns_callable(short_strings_first, dup(values)));
    }
    SECTION("stable_partition is not pipeable")
    {
        auto local_values = values;
        auto short_strings_first
            = composer::stable_partition(composer::transform_args(
                &numname::name | composer::size, composer::less_than(4)));
        STATIC_REQUIRE_FALSE(can_pipe(local_values, short_strings_first));
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
    SECTION("partition_point is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(
            can_call(composer::partition_point,
                     dup(values),
                     &numname::name | composer::size | composer::less_than(4)));
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
    SECTION("is_sorted can be called with an r-value range")
    {
        REQUIRE(composer::is_sorted(dup(values), by_num(composer::less_than)));
        REQUIRE_FALSE(
            composer::is_sorted(dup(values), by_name(composer::less_than)));
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
    SECTION("is_sorted_until is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::is_sorted_until,
                                      dup(values),
                                      by_num(composer::less_than)));
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
    SECTION("lower_bound is not allowed on an r-value range")
    {
        STATIC_REQUIRE(returns_callable(composer::lower_bound,
                                        dup(values),
                                        composer::less_than,
                                        &numname::num));
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
    SECTION("upper_bound is not allowed on an r-value range")
    {
        STATIC_REQUIRE(returns_callable(composer::upper_bound,
                                        dup(values),
                                        composer::less_than,
                                        &numname::num));
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
    SECTION("binary_search can be called on an r-value range")
    {
        constexpr auto binary_search_num
            = composer::binary_search(composer::less_than, &numname::num);
        REQUIRE(binary_search_num(dup(even_numbers), 8));
        REQUIRE_FALSE(binary_search_num(dup(even_numbers), 7));
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
    SECTION("equal_range is not allowed on an r-value range")
    {
        STATIC_REQUIRE(returns_callable(composer::equal_range,
                                        dup(values),
                                        composer::less_than,
                                        &numname::num));
    }
}

SCENARIO("merge")
{
    static constexpr std::array odd{ 1, 3, 5 };
    static constexpr std::array even{ 0, 2, 4 };

    SECTION("calling merge with two ranges, an output and a predicate calls "
            "ranges::merge immediately")
    {
        std::vector<int> result;
        composer::merge(
            odd, even, std::back_inserter(result), composer::less_than);
        REQUIRE_THAT(result,
                     Catch::Matchers::RangeEquals({ 0, 1, 2, 3, 4, 5 }));
    }
    SECTION("merge called with a predicate is callable with a pair of ranges "
            "and an output")
    {
        std::vector<int> result;
        auto merge_increasing = composer::merge(composer::less_than);
        merge_increasing(odd, even, std::back_inserter(result));
        REQUIRE_THAT(result,
                     Catch::Matchers::RangeEquals({ 0, 1, 2, 3, 4, 5 }));
    }
    SECTION("merge called with an output and a predicate is callable with a "
            "pair of ranges")
    {
        std::vector<int> result;
        auto merge_increasing
            = composer::merge(std::back_inserter(result), composer::less_than);
        merge_increasing(odd, even);
        REQUIRE_THAT(result,
                     Catch::Matchers::RangeEquals({ 0, 1, 2, 3, 4, 5 }));
    }
    SECTION("merge cannot be called with r-value ranges")
    {
        std::vector<int> result;
        auto merge_increasing
            = composer::merge(std::back_inserter(result), composer::less_than);
        STATIC_REQUIRE(returns_callable(merge_increasing, odd, dup(even)));
        STATIC_REQUIRE(returns_callable(merge_increasing, dup(odd), even));
    }
}

SCENARIO("inplace_merge")
{
    std::array<numname, 5> nums = { { { 5, "five" },
                                      { 3, "three" },
                                      { 1, "one" },
                                      { 4, "four" },
                                      { 2, "two" } } };
    SECTION("inplace_merge called with a range, an iteratora, a predicate and "
            "a projection calls ranges::inplace_merge immediately")
    {
        composer::inplace_merge(
            nums, nums.begin() + 3, composer::greater_than, &numname::num);
        REQUIRE_THAT(nums | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 3, 2, 1 }));
    }
    SECTION("inplace_merge called with a predicate and a projection is "
            "callable with a range and an iterator")
    {
        auto merge_greater_num
            = composer::inplace_merge(composer::greater_than, &numname::num);
        merge_greater_num(nums, nums.begin() + 3);
        REQUIRE_THAT(nums | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 3, 2, 1 }));
    }
    SECTION("inplace_merge is not callable with an r-value range")
    {
        auto merge_greater_num
            = composer::inplace_merge(composer::greater_than, &numname::num);
        STATIC_REQUIRE(returns_callable(
            merge_greater_num, std::move(nums), nums.begin() + 3));
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
    SECTION("includes can be called with an r-value range")
    {
        static constexpr auto includes_nums
            = composer::includes(composer::less_than, &numname::num);
        REQUIRE(includes_nums(dup(values), std::array{ 2, 3, 5 }));
        REQUIRE_FALSE(includes_nums(dup(values), std::array{ 2, 3, 7 }));
    }
}

SCENARIO("set_difference")
{
    static constexpr std::array lower = { 4, 3, 2, 1 };
    static constexpr std::array higher = { 6, 5, 4, 3 };

    SECTION("calling set_difference with two inputs, and output and a "
            "predicate calls ranges::set_difference immediately")
    {
        std::vector<int> output;
        composer::set_difference(
            lower, higher, std::back_inserter(output), composer::greater_than);
        composer::set_difference(
            higher, lower, std::back_inserter(output), composer::greater_than);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 2, 1, 6, 5 }));
    }
    SECTION("set_difference called with an output and a predicate is callable "
            "with a pair of ranges")
    {
        std::vector<int> output;
        auto diff_falling = composer::set_difference(std::back_inserter(output),
                                                     composer::greater_than);
        diff_falling(lower, higher);
        diff_falling(higher, lower);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 2, 1, 6, 5 }));
    }
    SECTION("set_difference called with an output and a predicate, called with "
            "a range, is callable with a range")
    {
        std::vector<int> output;
        auto diff_falling = composer::set_difference(std::back_inserter(output),
                                                     composer::greater_than);
        auto remove_higher = diff_falling(composer::ref(higher));
        remove_higher(lower);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 2, 1 }));
    }
}

SCENARIO("set_intersection")
{
    static constexpr std::array lower = { 4, 3, 2, 1 };
    static constexpr std::array higher = { 6, 5, 4, 3 };

    SECTION("calling set_intersection with two inputs, and output and a "
            "predicate calls ranges::set_intersection immediately")
    {
        std::vector<int> output;
        composer::set_intersection(
            lower, higher, std::back_inserter(output), composer::greater_than);
        composer::set_intersection(
            higher, lower, std::back_inserter(output), composer::greater_than);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 4, 3, 4, 3 }));
    }
    SECTION(
        "set_intersection called with an output and a predicate is callable "
        "with a pair of ranges")
    {
        std::vector<int> output;
        auto overlapping_falling = composer::set_intersection(
            std::back_inserter(output), composer::greater_than);
        overlapping_falling(lower, higher);
        overlapping_falling(higher, lower);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 4, 3, 4, 3 }));
    }
    SECTION(
        "set_intersection called with an output and a predicate, called with "
        "a range, is callable with a range")
    {
        std::vector<int> output;
        auto overlapping_falling = composer::set_intersection(
            std::back_inserter(output), composer::greater_than);
        auto keep_higher = overlapping_falling(composer::ref(higher));
        keep_higher(lower);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 4, 3 }));
    }
}

SCENARIO("set_symmetric_difference")
{
    static constexpr std::array lower = { 4, 3, 2, 1 };
    static constexpr std::array higher = { 6, 5, 4, 3 };

    SECTION(
        "calling set_symmetric_difference with two inputs, and output and a "
        "predicate calls ranges::set_symmetric_difference immediately")
    {
        std::vector<int> output;
        composer::set_symmetric_difference(
            lower, higher, std::back_inserter(output), composer::greater_than);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 6, 5, 2, 1 }));
    }
    SECTION("set_symmetric_difference called with an output and a predicate is "
            "callable "
            "with a pair of ranges")
    {
        std::vector<int> output;
        auto diff_falling = composer::set_symmetric_difference(
            std::back_inserter(output), composer::greater_than);
        diff_falling(lower, higher);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 6, 5, 2, 1 }));
    }
    SECTION("set_symmetric_difference called with an output and a predicate, "
            "called with "
            "a range, is callable with a range")
    {
        std::vector<int> output;
        auto diff_falling = composer::set_symmetric_difference(
            std::back_inserter(output), composer::greater_than);
        auto unique = diff_falling(composer::ref(higher));
        unique(lower);
        REQUIRE_THAT(output, Catch::Matchers::RangeEquals({ 6, 5, 2, 1 }));
    }
}

SCENARIO("set_union")
{
    static constexpr std::array lower = { 4, 3, 2, 1 };
    static constexpr std::array higher = { 6, 5, 4, 3 };

    SECTION("calling set_union with two inputs, and output and a "
            "predicate calls ranges::set_union immediately")
    {
        std::vector<int> output;
        composer::set_union(
            lower, higher, std::back_inserter(output), composer::greater_than);
        REQUIRE_THAT(output,
                     Catch::Matchers::RangeEquals({ 6, 5, 4, 3, 2, 1 }));
    }
    SECTION("set_union called with an output and a predicate is "
            "callable with a pair of ranges")
    {
        std::vector<int> output;
        auto union_falling = composer::set_union(std::back_inserter(output),
                                                 composer::greater_than);
        union_falling(lower, higher);
        REQUIRE_THAT(output,
                     Catch::Matchers::RangeEquals({ 6, 5, 4, 3, 2, 1 }));
    }
    SECTION("set_union called with an output and a predicate, "
            "called with a range, is callable with a range")
    {
        std::vector<int> output;
        auto union_falling = composer::set_union(std::back_inserter(output),
                                                 composer::greater_than);
        auto include_high = union_falling(composer::ref(higher));
        include_high(lower);
        REQUIRE_THAT(output,
                     Catch::Matchers::RangeEquals({ 6, 5, 4, 3, 2, 1 }));
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
    SECTION("is_heap can be called on an r-value range")
    {
        constexpr auto is_heap_num = composer::is_heap(
            composer::transform_args(&numname::num, composer::greater_than));
        REQUIRE(is_heap_num(dup(values)));
        REQUIRE_FALSE(is_heap_num(dup(nonheap)));
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
    SECTION("is_heap_until is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(
            composer::is_heap_until,
            dup(values),
            composer::transform_args(&numname::num, composer::greater_than)));
    }
}

SCENARIO("make_heap")
{
    SECTION("make_heap called with a range, a predicate and a projection calls "
            "ranges::make_heap immediately")
    {
        auto local_values = values;
        composer::make_heap(
            local_values, composer::greater_than, &numname::name);
        REQUIRE(composer::is_heap(
            local_values,
            composer::transform_args(&numname::name, composer::greater_than)));
    }
    SECTION(
        "make_heap called with a composed predicate is callable with a range")
    {
        auto make_heap_by_name = composer::make_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        auto local_values = values;
        make_heap_by_name(local_values);
        REQUIRE(composer::is_heap(
            local_values,
            composer::transform_args(&numname::name, composer::greater_than)));
    }
    SECTION("make_heap is not callable with an r-value range")
    {
        auto make_heap_by_name = composer::make_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        STATIC_REQUIRE(returns_callable(make_heap_by_name, dup(values)));
    }
    SECTION("make_heap is not pipeable")
    {
        auto make_heap_by_name = composer::make_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        auto local_values = values;
        STATIC_REQUIRE_FALSE(can_pipe(local_values, make_heap_by_name));
    }
}

SCENARIO("push_heap")
{
    SECTION("push_heap called with a range, a predicate and a projection calls "
            "ranges::push_heap immediately")
    {
        std::vector<numname> local_heap;
        for (auto& v : values) {
            local_heap.push_back(v);
            composer::push_heap(
                local_heap, composer::greater_than, &numname::name);
        }
        REQUIRE(composer::is_heap(
            local_heap,
            composer::transform_args(&numname::name, composer::greater_than)));
    }
    SECTION(
        "push_heap called with a composed predicate is callable with a range")
    {
        auto push_heap_by_name = composer::push_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        std::vector<numname> local_heap;
        for (auto& v : values) {
            local_heap.push_back(v);
            push_heap_by_name(local_heap);
        }
        REQUIRE(composer::is_heap(
            local_heap,
            composer::transform_args(&numname::name, composer::greater_than)));
    }
    SECTION("push_heap is not callable with an r-value range")
    {
        auto push_heap_by_name = composer::push_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        STATIC_REQUIRE(returns_callable(push_heap_by_name, dup(values)));
    }
    SECTION("push_heap is not pipeable")
    {
        auto push_heap_by_name = composer::push_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        auto local_values = values;
        STATIC_REQUIRE_FALSE(can_pipe(local_values, push_heap_by_name));
    }
}

SCENARIO("pop_heap")
{
    std::vector heap_of_values(values.begin(), values.end());
    composer::make_heap(heap_of_values, composer::greater_than, &numname::name);

    SECTION("calling pop_heap with a range, a predicate and a projection calls "
            "ranges::pop_heap immediately")
    {
        std::vector<numname> sorted;
        while (!heap_of_values.empty()) {
            sorted.push_back(heap_of_values.front());
            composer::pop_heap(
                heap_of_values, composer::greater_than, &numname::name);
            heap_of_values.pop_back();
        }
        REQUIRE_THAT(sorted | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "five", "four", "one", "three", "two" }));
    }
    SECTION("pop_heap called with a predicate and a projection is callable "
            "with a range")
    {
        std::vector<numname> sorted;
        auto pop_by_name
            = composer::pop_heap(composer::greater_than, &numname::name);
        while (!heap_of_values.empty()) {
            sorted.push_back(heap_of_values.front());
            pop_by_name(heap_of_values);
            heap_of_values.pop_back();
        }
        REQUIRE_THAT(sorted | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "five", "four", "one", "three", "two" }));
    }
    SECTION(
        "pop_heap called with a composed predicate is callable with a range")
    {
        std::vector<numname> sorted;
        auto pop_by_name = composer::pop_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        while (!heap_of_values.empty()) {
            sorted.push_back(heap_of_values.front());
            pop_by_name(heap_of_values);
            heap_of_values.pop_back();
        }
        REQUIRE_THAT(sorted | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "five", "four", "one", "three", "two" }));
    }
    SECTION("pop_heap cannot be called with an r-value range")
    {
        auto pop_by_name = composer::pop_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        STATIC_REQUIRE(
            returns_callable(pop_by_name, std::move(heap_of_values)));
    }
    SECTION("pop_heap is not pipeable")
    {
        auto pop_by_name = composer::pop_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        STATIC_REQUIRE_FALSE(can_pipe(heap_of_values, pop_by_name));
    }
}

SCENARIO("sort_heap")
{
    std::vector heap_of_values(values.begin(), values.end());
    composer::make_heap(heap_of_values, composer::greater_than, &numname::name);

    SECTION(
        "calling sort_heap with a range, a predicate and a projection calls "
        "ranges::sort_heap immediately")
    {
        composer::sort_heap(
            heap_of_values, composer::greater_than, &numname::name);
        REQUIRE_THAT(heap_of_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "two", "three", "one", "four", "five" }));
    }
    SECTION("sort_heap called with a predicate and a projection is callable "
            "with a range")
    {
        auto sort_by_name
            = composer::sort_heap(composer::greater_than, &numname::name);
        sort_by_name(heap_of_values);
        REQUIRE_THAT(heap_of_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "two", "three", "one", "four", "five" }));
    }
    SECTION(
        "sort_heap called with a composed predicate is callable with a range")
    {
        auto sort_by_name = composer::sort_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        sort_by_name(heap_of_values);
        REQUIRE_THAT(heap_of_values | std::views::transform(&numname::name),
                     Catch::Matchers::RangeEquals(
                         { "two", "three", "one", "four", "five" }));
    }
    SECTION("sort_heap cannot be called with an r-value range")
    {
        auto sort_by_name = composer::sort_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        STATIC_REQUIRE(
            returns_callable(sort_by_name, std::move(heap_of_values)));
    }
    SECTION("sort_heap is not pipeable")
    {
        auto sort_by_name = composer::sort_heap(
            composer::transform_args(&numname::name, composer::greater_than));
        STATIC_REQUIRE_FALSE(can_pipe(heap_of_values, sort_by_name));
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
    SECTION("max_element is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::max_element,
                                      dup(values),
                                      composer::less_than,
                                      &numname::num));
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
    SECTION("min_element is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::min_element,
                                      dup(values),
                                      composer::less_than,
                                      &numname::num));
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
            "a range")
    {
        const auto [min, max]
            = values
            | composer::minmax_element(
                  composer::transform_args(&numname::num, composer::less_than));
        REQUIRE(min->num == 1);
        REQUIRE(max->num == 5);
    }
    SECTION("minmax_element is not allowed on an r-value range")
    {
        STATIC_REQUIRE_FALSE(can_call(
            composer::minmax_element,
            dup(values),
            composer::transform_args(&numname::num, composer::less_than)));
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

SCENARIO("sort")
{
    std::array ints{ 1, 5, 2, 3, 8 };

    SECTION("calling sort with an array of ints sorts in increasing order")
    {
        composer::sort(ints);
        REQUIRE_THAT(ints, Catch::Matchers::RangeEquals({ 1, 2, 3, 5, 8 }));
    }
    SECTION("calling sort with an array and a predicate changes sort order")
    {
        composer::sort(ints, composer::greater_than);
        REQUIRE_THAT(ints, Catch::Matchers::RangeEquals({ 8, 5, 3, 2, 1 }));
    }
    SECTION("sort called with a predicate is callable with an array of ints")
    {
        constexpr auto sort_decreasing = composer::sort(composer::greater_than);
        sort_decreasing(ints);
        REQUIRE_THAT(ints, Catch::Matchers::RangeEquals({ 8, 5, 3, 2, 1 }));
    }
    SECTION("sort called with a predicate and a projection is callable with an "
            "array")
    {
        constexpr auto sort_by_name
            = composer::sort(composer::less_than, &numname::name);
        auto valcopy = values;
        sort_by_name(valcopy);
        REQUIRE_THAT(valcopy | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 1, 3, 2 }));
    }
    SECTION("sort called with a composed predicate is callable with an array")
    {
        constexpr auto sort_by_name = composer::sort(
            composer::transform_args(&numname::name, composer::less_than));
        auto valcopy = values;
        sort_by_name(valcopy);
        REQUIRE_THAT(valcopy | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 1, 3, 2 }));
    }
    SECTION("piping an array of ints to sort is not allowed")
    {
        STATIC_REQUIRE_FALSE(can_pipe(ints, composer::sort));
    }
    SECTION("calling sort with an rvalue array is not allowed")
    {
        STATIC_REQUIRE_FALSE(can_call(composer::sort, std::move(ints)));
    }
    SECTION("calling sort with a const array returns an arity_function")
    {
        STATIC_REQUIRE(returns_callable(composer::sort, std::as_const(ints)));
    }
}

SCENARIO("partial_sort")
{
    SECTION("calling partial_sort with a range, an iterator, a predicate and a "
            "projection calls ranges::stable_sort immediately")
    {
        auto valcopy = values;
        composer::partial_sort(valcopy,
                               valcopy.begin() + 3,
                               composer::greater_than,
                               &numname::num);
        REQUIRE_THAT(valcopy | std::views::transform(&numname::num)
                         | std::views::take(3),
                     Catch::Matchers::RangeEquals({ 5, 4, 3 }));
        REQUIRE_THAT(valcopy | std::views::transform(&numname::num)
                         | std::views::drop(3) | std::ranges::to<std::vector>(),
                     Catch::Matchers::UnorderedEquals(std::vector{ 1, 2 }));
    }
    SECTION("calling partial_sort with a range, an iterator and a composed "
            "predicate calls ranger::stable_sort immediately")
    {
        auto valcopy = values;
        composer::partial_sort(
            valcopy,
            valcopy.begin() + 3,
            composer::transform_args(&numname::num, composer::greater_than));
        REQUIRE_THAT(valcopy | std::views::transform(&numname::num)
                         | std::views::take(3),
                     Catch::Matchers::RangeEquals({ 5, 4, 3 }));
    }
    SECTION("calling partial_sort with a composed predicate returns a callable "
            "for a range and an iterator")
    {
        auto valcopy = values;
        constexpr auto sort_by_num = composer::partial_sort(
            composer::transform_args(&numname::num, composer::greater_than));
        sort_by_num(valcopy, valcopy.begin() + 3);
        REQUIRE_THAT(valcopy | std::views::transform(&numname::num)
                         | std::views::take(3),
                     Catch::Matchers::RangeEquals({ 5, 4, 3 }));
    }
    SECTION("partial_sort called with an iterator and a composed predicate is "
            "callable with an l-value range")
    {
        auto valcopy = values;
        auto dangerous = composer::partial_sort(
            valcopy.begin() + 3,
            composer::transform_args(&numname::num, composer::greater_than));
        dangerous(valcopy);
        REQUIRE_THAT(valcopy | std::views::transform(&numname::num)
                         | std::views::take(3),
                     Catch::Matchers::RangeEquals({ 5, 4, 3 }));
    }
    SECTION("partial_sort called with an iterator and a composed predicate is "
            "not callable with an r-value range")
    {
        auto valcopy = values;
        auto dangerous = composer::partial_sort(
            valcopy.begin() + 3,
            composer::transform_args(&numname::num, composer::greater_than));
        STATIC_REQUIRE(returns_callable(dangerous, std::move(valcopy)));
    }
    SECTION("partial_sort called with an iterator and a composed predicate is "
            "not pipeable from a range")
    {
        auto valcopy = values;
        auto dangerous = composer::partial_sort(
            valcopy.begin() + 3,
            composer::transform_args(&numname::num, composer::greater_than));
        STATIC_REQUIRE_FALSE(can_pipe(valcopy, dangerous));
    }
}

SCENARIO("partial_sort_copy")
{
    SECTION("calling partial_sort_copy with input range, output range and "
            "composed predicate calls ranges::partial_sort_copy immediately")
    {
        std::vector<numname> result(3);
        composer::partial_sort_copy(
            values,
            result,
            composer::transform_args(&numname::name, composer::less_than));
        REQUIRE_THAT(result | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 1 }));
    }
    SECTION("partial_sort_copy called with a composed predicate is callable "
            "with an input range and an output range")
    {
        constexpr auto sort_by_name = composer::partial_sort_copy(
            composer::transform_args(&numname::name, composer::less_than));
        std::vector<numname> result(3);
        sort_by_name(values, result);
        REQUIRE_THAT(result | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 1 }));
    }
    SECTION("partial_sort_copy called with an output range by ref and a "
            "composed predicate is callable with an l-value range")
    {
        std::vector<numname> result(3);
        auto sort_by_name = composer::partial_sort_copy(
            composer::ref(result),
            composer::transform_args(&numname::name, composer::less_than));
        sort_by_name(values);
        REQUIRE_THAT(result | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 1 }));
    }
    SECTION("partial_sort_copy cannot be called with an r-value output range")
    {
        STATIC_REQUIRE_FALSE(can_call(
            composer::partial_sort_copy,
            values,
            std::vector<numname>(3),
            composer::transform_args(&numname::num, composer::less_than)));
    }
    SECTION("partial_sort_copy can not be called with an r-value input range")
    {
        std::vector<numname> result(3);
        auto sort_by_name = composer::partial_sort_copy(
            composer::transform_args(&numname::name, composer::less_than));
        STATIC_REQUIRE(returns_callable(sort_by_name, dup(values), result));
    }
}

SCENARIO("stable_sort")
{
    SECTION("calling stable_sort with a range, a predicate and a projection "
            "calls ranges::stable_sort immediately")
    {
        auto local_values = values;
        composer::stable_sort(
            local_values, composer::greater_than, &numname::num);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 5, 4, 3, 2, 1 }));
    }
    SECTION("calling stable_sort with a range and a composed predicate calls "
            "ranges::stable_sort immediately")
    {
        auto local_values = values;
        auto sort_by_name_len = composer::stable_sort(composer::transform_args(
            &numname::name | composer::size, composer::greater_than));
        sort_by_name_len(local_values);
        REQUIRE_THAT(local_values | std::views::transform(&numname::num),
                     Catch::Matchers::RangeEquals({ 3, 4, 5, 1, 2 }));
    }
    SECTION("stable_sort called with a composed predicate is not pipeable from "
            "a range")
    {
        auto local_values = values;
        auto sort_by_name_len = composer::stable_sort(composer::transform_args(
            &numname::name | composer::size, composer::greater_than));
        STATIC_REQUIRE_FALSE(can_pipe(local_values, sort_by_name_len));
    }
    SECTION("stable_sort is not callable with an r-value range")
    {
        auto sort_by_name_len = composer::stable_sort(composer::transform_args(
            &numname::name | composer::size, composer::greater_than));
        STATIC_REQUIRE(returns_callable(sort_by_name_len, dup(values)));
    }
}

SCENARIO("nth_element")
{
    SECTION("calling nth_element with a range, an iterator and a composed "
            "predicate calls ranges::nth_element immediately")
    {
        auto local_values = values;
        composer::nth_element(
            local_values,
            local_values.begin() + 2,
            composer::transform_args(&numname::name, composer::less_than));
        REQUIRE(local_values[2].num == 1);
        REQUIRE_THAT(
            local_values | std::views::transform(&numname::num)
                | std::ranges::to<std::vector>(),
            Catch::Matchers::UnorderedEquals(std::vector{ 1, 2, 3, 4, 5 }));
    }
    SECTION("calling nth_element with a composed predicate is callable with a "
            "range and an iterato")
    {
        auto local_values = values;
        auto nth_by_name = composer::nth_element(
            composer::transform_args(&numname::name, composer::less_than));
        nth_by_name(local_values, local_values.begin() + 2);
        REQUIRE(local_values[2].num == 1);
    }
    SECTION("nth_element is not callable with an r-value range")
    {
        auto local_values = values;
        auto nth_by_name = composer::nth_element(
            composer::transform_args(&numname::name, composer::less_than));
        STATIC_REQUIRE(returns_callable(
            nth_by_name, std::move(local_values), local_values.begin() + 2));
    }
    SECTION("nth_element is not pipeable")
    {
        auto local_values = values;
        auto third_by_name = composer::nth_element(
            local_values.begin() + 2,
            composer::transform_args(&numname::name, composer::less_than));
        STATIC_REQUIRE_FALSE(can_pipe(values, third_by_name));
    }
}
