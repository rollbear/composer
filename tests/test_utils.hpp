#ifndef COMPOSER_TEST_UTILS_HPP
#define COMPOSER_TEST_UTILS_HPP

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
#endif // COMPOSER_TEST_UTILS_HPP
