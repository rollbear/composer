#include <composer/front_binding.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("a front bound function is called with all provided arguments")
{
    constexpr auto minus = composer::front_binding<2, std::minus<>>{};
    STATIC_REQUIRE(minus(5, 2) == 3);
    REQUIRE(minus(5, 2) == 3);
}

TEST_CASE("a front bound function is not callable with too many arguments")
{
    constexpr auto minus = composer::front_binding<2, std::minus<>>{};
    STATIC_REQUIRE_FALSE(std::is_invocable_v<decltype(minus), int, int, int>);
}

TEST_CASE("a front bound function called with fewer arguments that required, "
          "returns a callable that binds themthe beginning")
{
    constexpr auto minus = composer::front_binding<2, std::minus<>>{};
    constexpr auto f5minus = minus(5);
    STATIC_REQUIRE(f5minus(2) == 3);
    REQUIRE(f5minus(2) == 3);
}

TEST_CASE("a front bound function can be called with fewer than arity "
          "arguments if the underlying function allows it")
{
    constexpr auto dec
        = composer::make_arity_function<2, composer::front_binding>(
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
    void operator()(this Self&&, T&&, int)
    {
        static_assert(same_cv<Self&&, Expected>{});
        static_assert(same_cv<T&&, Expected>{});
    }
};
} // namespace

TEST_CASE("a front bound function and its bound args are called with the same "
          "qualifiers as the function object")
{
    SECTION("coll on const l-value reference is called as and with const "
            "l-value reference")
    {
        auto f = composer::make_arity_function<2, composer::front_binding>(
            check<const int&>{});
        auto fb = f(3);
        std::as_const(fb)(0);
    }
    SECTION("coll on non-const l-value reference is called as and with "
            "non-l-value reference")
    {
        auto f = composer::make_arity_function<2, composer::front_binding>(
            check<int&>{});
        auto fb = f(3);
        fb(0);
    }
    SECTION("coll on const r-value reference is called as and with const "
            "r-value reference")
    {
        auto f = composer::make_arity_function<2, composer::front_binding>(
            check<const int&&>{});
        auto fb = f(3);
        std::move(std::as_const(fb))(0);
    }
    SECTION("coll on non-const r-value reference is called as and with "
            "non-const r-value reference")
    {
        auto f = composer::make_arity_function<2, composer::front_binding>(
            check<int&&>{});
        auto fb = f(3);
        std::move(fb)(0);
    }
}

TEST_CASE("a piped expression is a front bound function that calls the right "
          "hand function with the result of the left hand function")
{
    auto to_string = composer::make_arity_function<1>(
        [](auto t) { return std::to_string(t); });
    constexpr auto minus = composer::front_binding<2, std::minus<>>{};
    auto sub_to_str = minus | to_string;
    auto sub_from_2_to_str = sub_to_str(2);
    REQUIRE(sub_from_2_to_str(5) == "-3");
}

TEST_CASE("a captured value is copied into the front_binding function object",
          "[front_binding]")
{
    auto p = std::make_shared<int>(3);
    auto func = composer::make_arity_function<2, composer::front_binding>(
        [](auto x, auto y) { return *x + *y; });
    {
        auto captured_one = func(p);
        REQUIRE(p.use_count() == 2);
        REQUIRE(captured_one(std::make_shared<int>(2)) == 5);
    }
    REQUIRE(p.use_count() == 1);
}

TEST_CASE(
    "a captured value can be moved into the front_binding function object",
    "[front_binding]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::front_binding>(
        [](const auto& x, const auto& y) { return *x + *y; });

    auto captured_one = func(std::move(p));
    REQUIRE_FALSE(p);
    REQUIRE(captured_one(std::make_unique<int>(2)) == 5);
}

TEST_CASE(
    "a captured value is moved to the front_binding function if called on "
    "an rvalue",
    "[front_binding]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::front_binding>(
        [](auto x, auto y) { return *x + *y; });

    auto captured_one = func(std::move(p));
    REQUIRE_FALSE(p);
    REQUIRE(std::move(captured_one)(std::make_unique<int>(2)) == 5);
}

