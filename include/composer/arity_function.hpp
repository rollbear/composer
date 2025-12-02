#ifndef COMPOSER_ARITY_FUNCTION_HPP
#define COMPOSER_ARITY_FUNCTION_HPP

#include <type_traits>
#include <utility>

namespace composer {

template <typename T>
concept nodiscard_function = requires { T::is_nodiscard; } && T::is_nodiscard;

template <typename T>
struct nodiscard : T {
    static constexpr bool is_nodiscard = true;
};

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
    static constexpr bool is_nodiscard = nodiscard_function<RH>;

    template <typename Self, typename... Ts>
    [[nodiscard]] constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> decltype(std::forward_like<Self>(self.rh)(
            std::forward_like<Self>(self.lh)(std::forward<Ts>(ts)...)))
    {
        return std::forward_like<Self>(self.rh)(
            std::forward_like<Self>(self.lh)(std::forward<Ts>(ts)...));
    }
};

template <typename, std::size_t, typename>
struct rebind_function;

template <template <std::size_t, typename> class C,
          std::size_t N,
          typename F,
          std::size_t NN,
          typename FF>
struct rebind_function<C<N, F>, NN, FF> {
    using type = C<NN, FF>;
};

template <typename C, std::size_t N, typename F>
using rebind_function_t = typename rebind_function<C, N, F>::type;
} // namespace internal

template <std::size_t N, typename F>
struct [[nodiscard]] arity_function {
    static constexpr auto arity = N;
    static constexpr bool is_nodiscard = nodiscard_function<F>;
    [[no_unique_address]] F f;

    template <typename Self, typename... Ts>
    [[nodiscard]]
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> decltype(std::forward_like<Self>(self.f)(std::forward<Ts>(ts)...))
        requires nodiscard_function<F>
    {
        return std::forward_like<Self>(self.f)(std::forward<Ts>(ts)...);
    }

    template <typename Self, typename... Ts>
    constexpr auto operator()(this Self&& self, Ts&&... ts)
        -> decltype(std::forward_like<Self>(self.f)(std::forward<Ts>(ts)...))
    {
        return std::forward_like<Self>(self.f)(std::forward<Ts>(ts)...);
    }

    template <typename Self, typename RH>
    [[nodiscard]] constexpr auto operator|(this Self&& self, RH&& rh)
        -> internal::rebind_function_t<
            std::remove_cvref_t<Self>,
            N,
            internal::composition<F, std::remove_cvref_t<RH>>>
    {
        return { { std::forward_like<Self>(self.f), std::forward<RH>(rh) } };
    }
};

template <std::size_t N,
          template <std::size_t, typename> class AF = arity_function>
inline constexpr auto make_arity_function
    = []<typename F> [[nodiscard]] (F&& f) -> AF<N, std::remove_cvref_t<F>> {
    return { std::forward<F>(f) };
};

template <typename IN, arity_function_type F>
[[nodiscard]] constexpr auto operator|(IN&& in, F&& f)
    -> decltype(std::forward<F>(f)(std::forward<IN>(in)))
    requires(!arity_function_v<std::remove_cvref_t<IN>>)
{
    return std::forward<F>(f)(std::forward<IN>(in));
}

} // namespace composer

#endif // COMPOSER_ARITY_FUNCTION_HPP
