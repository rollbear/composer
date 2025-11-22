#include <composer/functional.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("less_than is back binding")
{
    SECTION("when called with 2 args, the result is arg1 < arg2")
    {
        STATIC_REQUIRE_FALSE(composer::less_than(2, 2));
        STATIC_REQUIRE(composer::less_than(1, 2));
        REQUIRE_FALSE(composer::less_than(2, 2));
        REQUIRE(composer::less_than(1, 2));
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto lt2 = composer::less_than(2);
        STATIC_REQUIRE_FALSE(lt2(2));
        STATIC_REQUIRE(lt2(1));
        REQUIRE_FALSE(lt2(2));
        REQUIRE(lt2(1));
    }
}

TEST_CASE("less_or_equal_to is back binding")
{
    SECTION("when called with 2 args, the result is arg1 <= arg2")
    {
        STATIC_REQUIRE_FALSE(composer::less_or_equal_to(3, 2));
        STATIC_REQUIRE(composer::less_or_equal_to(2, 2));
        REQUIRE_FALSE(composer::less_or_equal_to(3, 2));
        REQUIRE(composer::less_or_equal_to(2, 2));
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto le2 = composer::less_or_equal_to(2);
        STATIC_REQUIRE_FALSE(le2(3));
        STATIC_REQUIRE(le2(2));
        REQUIRE_FALSE(le2(3));
        REQUIRE(le2(2));
    }
}

TEST_CASE("greater_than is back binding")
{
    SECTION("when called with 2 args, the result is arg1 > arg2")
    {
        STATIC_REQUIRE_FALSE(composer::greater_than(2, 2));
        STATIC_REQUIRE(composer::greater_than(3, 2));
        REQUIRE_FALSE(composer::greater_than(2, 2));
        REQUIRE(composer::greater_than(3, 2));
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto gt2 = composer::greater_than(2);
        STATIC_REQUIRE_FALSE(gt2(2));
        STATIC_REQUIRE(gt2(3));
        REQUIRE_FALSE(gt2(2));
        REQUIRE(gt2(3));
    }
}

TEST_CASE("greater_or_equal_to is back binding")
{
    SECTION("when called with 2 args, the result is arg1 >= arg2")
    {
        STATIC_REQUIRE_FALSE(composer::greater_or_equal_to(1, 2));
        STATIC_REQUIRE(composer::greater_or_equal_to(2, 2));
        REQUIRE_FALSE(composer::greater_or_equal_to(1, 2));
        REQUIRE(composer::greater_or_equal_to(2, 2));
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto ge2 = composer::greater_or_equal_to(2);
        STATIC_REQUIRE_FALSE(ge2(1));
        STATIC_REQUIRE(ge2(2));
        REQUIRE_FALSE(ge2(1));
        REQUIRE(ge2(2));
    }
}

TEST_CASE("compare_three_way is back binding")
{
    SECTION("when called with 2 args, the result is arg1 <=> arg2")
    {
        STATIC_REQUIRE(composer::compare_three_way(1, 2)
                       == std::strong_ordering::less);
        STATIC_REQUIRE(composer::compare_three_way(2, 2)
                       == std::strong_ordering::equal);
        REQUIRE(composer::compare_three_way(1, 2)
                == std::strong_ordering::less);
        REQUIRE(composer::compare_three_way(2, 2)
                == std::strong_ordering::equal);
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto compare = composer::compare_three_way(2);
        STATIC_REQUIRE(compare(1) == std::strong_ordering::less);
        STATIC_REQUIRE(compare(2) == std::strong_ordering::equal);
        REQUIRE(compare(1) == std::strong_ordering::less);
        REQUIRE(compare(2) == std::strong_ordering::equal);
    }
}

TEST_CASE("plus is back binding")
{
    using namespace std::string_literals;
    SECTION("when called with 2 args, the result is arg1 + arg2")
    {
        REQUIRE(composer::plus("1"s, "2"s) == "12");
    }
    SECTION("when called with one arg, it binds to the right")
    {
        auto append_two = composer::plus("2"s);
        REQUIRE(append_two("1") == "12");
    }
    SECTION("and it's also constexpr")
    {
        STATIC_REQUIRE(composer::plus(1, 3) == 4);
        STATIC_REQUIRE(composer::plus(1)(3) == 4);
    }
}

