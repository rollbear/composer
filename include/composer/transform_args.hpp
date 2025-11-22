#ifndef COMPOSER_TRANSFORM_ARGS_HPP
#define COMPOSER_TRANSFORM_ARGS_HPP

#include "arity_function.hpp"
#include "functional.hpp"

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

template <typename T>
constexpr inline auto transformation(T&& t)
{
    if constexpr (std::is_member_pointer_v<std::remove_cvref_t<T>>) {
        return composer::mem_fn(std::forward<T>(t));
    } else {
        return t;
    }
}

} // namespace internal

inline constexpr auto transform_args = make_arity_function<2, left_curry>(
    []<typename T, arity_function_type F> [[nodiscard]] (T&& t, F&& f)
        -> internal::rebind_function_t<
            std::remove_cvref_t<F>,
            internal::arg_transformer<decltype(internal::transformation(
                                          std::forward<T>(t))),
                                      std::remove_cvref_t<decltype(f.f)>>,
            std::remove_cvref_t<F>::arity> {
        return { internal::transformation(std::forward<T>(t)),
                 std::forward_like<F>(f.f) };
    });

} // namespace composer

#endif // COMPOSER_TRANSFORM_ARGS_HPP
