#ifndef COMPOSER_TRANSFORM_ARGS_HPP
#define COMPOSER_TRANSFORM_ARGS_HPP

#include "arity_function.hpp"

namespace composer {

namespace internal {
template <typename T, typename F>
struct arg_transformer {
    T t;
    F f;

    template <typename Self, typename... Ts>
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> decltype(std::forward_like<Self>(self.f)(
            std::forward_like<Self>(self.t)(std::forward<Ts>(ts))...))
    {
        return std::forward_like<Self>(self.f)(
            std::forward_like<Self>(self.t)(std::forward<Ts>(ts))...);
    }
};
} // namespace internal

inline constexpr auto transform_args
    = []<typename T, arity_function_type F> [[nodiscard]] (T&& t, F&& f)
    -> internal::rebind_function_t<
        std::remove_cvref_t<F>,
        internal::arg_transformer<std::remove_cvref_t<T>,
                                  std::remove_cvref_t<decltype(f.f)>>,
        std::remove_cvref_t<F>::arity> {
    return { std::forward<T>(t), std::forward_like<F>(f.f) };
};

} // namespace composer

#endif // COMPOSER_TRANSFORM_ARGS_HPP
