#include <composer/right_curry.hpp>

#include <catch2/catch_test_macros.hpp>

#include <memory>

TEST_CASE("a right curried function is called with all provided arguments")
{
    constexpr auto minus = composer::right_curry<std::minus<>, 2>{};
    STATIC_REQUIRE(minus(5, 2) == 3);
    REQUIRE(minus(5, 2) == 3);
}

TEST_CASE("a right curried function is not callable with too many arguments")
{
    constexpr auto minus = composer::right_curry<std::minus<>, 2>{};
    STATIC_REQUIRE_FALSE(std::is_invocable_v<decltype(minus), int, int, int>);
    REQUIRE_FALSE(std::is_invocable_v<decltype(minus), int, int, int>);
}

TEST_CASE("a right curried function called with fewer arguments than required, "
          "returns a callable that binds them at the end")
{
    constexpr auto minus = composer::right_curry<std::minus<>, 2>{};
    constexpr auto minus2 = minus(2);
    STATIC_REQUIRE(minus2(5) == 3);
    REQUIRE(minus2(5) == 3);
}

TEST_CASE("a right curried function can be called with fewer than arity "
          "arguments if the underlying function allows it")
{
    constexpr auto dec
        = composer::make_arity_function<2, composer::right_curry>(
            [](int a, int b = 1) { return a - b; });
    STATIC_REQUIRE(dec(3) == 2);
    REQUIRE(dec(3) == 2);
}

namespace {
template <typename, typename>
struct same_cv : std::false_type {};

template <typename T, typename U>
struct same_cv<T&, U&> : std::true_type {};

template <typename T, typename U>
struct same_cv<T&&, U&&> : std::true_type {};

template <typename T, typename U>
struct same_cv<const T&, const U&> : std::true_type {};

template <typename T, typename U>
struct same_cv<const T&&, const U&&> : std::true_type {};

template <typename Expected>
struct check {
    template <typename Self, typename T>
    void operator()(this Self&&, int, T&&)
    {
        static_assert(same_cv<Self&&, Expected>{});
        static_assert(same_cv<T&&, Expected>{});
    }
};
} // namespace

TEST_CASE("a right curried function and its bound args are called with the "
          "same qualifiers as the function object")
{
    SECTION("coll on const l-value reference is called as and with const "
            "l-value reference")
    {
        auto f = composer::make_arity_function<2, composer::right_curry>(
            check<const int&>{});
        auto fb = f(3);
        std::as_const(fb)(0);
    }
    SECTION("coll on non-const l-value reference is called as and with "
            "non-l-value reference")
    {
        auto f = composer::make_arity_function<2, composer::right_curry>(
            check<int&>{});
        auto fb = f(3);
        fb(0);
    }
    SECTION("coll on const r-value reference is called as and with const "
            "r-value reference")
    {
        auto f = composer::make_arity_function<2, composer::right_curry>(
            check<const int&&>{});
        auto fb = f(3);
        std::move(std::as_const(fb))(0);
    }
    SECTION("coll on non-const r-value reference is called as and with "
            "non-const r-value reference")
    {
        auto f = composer::make_arity_function<2, composer::right_curry>(
            check<int&&>{});
        auto fb = f(3);
        std::move(fb)(0);
    }
}

TEST_CASE("a piped expression is a right curried function that calls the right "
          "hand function with the result of the left hand function")
{
    auto to_string = composer::make_arity_function<1>(
        [](auto t) { return std::to_string(t); });
    constexpr auto minus = composer::right_curry<std::minus<>, 2>{};
    auto sub_to_str = minus | to_string;
    auto minus2_to_str = sub_to_str(2);
    REQUIRE(minus2_to_str(5) == "3");
}

TEST_CASE("a captured value is copied into the righT_curry function object",
          "[right_curry]")
{
    auto p = std::make_shared<int>(3);
    auto func = composer::make_arity_function<2, composer::right_curry>(
        [](auto x, auto y) { return *x + *y; });
    {
        auto captured_one = func(p);
        REQUIRE(p.use_count() == 2);
        REQUIRE(captured_one(std::make_shared<int>(2)) == 5);
    }
    REQUIRE(p.use_count() == 1);
}

TEST_CASE("a captured value can be moved into the right_curry function object",
          "[right_curry]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::right_curry>(
        [](const auto& x, const auto& y) { return *x + *y; });

    auto captured_one = func(std::move(p));
    REQUIRE_FALSE(p);
    REQUIRE(captured_one(std::make_unique<int>(2)) == 5);
}

TEST_CASE("a captured value is moved to the right_curry function if called on "
          "an rvalue",
          "[right_curry]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::right_curry>(
        [](auto x, auto y) { return *x + *y; });

    auto captured_one = func(std::move(p));
    REQUIRE_FALSE(p);
    REQUIRE(std::move(captured_one)(std::make_unique<int>(2)) == 5);
}

TEST_CASE("a move only type can be reference wrapped in the capture of a "
          "right_curry function object",
          "[right_curry]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::right_curry>(
        [](auto x, std::unique_ptr<int>& y) { return *x + *y; });
    auto captured_one = func(composer::ref(p));
    REQUIRE(p);
    REQUIRE(captured_one(std::make_unique<int>(2)) == 5);
}

TEST_CASE("a reference wrapped capture is passed as lvalue reference even on "
          "an rvalue right_curry function object",
          "[right_curry]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::right_curry>(
        [](auto x, std::unique_ptr<int>& y) { return *x + *y; });
    auto captured_one = func(composer::ref(p));
    REQUIRE(p);
    REQUIRE(std::move(captured_one)(std::make_unique<int>(2)) == 5);
}

TEST_CASE("a non-const reference wrapped capture is passed as non-const lvalue "
          "reference even on a const right_curry function object",
          "[right_curry]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::right_curry>(
        [](auto x, std::unique_ptr<int>& y) { return *x + *y; });
    auto captured_one = func(composer::ref(p));
    REQUIRE(p);
    REQUIRE(std::as_const(captured_one)(std::make_unique<int>(2)) == 5);
}
