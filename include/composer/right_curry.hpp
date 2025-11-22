#ifndef COMPOSER_RIGHT_CURRY_HPP
#define COMPOSER_RIGHT_CURRY_HPP

#include "arity_function.hpp"

#include <tuple>

namespace composer {

namespace internal {

template <typename F, typename... As>
struct back_binder {
    static constexpr auto indexes = std::index_sequence_for<As...>{};
    F f;
    std::tuple<As...> as;

    template <typename Self, typename... Ts>
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> decltype(std::forward<Self>(self).call(indexes,
                                                  std::forward<Ts>(ts)...))
    {
        return std::forward<Self>(self).call(indexes, std::forward<Ts>(ts)...);
    }

    template <typename Self, std::size_t... Is, typename... Ts>
    constexpr auto
    call(this Self&& self, std::index_sequence<Is...>, Ts&&... ts)
        -> decltype(std::forward_like<Self>(self.f)(
            std::forward<Ts>(ts)...,
            std::forward_like<Self>(std::get<Is>(self.as))...))
    {
        return std::forward_like<Self>(self.f)(
            std::forward<Ts>(ts)...,
            std::forward_like<Self>(std::get<Is>(self.as))...);
    }
};
} // namespace internal

template <typename F, std::size_t N>
struct [[nodiscard]] right_curry : arity_function<F, N> {
    using arity_function<F, N>::operator();

    template <typename Self, typename... Ts>
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> right_curry<decltype(internal::back_binder{
                           std::forward<Self>(self),
                           std::tuple<internal::arg_binder_t<Ts>...>(
                               std::forward<Ts>(ts)...) }),
                       N - sizeof...(Ts)>
        requires(sizeof...(Ts) < N) && (!requires {
                    std::forward_like<Self>(self.f)(std::forward<Ts>(ts)...);
                })
    {
        return { std::forward<Self>(self), { std::forward<Ts>(ts)... } };
    }
};

} // namespace composer
#endif // COMPOSER_RIGHT_CURRY_HPP
