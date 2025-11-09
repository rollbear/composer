#ifndef COMPOSER_ALGORITHM_HPP
#define COMPOSER_ALGORITHM_HPP

#include "right_curry.hpp"

#include <algorithm>

namespace composer {

inline constexpr auto all_of = make_arity_function<4, right_curry>(
    []<typename... Ts>(
        Ts&&... ts) -> decltype(std::ranges::all_of(std::forward<Ts>(ts)...)) {
        return std::ranges::all_of(std::forward<Ts>(ts)...);
    });

inline constexpr auto any_of = make_arity_function<4, right_curry>(
    []<typename... Ts>(
        Ts&&... ts) -> decltype(std::ranges::any_of(std::forward<Ts>(ts)...)) {
        return std::ranges::any_of(std::forward<Ts>(ts)...);
    });

inline constexpr auto none_of = make_arity_function<4, right_curry>(
    []<typename... Ts>(
        Ts&&... ts) -> decltype(std::ranges::none_of(std::forward<Ts>(ts)...)) {
        return std::ranges::none_of(std::forward<Ts>(ts)...);
    });

inline constexpr auto for_each = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::for_each(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::for_each(std::forward<Ts>(ts)...);
    });

inline constexpr auto for_each_n = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::for_each_n(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::for_each_n(std::forward<Ts>(ts)...);
    });

inline constexpr auto count = make_arity_function<4, right_curry>(
    []<typename... Ts>(
        Ts&&... ts) -> decltype(std::ranges::count(std::forward<Ts>(ts)...)) {
        return std::ranges::count(std::forward<Ts>(ts)...);
    });

} // namespace composer

#endif // COMPOSER_ALGORITHM_HPP
