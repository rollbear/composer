#ifndef COMPOSER_LEFT_CURRY_HPP
#define COMPOSER_LEFT_CURRY_HPP

#include "arity_function.hpp"

#include <functional>

namespace composer {

template <typename F, size_t N>
struct left_curry : arity_function<F, N> {
    using arity_function<F, N>::operator();

    template <typename Self, typename... Ts>
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> left_curry<decltype(std::bind_front(std::forward<Self>(self),
                                               std::forward<Ts>(ts)...)),
                      N - sizeof...(Ts)>
        requires(sizeof...(Ts) < N) && (!requires {
                    std::forward_like<Self>(self.f)(std::forward<Ts>(ts)...);
                })
    {
        return { std::bind_front(std::forward<Self>(self),
                                 std::forward<Ts>(ts)...) };
    }
};

} // namespace composer
#endif // COMPOSER_LEFT_CURRY_HPP
