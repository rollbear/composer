#ifndef COMPOSER_FUNCTIONAL_HPP
#define COMPOSER_FUNCTIONAL_HPP

#include "back_binding.hpp"

#include <functional>

namespace composer {

inline constexpr auto mem_fn = make_arity_function<1>(
    []<typename T>(T&& t) -> decltype(make_arity_function<1>(
                              nodiscard{ std::mem_fn(std::forward<T>(t)) })) {
        return make_arity_function<1>(
            nodiscard{ std::mem_fn(std::forward<T>(t)) });
    });

inline constexpr auto equal_to
    = back_binding<2, nodiscard<std::ranges::equal_to>>{};
inline constexpr auto not_equal_to
    = back_binding<2, nodiscard<std::ranges::not_equal_to>>{};
inline constexpr auto less_than
    = back_binding<2, nodiscard<std::ranges::less>>{};
inline constexpr auto less_or_equal_to
    = back_binding<2, nodiscard<std::ranges::less_equal>>{};
inline constexpr auto greater_than
    = back_binding<2, nodiscard<std::ranges::greater>>{};
inline constexpr auto greater_or_equal_to
    = back_binding<2, nodiscard<std::ranges::greater_equal>>{};
inline constexpr auto compare_three_way
    = back_binding<2, nodiscard<std::compare_three_way>>{};
inline constexpr auto identity = arity_function<1, nodiscard<std::identity>>{};

inline constexpr auto dereference = make_arity_function<1>(
    nodiscard{ []<typename P>(P&& p) -> decltype(*std::forward<P>(p)) {
        return *std::forward<P>(p);
    } });

inline constexpr auto plus = back_binding<2, nodiscard<std::plus<>>>{};
inline constexpr auto minus = back_binding<2, nodiscard<std::minus<>>>{};
inline constexpr auto multiplies
    = back_binding<2, nodiscard<std::multiplies<>>>{};
inline constexpr auto divides = back_binding<2, nodiscard<std::divides<>>>{};
inline constexpr auto modulus = back_binding<2, nodiscard<std::modulus<>>>{};
inline constexpr auto negate = arity_function<1, nodiscard<std::negate<>>>{};

inline constexpr auto logical_and
    = back_binding<2, nodiscard<std::logical_and<>>>{};
inline constexpr auto logical_or
    = back_binding<2, nodiscard<std::logical_or<>>>{};
inline constexpr auto logical_not
    = back_binding<1, nodiscard<std::logical_not<>>>{};

inline constexpr auto bit_and = back_binding<2, nodiscard<std::bit_and<>>>{};
inline constexpr auto bit_or = back_binding<2, nodiscard<std::bit_or<>>>{};
inline constexpr auto bit_xor = back_binding<2, nodiscard<std::bit_xor<>>>{};
inline constexpr auto bit_not = arity_function<1, nodiscard<std::bit_not<>>>{};

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

#define COMPOSER_MAKE_OP(opname, op)                                        \
    namespace internal {                                                    \
    template <typename LH, typename RH>                                     \
    struct op_##opname {                                                    \
        LH lhf;                                                             \
        RH rhf;                                                             \
        template <typename Self, typename... Ts>                            \
        constexpr auto operator()(this Self&& self, const Ts&... ts)        \
            -> decltype(std::forward_like<Self>(self.lhf)(ts...)            \
                            op std::forward_like<Self>(self.rhf)(ts...))    \
        {                                                                   \
            return std::forward_like<Self>(self.lhf)(ts...)                 \
                op std::forward_like<Self>(self.rhf)(ts...);                \
        }                                                                   \
    };                                                                      \
    }                                                                       \
                                                                            \
    template <arity_function_type LH, arity_function_type RH>               \
    constexpr auto operator op(LH&& lh, RH&& rh)                            \
    {                                                                       \
        using LT = std::remove_cvref_t<LH>;                                 \
        using RT = std::remove_cvref_t<RH>;                                 \
        constexpr auto arity = std::min(LT::arity, RT::arity);              \
                                                                            \
        return make_arity_function<arity>(nodiscard{ internal::op_##opname{ \
            std::forward<LH>(lh), std::forward<RH>(rh) } });                \
    }

COMPOSER_MAKE_OP(eq, ==)
COMPOSER_MAKE_OP(lt, <)
COMPOSER_MAKE_OP(le, <=)
COMPOSER_MAKE_OP(gt, >)
COMPOSER_MAKE_OP(ge, >=)
COMPOSER_MAKE_OP(neq, !=)
COMPOSER_MAKE_OP(or, ||)
COMPOSER_MAKE_OP(and, &&)
COMPOSER_MAKE_OP(plus, +);
COMPOSER_MAKE_OP(minus, -);
COMPOSER_MAKE_OP(times, *);
COMPOSER_MAKE_OP(divides, /);
COMPOSER_MAKE_OP(remainder, %);
COMPOSER_MAKE_OP(ls, <<);
COMPOSER_MAKE_OP(rh, >>);

#undef COMPOSER_MAKE_OP

template <arity_function_type F>
constexpr auto operator!(F&& f) -> decltype(std::forward<F>(f) | logical_not)
{
    return std::forward<F>(f) | logical_not;
}

template <arity_function_type F>
constexpr auto operator*(F&& f) -> decltype(std::forward<F>(f) | dereference)
{
    return std::forward<F>(f) | dereference;
}

} // namespace composer
#endif // COMPOSER_FUNCTIONAL_HPP