TEST_CASE("a move only type can be reference wrapped in the capture of a "
          "front_binding function object",
          "[front_binding]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::front_binding>(
        [](std::unique_ptr<int>& x, auto y) { return *x + *y; });
    auto captured_one = func(composer::ref(p));
    REQUIRE(p);
    REQUIRE(captured_one(std::make_unique<int>(2)) == 5);
}

TEST_CASE("a reference wrapped capture is passed as lvalue reference even on "
          "an rvalue front_binding function object",
          "[front_binding]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::front_binding>(
        [](std::unique_ptr<int>& x, auto y) { return *x + *y; });
    auto captured_one = func(composer::ref(p));
    REQUIRE(p);
    REQUIRE(std::move(captured_one)(std::make_unique<int>(2)) == 5);
}

TEST_CASE("a non-const reference wrapped capture is passed as non-const lvalue "
          "reference even on a const front_binding function object",
          "[front_binding]")
{
    auto p = std::make_unique<int>(3);
    auto func = composer::make_arity_function<2, composer::front_binding>(
        [](std::unique_ptr<int>& x, auto y) { return *x + *y; });
    auto captured_one = func(composer::ref(p));
    REQUIRE(p);
    REQUIRE(std::as_const(captured_one)(std::make_unique<int>(2)) == 5);
}

TEST_CASE("front_binding bound arrays are copied")
{
    static constexpr auto f
        = composer::make_arity_function<2, composer::front_binding>(
            [](auto& p, int) -> auto& { return p; });
    static constexpr auto identity = composer::make_arity_function<1>(
        [](auto x) -> decltype(x) { return x; });
    char array[] = "foo";
    SECTION("non-const function object forwards copy as non-const array")
    {
        auto bound_func = f(array);
        auto& bound_array = bound_func(0);
        STATIC_REQUIRE(std::is_same_v<decltype(bound_array), char (&)[4]>);
        REQUIRE(+bound_array != +array);
        REQUIRE(bound_array == std::string_view("foo"));
    }
    SECTION("const function object forwards copy as const array")
    {
        const auto bound_func = identity | f(array);
        auto& bound_array = bound_func(0);
        STATIC_REQUIRE(
            std::is_same_v<decltype(bound_array), const char (&)[4]>);
        REQUIRE(+bound_array != +array);
        REQUIRE(bound_array == std::string_view("foo"));
    }
    SECTION("an rvalue of a function object with a bound array is not callable")
    {
        // This restriction may be removed in the future, but it's safer to
        // have something unnecessarily constrained that can be relaxed, than
        // to have something relaxed that you regret but can't change because
        // people depend on it.

        auto bound_func = f(array);
        STATIC_REQUIRE(
            !std::is_invocable_v<decltype(std::move(bound_func)), int>);
        STATIC_REQUIRE(
            !std::is_invocable_v<decltype(std::move(std::as_const(bound_func))),
                                 int>);
    }
    SECTION("bound objects are constexpr")
    {
        static constexpr char str[] = "foo";
        static constexpr auto bound_func = f(str);
        constexpr auto& bound_array = bound_func(0);
        STATIC_REQUIRE(
            std::is_same_v<decltype(bound_array), const char (&)[4]>);
        STATIC_REQUIRE(+bound_array != +array);
        STATIC_REQUIRE(bound_array == std::string_view("foo"));
    }
    SECTION("array passed via ref() is bound as references")
    {
        auto bound_func = f(composer::ref(array));
        auto& bound_array = bound_func(0);
        STATIC_REQUIRE(std::is_same_v<decltype(bound_array), char (&)[4]>);
        REQUIRE(+bound_array == +array);
        REQUIRE(bound_array == std::string_view("foo"));
    }
    SECTION("array passed via cref() is bound as const references")
    {
        auto bound_func = f(composer::cref(array));
        auto& bound_array = bound_func(0);
        STATIC_REQUIRE(
            std::is_same_v<decltype(bound_array), const char (&)[4]>);
        REQUIRE(+bound_array == +array);
        REQUIRE(bound_array == std::string_view("foo"));
    }
}
