#ifndef COMPOSER_FUNCTIONAL_HPP
#define COMPOSER_FUNCTIONAL_HPP

#include "back_binding.hpp"

#include <functional>

namespace composer {

inline constexpr auto mem_fn = make_arity_function<1>(
    []<typename T>(T&& t) -> decltype(make_arity_function<1>(
                              std::mem_fn(std::forward<T>(t)))) {
        return make_arity_function<1>(std::mem_fn(std::forward<T>(t)));
    });

inline constexpr auto equal_to = back_binding<2, std::ranges::equal_to>{};
inline constexpr auto not_equal_to
    = back_binding<2, std::ranges::not_equal_to>{};
inline constexpr auto less_than = back_binding<2, std::ranges::less>{};
inline constexpr auto less_or_equal_to
    = back_binding<2, std::ranges::less_equal>{};
inline constexpr auto greater_than = back_binding<2, std::ranges::greater>{};
inline constexpr auto greater_or_equal_to
    = back_binding<2, std::ranges::greater_equal>{};
inline constexpr auto compare_three_way
    = back_binding<2, std::compare_three_way>{};
inline constexpr auto identity = arity_function<1, std::identity>{};

inline constexpr auto dereference = make_arity_function<1>(
    []<typename P>(P&& p) -> decltype(*std::forward<P>(p)) {
        return *std::forward<P>(p);
    });

inline constexpr auto plus = back_binding<2, std::plus<>>{};
inline constexpr auto minus = back_binding<2, std::minus<>>{};
inline constexpr auto multiplies = back_binding<2, std::multiplies<>>{};
inline constexpr auto divides = back_binding<2, std::divides<>>{};
inline constexpr auto modulus = back_binding<2, std::modulus<>>{};
inline constexpr auto negate = arity_function<1, std::negate<>>{};

inline constexpr auto logical_and = back_binding<2, std::logical_and<>>{};
inline constexpr auto logical_or = back_binding<2, std::logical_or<>>{};
inline constexpr auto logical_not = back_binding<1, std::logical_not<>>{};

inline constexpr auto bit_and = back_binding<2, std::bit_and<>>{};
inline constexpr auto bit_or = back_binding<2, std::bit_or<>>{};
inline constexpr auto bit_xor = back_binding<2, std::bit_xor<>>{};
inline constexpr auto bit_not = arity_function<1, std::bit_not<>>{};

template <typename R, typename C, arity_function_type F>
constexpr auto operator|(R(C::* p), F&& f)
    -> decltype(mem_fn(p) | std::forward<F>(f))
    requires(!requires { std::forward<F>(f)(p); }
             || arity_function_v<decltype(std::forward<F>(f)(p))>)
{
    return mem_fn(p) | std::forward<F>(f);
}

template <typename IN, typename C, typename R>
[[nodiscard]] constexpr auto operator|(IN&& in, R(C::* p))
    -> decltype(mem_fn(p)(std::forward<IN>(in)))
{
    return mem_fn(p)(std::forward<IN>(in));
}

} // namespace composer
#endif // COMPOSER_FUNCTIONAL_HPP
