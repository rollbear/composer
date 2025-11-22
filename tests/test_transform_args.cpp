#include <composer/front_binding.hpp>
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

TEST_CASE("transform_args of a front bound function is a front bound function")
{
    constexpr auto f
        = composer::make_arity_function<2, composer::front_binding>(
            [](auto a, auto b) { return a - b; });
    constexpr auto deref_f = composer::transform_args(
        [](auto p) -> decltype(*p) { return *p; }, f);
    static constexpr int a = 5;
    constexpr auto sub_from_5 = deref_f(&a);
    static constexpr int b = 2;
    STATIC_REQUIRE(sub_from_5(&b) == 3);
    REQUIRE(sub_from_5(&b) == 3);
}

TEST_CASE("transform_args of a partially bound front binding function is "
          "callable with one arg through the transformation")
{
    constexpr auto minus
        = composer::make_arity_function<2, composer::front_binding>(
            [](auto a, auto b) -> decltype(a - b) { return a - b; });
    constexpr auto sub_from_5 = minus(5);
    constexpr auto dsub = composer::transform_args(
        [](auto p) -> decltype(*p) { return *p; }, sub_from_5);
    static constexpr int b = 2;
    STATIC_REQUIRE(dsub(&b) == 3);
    REQUIRE(dsub(&b) == 3);
}

TEST_CASE("transform_args can transform via a pointer to member")
{
    struct S {
        int a;
        int b;

        constexpr int get_b() const { return b; }
    };

    constexpr S s{ 5, 2 };
    constexpr auto equal_to
        = composer::back_binding<std::ranges::equal_to, 2>{};
    STATIC_REQUIRE(composer::transform_args(&S::a, equal_to(5))(s));
    REQUIRE(composer::transform_args(&S::a, equal_to(5))(s));
    STATIC_REQUIRE(composer::transform_args(&S::get_b, equal_to(2))(s));
    REQUIRE(composer::transform_args(&S::get_b, equal_to(2))(s));
}

TEST_CASE("transform_args is front binding")
{
    constexpr auto dereference
        = [](const auto& p) -> decltype(*p) { return *p; };
    constexpr auto dereferenced_args = composer::transform_args(dereference);
    constexpr auto equal_to
        = composer::back_binding<std::ranges::equal_to, 2>{};
    constexpr int a = 2;
    STATIC_REQUIRE(dereferenced_args(equal_to(2))(&a));
    STATIC_REQUIRE_FALSE(dereferenced_args(equal_to(3))(&a));
    REQUIRE(dereferenced_args(equal_to(2))(&a));
    REQUIRE_FALSE(dereferenced_args(equal_to(3))(&a));
}
