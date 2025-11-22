#ifndef COMPOSER_ARITY_FUNCTION_HPP
#define COMPOSER_ARITY_FUNCTION_HPP

#include <type_traits>
#include <utility>

namespace composer {
template <typename>
inline constexpr bool arity_function_v = false;

template <typename T>
    requires requires { T::arity * 1; }
inline constexpr bool arity_function_v<T> = true;
template <typename T>
concept arity_function_type = arity_function_v<std::remove_cvref_t<T>>;

template <typename T>
struct reference_wrapper {
    T& ref;
};

template <typename T>
[[nodiscard]] constexpr reference_wrapper<T> ref(T& t)
{
    return { t };
}

template <typename T>
[[nodiscard]] constexpr reference_wrapper<const T> cref(T& t)
{
    return { t };
}

template <typename>
inline constexpr bool is_reference_wrapper = false;

template <typename T>
inline constexpr bool is_reference_wrapper<reference_wrapper<T>> = true;

template <typename T>
[[nodiscard]] constexpr auto& unwrap(T&& t)
    requires is_reference_wrapper<std::remove_cvref_t<T>>
{
    return t.ref;
}

template <typename T>
[[nodiscard]] constexpr T unwrap(T&& t)
{
    return std::forward<T>(t);
}

namespace internal {
template <typename T, std::size_t N>
struct buffer {
    constexpr buffer(T (&p)[N])
    {
        for (std::size_t i = 0; i != N; ++i) {
            data_[i] = p[i];
        }
    }

    std::remove_const_t<T> data_[N];
};

template <typename T>
struct arg_binder {
    using type = std::remove_reference_t<T>;
};

template <typename T, std::size_t N>
struct arg_binder<T (&)[N]> {
    using type = buffer<T, N>;
};

template <typename T>
using arg_binder_t = typename arg_binder<T>::type;

} // namespace internal

template <typename T, std::size_t N>
constexpr auto unwrap(internal::buffer<T, N>& t) -> T (&)[N]
{
    return t.data_;
}

template <typename T, std::size_t N>
constexpr auto unwrap(const internal::buffer<T, N>& t) -> const T (&)[N]
{
    return t.data_;
}

template <typename T, std::size_t N>
constexpr auto unwrap(const internal::buffer<T, N>&& t) -> const T (&)[N]
    = delete;

namespace internal {
template <typename LH, typename RH>
struct composition {
    LH lh;
    RH rh;

    template <typename Self, typename... Ts>
    [[nodiscard]] constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> decltype(std::forward_like<Self>(self.rh)(
            std::forward_like<Self>(self.lh)(std::forward<Ts>(ts)...)))
    {
        return std::forward_like<Self>(self.rh)(
            std::forward_like<Self>(self.lh)(std::forward<Ts>(ts)...));
    }
};

template <typename, typename, std::size_t>
struct rebind_function;

template <template <typename, std::size_t> class C,
          typename F,
          std::size_t N,
          typename FF,
          std::size_t NN>
struct rebind_function<C<F, N>, FF, NN> {
    using type = C<FF, NN>;
};

template <typename C, typename F, std::size_t N>
using rebind_function_t = typename rebind_function<C, F, N>::type;
} // namespace internal

template <typename F, std::size_t N>
struct [[nodiscard]] arity_function {
    static constexpr auto arity = N;
    [[no_unique_address]] F f;

    template <typename Self, typename... Ts>
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> decltype(std::forward_like<Self>(self.f)(
            unwrap(std::forward<Ts>(ts))...))
    {
        return std::forward_like<Self>(self.f)(unwrap(std::forward<Ts>(ts))...);
    }

    template <typename Self, arity_function_type RH>
    [[nodiscard]] constexpr auto operator|(this Self&& self, RH&& rh)
        -> internal::rebind_function_t<
            std::remove_cvref_t<Self>,
            internal::composition<F, std::remove_cvref_t<RH>>,
            N>
    {
        return { { std::forward_like<Self>(self.f), std::forward<RH>(rh) } };
    }
};

template <std::size_t N,
          template <typename, std::size_t> class AF = arity_function>
inline constexpr auto make_arity_function
    = []<typename F> [[nodiscard]] (F&& f) -> AF<std::remove_cvref_t<F>, N> {
    return { std::forward<F>(f) };
};

template <typename IN, arity_function_type F>
[[nodiscard]] inline constexpr auto operator|(IN&& in, F&& f)
    -> decltype(std::forward<F>(f)(std::forward<IN>(in)))
    requires(!arity_function_v<std::remove_cvref_t<IN>>)
{
    return std::forward<F>(f)(std::forward<IN>(in));
}

} // namespace composer

#endif // COMPOSER_ARITY_FUNCTION_HPP
