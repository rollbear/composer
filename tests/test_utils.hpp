#ifndef COMPOSER_TEST_UTILS_HPP
#define COMPOSER_TEST_UTILS_HPP

#include <composer/arity_function.hpp>

#include <type_traits>

template <typename LH, typename RH>
constexpr bool can_pipe(LH&& lh, RH&& rh)
{
    return requires { std::forward<LH>(lh) | std::forward<RH>(rh); };
}

template <typename F, typename... As>
constexpr bool can_call(F&& f, As&&... as)
{
    return requires { std::forward<F>(f)(std::forward<As>(as)...); };
}

template <typename F, typename... As>
constexpr bool returns_callable(F&& f, As&&... as)
{
    constexpr auto test = [](composer::arity_function_type auto) {};
    return requires { test(std::forward<F>(f)(std::forward<As>(as)...)); };
}

#endif // COMPOSER_TEST_UTILS_HPP