TEST_CASE("minus is back binding")
{
    SECTION("when called with 2 args, the result is arg1 - arg2")
    {
        STATIC_REQUIRE(composer::minus(5, 3) == 2);
        REQUIRE(composer::minus(5, 3) == 2);
    }
    SECTION("when called with one arg, it binds to the right")
    {
        static constexpr auto minus_two = composer::minus(2);
        STATIC_REQUIRE(minus_two(5) == 3);
        REQUIRE(minus_two(5) == 3);
    }
}

struct stringmul {
    std::string s;

    friend std::string operator*(const stringmul& lh, const stringmul& rh)
    {
        return lh.s + "*" + rh.s;
    }
};

TEST_CASE("multiplies is back binding")
{
    SECTION("when called with 2 args, the result is arg1 * arg2")
    {
        REQUIRE(composer::multiplies(stringmul{ "a" }, stringmul{ "b" })
                == "a*b");
    }
    SECTION("when called with one arg, it binds to the right")
    {
        auto mulr = composer::multiplies(stringmul{ "rh" });
        REQUIRE(mulr(stringmul{ "lh" }) == "lh*rh");
    }
    SECTION("multiplies is constexpr")
    {
        STATIC_REQUIRE(composer::multiplies(2, 3) == 6);
        STATIC_REQUIRE(composer::multiplies(2)(3) == 6);
    }
}

TEST_CASE("divides is back binding")
{
    SECTION("when called with 2 args, the result is arg1 / arg2")
    {
        STATIC_REQUIRE(composer::divides(8, 2) == 4);
        REQUIRE(composer::divides(8, 2) == 4);
    }
    SECTION("when called with 1 arg, it binds to the right")
    {
        constexpr auto half = composer::divides(2);
        STATIC_REQUIRE(half(10) == 5);
        REQUIRE(half(10) == 5);
    }
}

TEST_CASE("modulus is back binding")
{
    SECTION("when called with 2 args, the result is arg1 % arg2")
    {
        STATIC_REQUIRE(composer::modulus(8, 3) == 2);
        REQUIRE(composer::modulus(8, 3) == 2);
    }
    SECTION("when called with 1 arg, it binds to the right")
    {
        constexpr auto mod3 = composer::modulus(3);
        STATIC_REQUIRE(mod3(8) == 2);
        REQUIRE(mod3(8) == 2);
    }
}

TEST_CASE("negate is unary negation")
{
    STATIC_REQUIRE(composer::negate(5) == -5);
}

TEST_CASE("mem_fn selects member variable or member function")
{
    struct S {
        int i;

        constexpr int get() const { return i + 1; }
    };

    SECTION("member variable selection is constexpr")
    {
        constexpr S s{ 3 };
        STATIC_REQUIRE(composer::mem_fn(&S::i)(s) == 3);
        REQUIRE(composer::mem_fn(&S::i)(s) == 3);
    }
    SECTION("member function selection is constexpr")
    {
        STATIC_REQUIRE(composer::mem_fn(&S::get)(S{ 3 }) == 4);
        REQUIRE(composer::mem_fn(&S::get)(S{ 3 }) == 4);
    }
}

TEST_CASE("a piped expression from a pointer to member to an arity function "
          "yields a mem_ln composition")
{
    struct S {
        int a;
        int b;

        constexpr int get_a() const { return a; }
    };

    constexpr auto plus1 = composer::plus(1);
    SECTION("pointer to member variable is constexpr")
    {
        STATIC_REQUIRE((&S::b | plus1)(S{ 3, 5 }) == 6);
        REQUIRE((&S::b | plus1)(S{ 3, 5 }) == 6);
    }
    SECTION("pointer to member function is constexpr")
    {
        STATIC_REQUIRE((&S::get_a | plus1)(S{ 3, 5 }) == 4);
        REQUIRE((&S::get_a | plus1)(S{ 3, 5 }) == 4);
    }
}

struct Bool {
    bool b;

    constexpr friend bool operator&&(bool lh, Bool rh) { return lh && rh.b; }

    bool operator&&(bool) const = delete;

    constexpr friend bool operator||(bool lh, Bool rh) { return lh || rh.b; }

