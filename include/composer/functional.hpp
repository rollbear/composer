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

inline constexpr auto equal_to = back_binding<std::ranges::equal_to, 2>{};
inline constexpr auto not_equal_to
    = back_binding<std::ranges::not_equal_to, 2>{};
inline constexpr auto less_than = back_binding<std::ranges::less, 2>{};
inline constexpr auto less_or_equal_to
    = back_binding<std::ranges::less_equal, 2>{};
inline constexpr auto greater_than = back_binding<std::ranges::greater, 2>{};
inline constexpr auto greater_or_equal_to
    = back_binding<std::ranges::greater_equal, 2>{};
inline constexpr auto compare_three_way
    = back_binding<std::compare_three_way, 2>{};
inline constexpr auto identity = arity_function<std::identity, 1>{};

inline constexpr auto dereference = make_arity_function<1>(
    []<typename P>(P&& p) -> decltype(*std::forward<P>(p)) {
        return *std::forward<P>(p);
    });

inline constexpr auto plus = back_binding<std::plus<>, 2>{};
inline constexpr auto minus = back_binding<std::minus<>, 2>{};
inline constexpr auto multiplies = back_binding<std::multiplies<>, 2>{};
inline constexpr auto divides = back_binding<std::divides<>, 2>{};
inline constexpr auto modulus = back_binding<std::modulus<>, 2>{};
inline constexpr auto negate = arity_function<std::negate<>, 1>{};

inline constexpr auto logical_and = back_binding<std::logical_and<>, 2>{};
inline constexpr auto logical_or = back_binding<std::logical_or<>, 2>{};
inline constexpr auto logical_not = back_binding<std::logical_not<>, 1>{};

inline constexpr auto bit_and = back_binding<std::bit_and<>, 2>{};
inline constexpr auto bit_or = back_binding<std::bit_or<>, 2>{};
inline constexpr auto bit_xor = back_binding<std::bit_xor<>, 2>{};
inline constexpr auto bit_not = arity_function<std::bit_not<>, 1>{};

template <typename R, typename C, arity_function_type F>
constexpr auto operator|(R(C::* p), F&& f)
    -> decltype(mem_fn(p) | std::forward<F>(f))
    requires(!requires { std::forward<F>(f)(p); })
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
