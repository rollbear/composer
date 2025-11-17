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

inline constexpr auto count_if = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::count_if(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::count_if(std::forward<Ts>(ts)...);
    });

inline constexpr auto find = make_arity_function<4, right_curry>(
    []<typename... Ts>(
        Ts&&... ts) -> decltype(std::ranges::find(std::forward<Ts>(ts)...)) {
        return std::ranges::find(std::forward<Ts>(ts)...);
    });

inline constexpr auto find_if = make_arity_function<4, right_curry>(
    []<typename... Ts>(
        Ts&&... ts) -> decltype(std::ranges::find_if(std::forward<Ts>(ts)...)) {
        return std::ranges::find_if(std::forward<Ts>(ts)...);
    });

inline constexpr auto find_if_not = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_if_not(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::find_if_not(std::forward<Ts>(ts)...);
    });

inline constexpr auto find_last = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_last(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::find_last(std::forward<Ts>(ts)...);
    });

inline constexpr auto find_last_if = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_last_if(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::find_last_if(std::forward<Ts>(ts)...);
    });

inline constexpr auto find_last_if_not = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_last_if_not(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::find_last_if_not(std::forward<Ts>(ts)...);
    });

inline constexpr auto find_end = make_arity_function<7, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_end(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::find_end(std::forward<Ts>(ts)...);
    });

inline constexpr auto find_first_of = make_arity_function<7, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_first_of(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::find_first_of(std::forward<Ts>(ts)...);
    });

inline constexpr auto adjacent_find = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::adjacent_find(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::adjacent_find(std::forward<Ts>(ts)...);
    });

inline constexpr auto search = make_arity_function<7, right_curry>(
    []<typename... Ts>(
        Ts&&... ts) -> decltype(std::ranges::search(std::forward<Ts>(ts)...)) {
        return std::ranges::search(std::forward<Ts>(ts)...);
    });

inline constexpr auto search_n = make_arity_function<6, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::search_n(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::search_n(std::forward<Ts>(ts)...);
    });

inline constexpr auto contains = make_arity_function<4, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::contains(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::contains(std::forward<Ts>(ts)...);
    });
#if defined(__cpp_lib_ranges_starts_ends_with)

inline constexpr auto starts_with = make_arity_function<7, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::starts_with(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::starts_with(std::forward<Ts>(ts)...);
    });

inline constexpr auto ends_with = make_arity_function<7, right_curry>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::ends_with(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::ends_with(std::forward<Ts>(ts)...);
    });

#endif
} // namespace composer

#endif // COMPOSER_ALGORITHM_HPP
