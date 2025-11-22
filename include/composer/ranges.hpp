#ifndef COMPOSER_RANGES_HPP
#define COMPOSER_RANGES_HPP

#include "arity_function.hpp"

#include <ranges>

namespace composer {
inline constexpr auto size = make_arity_function<1>(
    []<typename T>(T&& t) -> decltype(std::ranges::size(std::forward<T>(t))) {
        return std::ranges::size(std::forward<T>(t));
    });

inline constexpr auto ssize = make_arity_function<1>(
    []<typename T>(T&& t) -> decltype(std::ranges::ssize(std::forward<T>(t))) {
        return std::ranges::ssize(std::forward<T>(t));
    });

inline constexpr auto distance = make_arity_function<2>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::distance(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::distance(std::forward<Ts>(ts)...);
    });
} // namespace composer
#endif // COMPOSER_RANGES_HPP
