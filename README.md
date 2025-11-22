<a href="https://www.buymeacoffee.com/bjornfahller"> <img src="https://cdn.buymeacoffee.com/buttons/v2/default-orange.png" height="50" width="210" alt="Buy me a coffee"/></a>


**composer** is an experimental C++23 library for functional composition.

There are basic building blocks for creating function objects and support
[partial application](https://en.wikipedia.org/wiki/Partial_application) of
function arguments, and a set of combosable functions.

Partial application comes in two flavors [`left_curry`](#left_curry),
which binds the left most arguments to the function, and
[`right_curry`](#right curry), which binds the right most arcuments to
the function.

In this library, the function object that results from a partial application
always owns the bound arguments, either by copying or by moving. This even
goes for C-arrays, which otherwise always decay to ponters to the first
element. If you want a bound argument to be referenced, you can use
[`ref`](#ref) or [`cref`](#cref).


Teaser example:

```c++
int main()
{
    static constexpr int numbers[]{1,2,3,4};
    static constexpr std::string_view names[]{
        "one", "two", "three", "four"
    };
    constexpr auto it = std::ranges::find_if(std::views::zip(numbers, names),
                                             get<1> | equal_to("three"));
    return it | deref | get<0>; // returns 3
}
```

# Building blocks

## type templates

### `composer::arity_function<F, N>`

A composable version of the function type `F`. `N` is the arity of the
function, i.e. the number of arguments it takes, including defaulted arguments, if there
are any.

Arity functions can be called with whatever arguments that the function `F` is callable
with. Arity functions are composable using *pipes* (`|`, see the teaser example above),
where the function on the right hand side is callable with the result of the left hand
side function.

Arity functions that accept only one argument can also be called using a *pipe* syntax,
like `value | function` (which is synanymous with `function(value)`)

### <A name="left_curry"/> `composer::left_curry<F, N>`

A `left_ccurry` is an `arity_function`, modeling a normal
[curried function](https://en.wikipedia.org/wiki/Currying).
If called with fewer arguments than
the arity, a new callable is returned, which binds the arguments to the left.
When that callable is invoked with args, the function is called with the bound
arguments first, and the remaining arguments in the back.

Example:
```c++
auto add = [](auto a, auto b) -> decltype(a + b){ return a + b; };
auto plus = composer::left_arity<decltype(subtract), 2>{};
auto plus2 = plus(2); // binds 2 as a for add()
auto x = plus2(5); // calls add(2, 5)
```

All bound
[lvalues](https://en.cppreference.com/w/cpp/language/value_category.html)
are copied, and bound rvalue are moved. If you want to bind an argument by
reference, use `composer::ref()` or `composer::crev()` (below).

Any bound arguments are forwarded to the function using the same qualifiers
as the function object. If you have bound a move-only type like
[`std::unique_ptr<>`](https://en.cppreference.com/w/cpp/memory/unique_ptr.html)
and want to forward it to a function that accepts its argument by value, you
call `std::move(funcion_object)(args...)`.

### <A name="right_curry"/> `composer::right_curry<F, N>`

A `right_curry` is an `arity_function` which models currying from the
back, i.e. when called with fewer arguments than needed for the underlying
function, it binds the arguments to the right. This comes natural for many
function, e.g. subtraction, where the order better mirrors human language.

Example:
```c++
constexpr auto sub = [](auto a, auto b) -> decltype(a - b) { return a - b;};
auto minus = composer::right_curry<decltype(sub), 2>{};
auto minus2 = minus(2); / binds 2 as b for sub()
auto x = minus2(5); // calls sub(5,2)
```

All bound
[lvalues](https://en.cppreference.com/w/cpp/language/value_category.html)
are copied, and bound rvalue are moved. If you want to bind an argument by
reference, use `composer::ref()` or `composer::crev()` (below).

Any bound arguments are forwarded to the function using the same qualifiers
as the function object. If you have bound a move-only type like
[`std::unique_ptr<>`](https://en.cppreference.com/w/cpp/memory/unique_ptr.html)
and want to forward it to a function that accepts its argument by value, you
call `std::move(funcion_object)(args...)`.

## helper function template objects

### `composer::make_arity_function<N, Kind = arity_function>(F&& f)`

Make it easier to create arity functions from lambdas or other callables.

Exmaple:
```c++
auto minus = make_arity_function<2, right_curry>([](auto a, auto b)-> decltype(a - b){ return a - b;});
```

### <A name="ref"/> `composer::ref(T&)`

Used when binding arguments to curried functions, and you want to bend them by
reference.

### <A name="cref"/> `composer::cref(T&)`

Used when binding arguments to curried functions, and you want to bend them by
const reference.

# Predefined function objects

## `<composer/functional.hpp>`

### `composer::mem_fn(T C::*)`

Creates a composable function objects that calls
[`std::mem_fn`](https://en.cppreference.com/w/cpp/utility/functional/mem_fn.html)
on its argument.

### `operator|(T C::*p, arity_function)`

Synonymous with `composer::mem_fn(<pointer_to_member>) | function`, i.e.
returns a composable function that calls the right hand side with the result of
picking the member via the pointer-to-member.

### `operator|(V&& v, T C::*p)`

Synonymous with `mem_fn(&C::p)(std::forward<V>(v)`. Only appliccable if the
pointer to member is appliccable.

### `composer::equal_to(a, b)`

Right curried version of [`std::ranges::equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/equal_to). 

### `composer::not_equal_to(a, b)`

Right curried version of [`std::ranges::not_equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/not_equal_to).

### `composer::less(a, b)`

Right curried version of [`std::ranges::less`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less).

### `composer::less_equal(a, b)`

Right curried version of [`std::ranges::less_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less_equal).

### `composer::greater(a, b)`

Right curried version of [`std::ranges::greater`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater).

### `composer::greater_equal(a, b)`

Right curried version of [`std::ranges::greater_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater_equal).

### `composer::compare_three_wayl(a, b)`

Right curried version of [`std::compare_three_way`](https://en.cppreference.com/w/cpp/utility/compare/compare_three_way).

### `composer::identity`

Composable version of [`std::identity`](https://en.cppreference.com/w/cpp/utility/functional/identity.html).

### `composer::dereference`

Composable function that returns the result of unary `operator*` on its argument.

### `composer::plus`

Right curried version of [`std::plus<>`](https://en.cppreference.com/w/cpp/utility/functional/plus_void.html)

### `composer::minus`

Right curried version of [`std::minus<>`](https://en.cppreference.com/w/cpp/utility/functional/minus_void.html)

### `composer::multiplies`

Right curried version of [`std::multiplies<>`](https://en.cppreference.com/w/cpp/utility/functional/multiplies_void.html)

### `composer::divides`

Right curried version of [`std::divides<>`](https://en.cppreference.com/w/cpp/utility/functional/divides_void.html)

### `composer::modulus`

Right curried version of [`std::modulus<>`](https://en.cppreference.com/w/cpp/utility/functional/modulus_void.html)

### `composer::negate`

Composable version of [`std::negate`](https://en.cppreference.com/w/cpp/utility/functional/negate.html)

### `composer::logical_and`

Right curried version of [`std::logical_and<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

### `composer::logical_or`

Right curried version of [`std::logical_or<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

### `composer::logical_not`

Composable version of [`std::logical_not<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_not_void.html)

### `composer::bit_and`

Right curried version of [`std::bit_and<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_and_void.html)

### `composer::bit_or`

Right curried version of [`std::bit_or<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_or_void.html)

### `composer::bit_xor`

Right curried version of [`std::bit_xor<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_xor_void.html)

### `composer::bit_not`

Composable version of [`std::bit_not<>](https://en.cppreference.com/w/cpp/utility/functional/bit_not_void.html)

## `composer/transform_args.hpp>`

### `composer::transform_args(transformation, arity_function)`

Creates a new version of the arity function that passes all args via the
transformation function. The returned function is of the same kind as the
arity function passed in, so e.g. calling transform_args on a right curried
function returns a right curried function.

Example:
```c++
inline constexpr auto less_ptr = composer::transform_args(composer::dereference,
                                                          composer::less);
std::array<int*, N> values = ...
std::ranges::sort(values, less_ptr);
```



## `<composer/tuple.hpp>`

### `template <size_t I> composer::get`

Composable function to select the I:th member of a tuple-like type. See [`std::get`](https://en.cppreference.com/w/cpp/utility/tuple/get)

There is unfortunately no *type* overload since `get` needs to be an object, not a function template, and object templates cannot be overloaded.

### `composer::apply_to`

Right curried version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

### `composer::apply_using`

Lelt curried version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

## `<composer/ranges.hpp>`

### `composer::size`

Composable function object for [`std::ranges::size`]()

### `composer::ssize`

Composable function object for [`std::ranges::ssize`]()

### `composer::distance`

Composable function object for [`std::ranges::distance`]()

`composable::distance` is callable either with a range, or with an iterator/sentinel pair.
