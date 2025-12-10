#include <composer/functional.hpp>
#include <composer/ranges.hpp>

#include <catch2/catch_test_macros.hpp>

template <typename T>
struct strict {
    T t;

    constexpr bool operator==(T v) const { return t == v; }

    template <typename U>
    constexpr bool operator==(U) const = delete;
};

using s = strict<size_t>;
using ss = strict<ptrdiff_t>;

SCENARIO("size")
{
    SECTION("size can be called with a c-array")
    {
        STATIC_REQUIRE(composer::size("foo") == s{ 4 });
        REQUIRE(composer::size("foo") == s{ 4 });
    }
    SECTION("size can be called on a type with a .size() member function")
    {
        struct S {
            [[nodiscard]] constexpr size_t size() const { return 3; }
        };

        STATIC_REQUIRE(composer::size(S{}) == s{ 3 });
        REQUIRE(composer::size(S{}) == s{ 3 });
    }
    SECTION("size can be called on a type with begin/end member functions")
    {
        struct S {
            std::array<int, 4> a{};

            [[nodiscard]] constexpr auto begin() const { return a.begin(); }

            [[nodiscard]] constexpr auto end() const { return a.end(); }
        };

        constexpr S obj{};
        STATIC_REQUIRE(composer::size(obj) == s{ 4 });
        REQUIRE(composer::size(obj) == s{ 4 });
    }
    SECTION("size can be piped")
    {
        STATIC_REQUIRE("foo" | composer::size | composer::equal_to(4U));
        REQUIRE(("foo" | composer::size | composer::equal_to(4U)));
    }
}

SCENARIO("ssize")
{
    SECTION("ssize can be called with a c-array")
    {
        STATIC_REQUIRE(composer::ssize("foo") == ss{ 4 });
        REQUIRE(composer::ssize("foo") == ss{ 4 });
    }
    SECTION("ssize can be called on a type with a .size() member function")
    {
        struct S {
            [[nodiscard]] constexpr ptrdiff_t size() const { return 3; }
        };

        STATIC_REQUIRE(composer::ssize(S{}) == ss{ 3 });
        REQUIRE(composer::ssize(S{}) == ss{ 3 });
    }
    SECTION("ssize can be called on a type with begin/end member functions")
    {
        struct S {
            std::array<int, 4> a{};

            [[nodiscard]] constexpr auto begin() const { return a.begin(); }

            [[nodiscard]] constexpr auto end() const { return a.end(); }
        };

        constexpr S obj{};
        STATIC_REQUIRE(composer::ssize(obj) == ss{ 4 });
        REQUIRE(composer::ssize(obj) == ss{ 4 });
    }
    SECTION("ssize can be piped")
    {
        STATIC_REQUIRE("foo" | composer::ssize | composer::equal_to(4));
        REQUIRE(("foo" | composer::ssize | composer::equal_to(4)));
    }
}

SCENARIO("distance")
{
    static constexpr std::string_view str = "abcd";
    SECTION(
        "distance called with 2 iterators calls ranges::distance immediately")
    {
        STATIC_REQUIRE(composer::distance(str.begin(), str.end()) == ss{ 4 });
        REQUIRE(composer::distance(str.begin(), str.end()) == ss{ 4 });
    }
    SECTION("distance called with a range calls ranges::distance immediately")
    {
        STATIC_REQUIRE(composer::distance(str) == ss{ 4 });
        REQUIRE(composer::distance(str) == ss{ 4 });
    }
    SECTION("the return type from distance is signed")
    {
        STATIC_REQUIRE(std::is_signed_v<decltype(composer::distance(str))>);
        STATIC_REQUIRE(std::is_signed_v<decltype(composer::distance(
                           str.begin(), str.end()))>);
    }
    SECTION("distance can be piped")
    {
        STATIC_REQUIRE(str | composer::distance | composer::equal_to(4));
        REQUIRE((str | composer::distance | composer::equal_to(4)));
    }
}
