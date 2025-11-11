#ifndef COMPOSER_FUNCTIONAL_HPP
#define COMPOSER_FUNCTIONAL_HPP

#include "right_curry.hpp"

#include <functional>

namespace composer {

inline constexpr auto mem_fn = make_arity_function<1>(
    []<typename T>(T&& t) -> decltype(make_arity_function<1>(
                              std::mem_fn(std::forward<T>(t)))) {
        return make_arity_function<1>(std::mem_fn(std::forward<T>(t)));
    });

inline constexpr auto equal_to = right_curry<std::ranges::equal_to, 2>{};
inline constexpr auto not_equal_to
    = right_curry<std::ranges::not_equal_to, 2>{};
inline constexpr auto less = right_curry<std::ranges::less, 2>{};
inline constexpr auto less_equal = right_curry<std::ranges::less_equal, 2>{};
inline constexpr auto greater = right_curry<std::ranges::greater, 2>{};
inline constexpr auto greater_equal
    = right_curry<std::ranges::greater_equal, 2>{};
inline constexpr auto compare_three_way
    = right_curry<std::compare_three_way, 2>{};
inline constexpr auto identity = arity_function<std::identity, 1>{};

inline constexpr auto dereference = make_arity_function<1>(
    []<typename P>(P&& p) -> decltype(*std::forward<P>(p)) {
        return *std::forward<P>(p);
    });

inline constexpr auto plus = right_curry<std::plus<>, 2>{};
inline constexpr auto minus = right_curry<std::minus<>, 2>{};
inline constexpr auto multiplies = right_curry<std::multiplies<>, 2>{};
inline constexpr auto divides = right_curry<std::divides<>, 2>{};
inline constexpr auto modulus = right_curry<std::modulus<>, 2>{};
inline constexpr auto negate = arity_function<std::negate<>, 1>{};

inline constexpr auto logical_and = right_curry<std::logical_and<>, 2>{};
inline constexpr auto logical_or = right_curry<std::logical_or<>, 2>{};
inline constexpr auto logical_not = right_curry<std::logical_not<>, 1>{};

inline constexpr auto bit_and = right_curry<std::bit_and<>, 2>{};
inline constexpr auto bit_or = right_curry<std::bit_or<>, 2>{};
inline constexpr auto bit_xor = right_curry<std::bit_xor<>, 2>{};
inline constexpr auto bit_not = arity_function<std::bit_not<>, 1>{};

template <typename R, typename C, arity_function_type F>
inline constexpr auto operator|(R(C::* p), F&& f)
    -> decltype(mem_fn(p) | std::forward<F>(f))
    requires(!requires { std::forward<F>(f)(p); })
{
    return mem_fn(p) | std::forward<F>(f);
}

} // namespace composer
#endif // COMPOSER_FUNCTIONAL_HPP
