#ifndef COMPOSER_TUPLE_HPP
#define COMPOSER_TUPLE_HPP

#include "back_binding.hpp"
#include "front_binding.hpp"

namespace composer {
template <std::size_t I>
inline constexpr auto get = make_arity_function<1>(
    []<typename T>(T&& t) -> decltype(std::get<I>(std::forward<T>(t))) {
        return std::get<I>(std::forward<T>(t));
    });

inline constexpr auto apply_to = make_arity_function<100, back_binding>(
    []<typename F, typename... Ts>(F&& f, Ts&&... ts)
        -> decltype(std::apply(std::forward<F>(f), std::forward<Ts>(ts)...)) {
        return std::apply(std::forward<F>(f), std::forward<Ts>(ts)...);
    });

inline constexpr auto apply_using = make_arity_function<100, front_binding>(
    []<typename F, typename... Ts>(F&& f, Ts&&... ts)
        -> decltype(std::apply(std::forward<F>(f), std::forward<Ts>(ts)...)) {
        return std::apply(std::forward<F>(f), std::forward<Ts>(ts)...);
    });

} // namespace composer

#endif // COMPOSER_TUPLE_HPP