    bool operator||(bool) const = delete;
};

TEST_CASE("logical_and is back binding")
{
    SECTION("called with two args the result is arg1 && arg2")
    {
        STATIC_REQUIRE_FALSE(composer::logical_and(false, false));
        STATIC_REQUIRE_FALSE(composer::logical_and(false, true));
        STATIC_REQUIRE_FALSE(composer::logical_and(true, false));
        STATIC_REQUIRE(composer::logical_and(true, true));
        REQUIRE_FALSE(composer::logical_and(false, false));
        REQUIRE_FALSE(composer::logical_and(false, true));
        REQUIRE_FALSE(composer::logical_and(true, false));
        REQUIRE(composer::logical_and(true, true));
    }
    SECTION("called with one argument binds the right hand side")
    {
        static constexpr auto and_false = composer::logical_and(Bool{ false });
        static constexpr auto and_true = composer::logical_and(Bool{ true });

        STATIC_REQUIRE_FALSE(and_false(false));
        STATIC_REQUIRE_FALSE(and_false(true));
        STATIC_REQUIRE_FALSE(and_true(false));
        STATIC_REQUIRE(and_true(true));
        REQUIRE_FALSE(and_false(false));
        REQUIRE_FALSE(and_false(true));
        REQUIRE_FALSE(and_true(false));
        REQUIRE(and_true(true));
    }
}

TEST_CASE("logical_or is back binding")
{
    SECTION("called with two args the result is arg1 || arg2")
    {
        STATIC_REQUIRE_FALSE(composer::logical_or(false, false));
        STATIC_REQUIRE(composer::logical_or(false, true));
        STATIC_REQUIRE(composer::logical_or(true, false));
        STATIC_REQUIRE(composer::logical_or(true, true));
        REQUIRE_FALSE(composer::logical_or(false, false));
        REQUIRE(composer::logical_or(false, true));
        REQUIRE(composer::logical_or(true, false));
        REQUIRE(composer::logical_or(true, true));
    }
    SECTION("called with one argument binds the right hand side")
    {
        static constexpr auto or_false = composer::logical_or(Bool{ false });
        static constexpr auto or_true = composer::logical_or(Bool{ true });

        STATIC_REQUIRE_FALSE(or_false(false));
        STATIC_REQUIRE(or_false(true));
        STATIC_REQUIRE(or_true(false));
        STATIC_REQUIRE(or_true(true));
        REQUIRE_FALSE(or_false(false));
        REQUIRE(or_false(true));
        REQUIRE(or_true(false));
        REQUIRE(or_true(true));
    }
}

TEST_CASE("logical not inverts a bool expression")
{
    STATIC_REQUIRE(composer::equal_to(1, 2) | composer::logical_not);
    STATIC_REQUIRE_FALSE(composer::equal_to(2, 2) | composer::logical_not);
    REQUIRE(composer::equal_to(1, 2) | composer::logical_not);
    REQUIRE_FALSE(composer::equal_to(2, 2) | composer::logical_not);
}

template <std::integral T>
struct Bits {
    T value;

    constexpr friend T operator&(T t, Bits b) { return t & b.value; }

    T operator&(T) const = delete;

    constexpr friend T operator|(T t, Bits b) { return t | b.value; }

    T operator|(T) const = delete;

    constexpr friend T operator^(T t, Bits b) { return t ^ b.value; }

    T operator^(T) const = delete;
};

TEST_CASE("bit_and is back binding")
{
    SECTION("when called with two args returns arg1 & arg2")
    {
        STATIC_REQUIRE(composer::bit_and(0xffffU, 0x00ff00ffU) == 0x000000ff);
        STATIC_REQUIRE(composer::bit_and(0xffffU, 0xff00ff00U) == 0x0000ff00);
        REQUIRE(composer::bit_and(0xffffU, 0x00ff00ffU) == 0x000000ff);
        REQUIRE(composer::bit_and(0xffffU, 0xff00ff00U) == 0x0000ff00);
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto mask_low = composer::bit_and(Bits(0x0000ffffU));
        constexpr auto mask_high = composer::bit_and(Bits(0xffff0000U));
        STATIC_REQUIRE(mask_low(0x00ff00ffU) == 0x000000ffU);
        STATIC_REQUIRE(mask_high(0x00ff00ffU) == 0x00ff0000U);
        REQUIRE(mask_low(0x00ff00ffU) == 0x000000ffU);
        REQUIRE(mask_high(0x00ff00ffU) == 0x00ff0000U);
    }
}

