#ifndef COMPOSER_RIGHT_CURRY_HPP
#define COMPOSER_RIGHT_CURRY_HPP

#include "arity_function.hpp"

#include <functional>

namespace composer {

template <typename F, size_t N>
struct right_curry : arity_function<F, N> {
    using arity_function<F, N>::operator();

    template <typename Self, typename... Ts>
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> right_curry<decltype(std::bind_back(std::forward<Self>(self),
                                               std::forward<Ts>(ts)...)),
                       N - sizeof...(Ts)>
        requires(sizeof...(Ts) < N) && (!requires {
                    std::forward_like<Self>(self.f)(std::forward<Ts>(ts)...);
                })
    {
        return { std::bind_back(std::forward<Self>(self),
                                std::forward<Ts>(ts)...) };
    }
};

} // namespace composer
#endif // COMPOSER_RIGHT_CURRY_HPP
