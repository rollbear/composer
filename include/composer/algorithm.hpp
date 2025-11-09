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

} // namespace composer

#endif // COMPOSER_ALGORITHM_HPP