TEST_CASE("bit_or is back binding")
{
    SECTION("when called with two args returns arg1 | arg2")
    {
        STATIC_REQUIRE(composer::bit_or(0xffffU, 0x00ff00ffU) == 0x00ffffffU);
        STATIC_REQUIRE(composer::bit_or(0xffffU, 0xff00ff00U) == 0xff00ffffU);
        REQUIRE(composer::bit_or(0xffffU, 0x00ff00ffU) == 0x00ffffffU);
        REQUIRE(composer::bit_or(0xffffU, 0xff00ff00U) == 0xff00ffffU);
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto set_low = composer::bit_or(Bits(0x0000ffffU));
        constexpr auto set_high = composer::bit_or(Bits(0xffff0000U));
        STATIC_REQUIRE(set_low(0x00ff00ffU) == 0x00ffffffU);
        STATIC_REQUIRE(set_high(0x00ff00ffU) == 0xffff00ffU);
        REQUIRE(set_low(0x00ff00ffU) == 0x00ffffffU);
        REQUIRE(set_high(0x00ff00ffU) == 0xffff00ffU);
    }
}

TEST_CASE("bit_xor is back binding")
{
    SECTION("when called with two args returns arg1 | arg2")
    {
        STATIC_REQUIRE(composer::bit_xor(0xffffU, 0x00ff00ffU) == 0x00ffff00U);
        STATIC_REQUIRE(composer::bit_xor(0xffffU, 0xff00ff00U) == 0xff0000ffU);
        REQUIRE(composer::bit_xor(0xffffU, 0x00ff00ffU) == 0x00ffff00U);
        REQUIRE(composer::bit_xor(0xffffU, 0xff00ff00U) == 0xff0000ffU);
    }
    SECTION("when called with one arg, it binds to the right")
    {
        constexpr auto flip_low = composer::bit_xor(Bits(0x0000ffffU));
        constexpr auto flip_high = composer::bit_xor(Bits(0xffff0000U));
        STATIC_REQUIRE(flip_low(0x00ff00ffU) == 0x00ffff00U);
        STATIC_REQUIRE(flip_high(0x00ff00ffU) == 0xff0000ffU);
        REQUIRE(flip_low(0x00ff00ffU) == 0x00ffff00U);
        REQUIRE(flip_high(0x00ff00ffU) == 0xff0000ffU);
    }
}

TEST_CASE("bit_not flips all bits")
{
    STATIC_REQUIRE((composer::bit_xor(0xffffU, 0x00ff00ffU) | composer::bit_not)
                   == 0xff0000ffU);
    REQUIRE((composer::bit_xor(0xffffU, 0x00ff00ffU) | composer::bit_not)
            == 0xff0000ffU);
}

TEST_CASE("pipe to pointer-to-member")
{
    using composer::operator|;

    struct S {
        int x;
        int y;

        constexpr int get_y() const { return y; }
    };

    SECTION("pipe forwards the correct member")
    {
        S s{ 5, 3 };
        REQUIRE((s | &S::x) == 5);
        REQUIRE((s | &S::y) == 3);
        REQUIRE((s | &S::get_y) == 3);
    }
    SECTION("pipe projects result using the correct cv-ref")
    {
        S s{ 5, 3 };
        STATIC_REQUIRE(std::is_same_v<decltype(s | &S::x), int&>);
        STATIC_REQUIRE(
            std::is_same_v<decltype(std::as_const(s) | &S::x), const int&>);
        STATIC_REQUIRE(std::is_same_v<decltype(std::move(s) | &S::x), int&&>);
        STATIC_REQUIRE(
            std::is_same_v<decltype(std::move(std::as_const(s)) | &S::x),
                           const int&&>);
    }
    SECTION("pipe is constexpr")
    {
        static constexpr S s{ 5, 3 };
        STATIC_REQUIRE((s | &S::x) == 5);
        STATIC_REQUIRE((s | &S::get_y) == 3);
    }
}
