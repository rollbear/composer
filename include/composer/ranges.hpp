#ifndef COMPOSER_RANGES_HPP
#define COMPOSER_RANGES_HPP

#include "composable_function.hpp"

#include <ranges>

namespace composer {
inline constexpr auto size = make_composable_function(nodiscard{
    []<typename T>(T&& t) -> decltype(std::ranges::size(std::forward<T>(t))) {
        return std::ranges::size(std::forward<T>(t));
    } });

inline constexpr auto ssize = make_composable_function(nodiscard{
    []<typename T>(T&& t) -> decltype(std::ranges::ssize(std::forward<T>(t))) {
        return std::ranges::ssize(std::forward<T>(t));
    } });

inline constexpr auto distance = make_composable_function(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::distance(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::distance(std::forward<Ts>(ts)...);
    } });
} // namespace composer
#endif // COMPOSER_RANGES_HPP
