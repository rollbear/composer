#ifndef COMPOSER_TRANSFORM_ARGS_HPP
#define COMPOSER_TRANSFORM_ARGS_HPP

#include "composable_function.hpp"
#include "front_binding.hpp"
#include "functional.hpp"

namespace composer {

namespace internal {
template <typename T, typename F>
struct arg_transformer {
    [[no_unique_address]] T t;
    [[no_unique_address]] F f;
    static constexpr bool is_nodiscard = nodiscard_function<F>;

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
constexpr auto transformation(T&& t)
{
    if constexpr (std::is_member_pointer_v<std::remove_cvref_t<T>>) {
        return composer::mem_fn(std::forward<T>(t));
    } else {
        return t;
    }
}

} // namespace internal

inline constexpr auto transform_args
    = make_composable_function<2, front_binding>(
        []<typename T, composable_function_type F> [[nodiscard]] (T&& t, F&& f)
            -> internal::rebind_function_t<
                std::remove_cvref_t<F>,
                std::remove_cvref_t<F>::arity,
                internal::arg_transformer<decltype(internal::transformation(
                                              std::forward<T>(t))),
                                          std::remove_cvref_t<decltype(f.f)>>> {
            return { internal::transformation(std::forward<T>(t)),
                     std::forward_like<F>(f.f) };
        });

} // namespace composer

#endif // COMPOSER_TRANSFORM_ARGS_HPP
