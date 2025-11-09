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

SCENARIO("for_each_n is right curried")
{
    static constexpr auto append_to_string
        = [](auto& s) { return [&s](int x) { s += std::to_string(x); }; };
    SECTION("calling for_each_n with iterator, count, function and projection "
            "calls "
            "ranges::for_each_n directly")
    {
        std::string result;
        composer::for_each_n(
            values + 1, 3, append_to_string(result), &numname::num);
        REQUIRE(result == "234");
    }
    SECTION(
        "calling for_each_n with iterator, count and composed function calls "
        "ranges::for_each_n directly")
    {
        std::string result;
        composer::for_each_n(values + 1,
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
        append_ints_from(values + 1, 3);
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
        append_3_ints_from(values + 1);
        REQUIRE(result == "234");
    }
}

SCENARIO("count is right curried")
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

SCENARIO("count_if is right curried")
{
    SECTION("calling count_if with range, predicate and projection calls "
            "ranges::count_if directly")
    {
        STATIC_REQUIRE(composer::count_if(values,
                                          composer::greater(3),
                                          composer::mem_fn(&numname::num))
                       == 2);
        STATIC_REQUIRE(composer::count_if(values,
                                          composer::equal_to(0),
                                          composer::mem_fn(&numname::num))
                       == 0);
        REQUIRE(composer::count_if(values,
                                   composer::greater(3),
                                   composer::mem_fn(&numname::num))
                == 2);
        REQUIRE(composer::count_if(values,
                                   composer::greater(5),
                                   composer::mem_fn(&numname::num))
                == 0);
    }
    SECTION(
        "calling count_if with iterator_pair, predicate and projection calls "
        "ranges::count_if directly")
    {
        STATIC_REQUIRE(composer::count_if(std::begin(values),
                                          std::end(values),
                                          composer::greater(3),
                                          composer::mem_fn(&numname::num))
                       == 2);
        STATIC_REQUIRE(composer::count_if(std::begin(values),
                                          std::end(values),
                                          composer::greater(5),
                                          composer::mem_fn(&numname::num))
                       == 0);
        REQUIRE(composer::count_if(std::begin(values),
                                   std::end(values),
                                   composer::greater(3),
                                   composer::mem_fn(&numname::num))
                == 2);
        REQUIRE(composer::count_if(std::begin(values),
                                   std::end(values),
                                   composer::greater(5),
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

SCENARIO("find is right curried")
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

SCENARIO("find_if is right curried")
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

SCENARIO("find_if_not is right curried")
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
