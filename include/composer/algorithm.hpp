#ifndef COMPOSER_ALGORITHM_HPP
#define COMPOSER_ALGORITHM_HPP

#include "back_binding.hpp"

#include <algorithm>

namespace composer {

inline constexpr auto all_of = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::all_of(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::all_of(std::forward<Ts>(ts)...);
    } });

inline constexpr auto any_of = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::any_of(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::any_of(std::forward<Ts>(ts)...);
    } });

inline constexpr auto none_of = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::none_of(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::none_of(std::forward<Ts>(ts)...);
    } });

inline constexpr auto for_each = make_arity_function<4, back_binding>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::for_each(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::for_each(std::forward<Ts>(ts)...);
    });

inline constexpr auto for_each_n = make_arity_function<4, back_binding>(
    []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::for_each_n(
                                       std::forward<Ts>(ts)...)) {
        return std::ranges::for_each_n(std::forward<Ts>(ts)...);
    });

inline constexpr auto count = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::count(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::count(std::forward<Ts>(ts)...);
    } });

inline constexpr auto count_if = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::count_if(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::count_if(std::forward<Ts>(ts)...);
    } });

inline constexpr auto find = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::find(std::forward<Ts>(ts)...);
    } });

inline constexpr auto find_if = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_if(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::find_if(std::forward<Ts>(ts)...);
    } });

inline constexpr auto find_if_not
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_if_not(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::find_if_not(std::forward<Ts>(ts)...);
        } });

inline constexpr auto find_last
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_last(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::find_last(std::forward<Ts>(ts)...);
        } });

inline constexpr auto find_last_if
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_last_if(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::find_last_if(std::forward<Ts>(ts)...);
        } });

inline constexpr auto find_last_if_not = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts)
                   -> decltype(std::ranges::find_last_if_not(
                       std::forward<Ts>(ts)...)) {
        return std::ranges::find_last_if_not(std::forward<Ts>(ts)...);
    } });

inline constexpr auto find_end = make_arity_function<7, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_end(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::find_end(std::forward<Ts>(ts)...);
    } });

inline constexpr auto find_first_of
    = make_arity_function<7, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::find_first_of(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::find_first_of(std::forward<Ts>(ts)...);
        } });

inline constexpr auto adjacent_find
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::adjacent_find(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::adjacent_find(std::forward<Ts>(ts)...);
        } });

inline constexpr auto search = make_arity_function<7, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::search(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::search(std::forward<Ts>(ts)...);
    } });

inline constexpr auto search_n = make_arity_function<6, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::search_n(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::search_n(std::forward<Ts>(ts)...);
    } });

inline constexpr auto contains = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::contains(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::contains(std::forward<Ts>(ts)...);
    } });

inline constexpr auto contains_subrange = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts)
                   -> decltype(std::ranges::contains_subrange(
                       std::forward<Ts>(ts)...)) {
        return std::ranges::contains_subrange(std::forward<Ts>(ts)...);
    } });

#if defined(__cpp_lib_ranges_starts_ends_with)

inline constexpr auto starts_with
    = make_arity_function<7, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::starts_with(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::starts_with(std::forward<Ts>(ts)...);
        } });

inline constexpr auto ends_with
    = make_arity_function<7, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::ends_with(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::ends_with(std::forward<Ts>(ts)...);
        } });

#endif

inline constexpr auto is_partitioned
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::is_partitioned(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::is_partitioned(std::forward<Ts>(ts)...);
        } });

inline constexpr auto partition_point
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::partition_point(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::partition_point(std::forward<Ts>(ts)...);
        } });

inline constexpr auto is_sorted
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::is_sorted(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::is_sorted(std::forward<Ts>(ts)...);
        } });

inline constexpr auto is_sorted_until
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::is_sorted_until(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::is_sorted_until(std::forward<Ts>(ts)...);
        } });

inline constexpr auto lower_bound
    = make_arity_function<5, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::lower_bound(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::lower_bound(std::forward<Ts>(ts)...);
        } });

inline constexpr auto upper_bound
    = make_arity_function<5, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::upper_bound(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::upper_bound(std::forward<Ts>(ts)...);
        } });

inline constexpr auto binary_search
    = make_arity_function<5, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::binary_search(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::binary_search(std::forward<Ts>(ts)...);
        } });

inline constexpr auto equal_range
    = make_arity_function<5, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::equal_range(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::equal_range(std::forward<Ts>(ts)...);
        } });

inline constexpr auto includes = make_arity_function<7, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::includes(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::includes(std::forward<Ts>(ts)...);
    } });

inline constexpr auto is_heap = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::is_heap(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::is_heap(std::forward<Ts>(ts)...);
    } });

inline constexpr auto is_heap_until
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::is_heap_until(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::is_heap_until(std::forward<Ts>(ts)...);
        } });

inline constexpr auto max = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::max(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::max(std::forward<Ts>(ts)...);
    } });

inline constexpr auto max_element
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::max_element(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::max_element(std::forward<Ts>(ts)...);
        } });

inline constexpr auto min = make_arity_function<4, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::min(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::min(std::forward<Ts>(ts)...);
    } });

inline constexpr auto min_element
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::min_element(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::min_element(std::forward<Ts>(ts)...);
        } });

inline constexpr auto minmax_element
    = make_arity_function<4, back_binding>(nodiscard{
        []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::minmax_element(
                                           std::forward<Ts>(ts)...)) {
            return std::ranges::minmax_element(std::forward<Ts>(ts)...);
        } });

inline constexpr auto clamp = make_arity_function<5, back_binding>(
    nodiscard{ []<typename... Ts>(Ts&&... ts) -> decltype(std::ranges::clamp(
                                                  std::forward<Ts>(ts)...)) {
        return std::ranges::clamp(std::forward<Ts>(ts)...);
    } });

} // namespace composer

#endif // COMPOSER_ALGORITHM_HPP
