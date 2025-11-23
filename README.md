<a href="https://www.buymeacoffee.com/bjornfahller"> <img src="https://cdn.buymeacoffee.com/buttons/v2/default-orange.png" height="50" width="210" alt="Buy me a coffee"/></a>


**composer** is an experimental C++23 library for functional composition.

There are basic building blocks for creating function objects and support
[partial application](https://en.wikipedia.org/wiki/Partial_application) of
function arguments, and a (very incomplete) set of composable versions of
functions from the standard library

Partial application comes in two flavors [`front_binding`](#front_binding),
which binds the left most arguments to the function, and
[`back_binding`](#back_binding), which binds the right most arguments to
the function.

In this library, the function object that results from a partial application
always owns the bound arguments, either by copying or by moving. This even
goes for C-arrays, which otherwise always decay to pointers to the first
element. If you want a bound argument to be referenced, you can use
[`ref`](#ref) or [`cref`](#cref).


Teaser example:

```c++
int main()
{
    struct numname {
        int num;
        std::string_view name;
    };
    static constexpr numname values[]{
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}
    };
    constexpr auto by_name = composer::transform_args(&numname::name);
    constexpr auto by_num = composer::transform_args(&numname::num);
    assert(! composer::is_sorted(values, by_name(composer::less_than)));
    assert(composer::is_sorted(values, by_num(composer::less_than)));
    
    constexpr auto it = values | composer::find_if(&numname::name | composer::equal_to("three"));
    return it->num; // returns 3
}
```

# Building blocks

## type templates

### `composer::arity_function<N, F>`

A composable version of the function type `F`. `N` is the arity of the
function, i.e. the number of arguments it takes, including defaulted arguments, if there
are any.

Arity functions can be called with whatever arguments that the function `F` is callable
with. Arity functions aqre composable using *pipes* (`|`, see the teaser example above),
where the function on the right hand side is callable with the result of the left hand
side function.

Arity functions that accept only one argument can also be called using a *pipe* syntax,
like `value | function` (which is synonymous with `function(value)`)

### <A name="front_binding"></A> `composer::front_binding<N, F>`

A `front_binding` is an `arity_function`. If called with fewer arguments than
the arity, a new callable is returned, which binds the arguments to the left.
When that callable is invoked with args, the function is called with the bound
arguments first, and the remaining arguments in the back.

Example:
```c++
auto add = [](auto a, auto b) -> decltype(a + b){ return a + b; };
auto plus = composer::front_binding<decltype(subtract), 2>{};
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

### <A name="back_binding"></A> `composer::back_binding<N, F>`

A `back_binding` is an `arity_function`. If called with fewer arguments than
needed for the underlying function, it binds the arguments to the right. This
comes natural for many function, e.g. subtraction, where the order better
mirrors human language.

Example:
```c++
constexpr auto sub = [](auto a, auto b) -> decltype(a - b) { return a - b;};
auto minus = composer::back_binding<decltype(sub), 2>{};
auto minus2 = minus(2); / binds 2 as b for sub()
auto x = minus2(5); // calls sub(5,2)
```

All bound
[lvalues](https://en.cppreference.com/w/cpp/language/value_category.html)
are copied, and bound rvalue are moved. If you want to bind an argument by
reference, use [`composer::ref()`](#ref) or [`composer::cref()`](#cref) (below).

Any bound arguments are forwarded to the function using the same qualifiers
as the function object. If you have bound a move-only type like
[`std::unique_ptr<>`](https://en.cppreference.com/w/cpp/memory/unique_ptr.html)
and want to forward it to a function that accepts its argument by value, you
call `std::move(funcion_object)(args...)`.

## helper function template objects

### `composer::make_arity_function<N, Kind = arity_function>(F&& f)`

Make it easier to create arity functions from lambdas or other callables.

Example:
```c++
auto minus = make_arity_function<2, back_binding>([](auto a, auto b)-> decltype(a - b){ return a - b;});
```

### <A name="ref"></A> `composer::ref(T&)`

Used when binding arguments to partially binding functions, and you want to
bind them by reference.

### <A name="cref"></A> `composer::cref(T&)`

Used when binding arguments to partially binding functions, and you want to
bind them by const reference.

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

Synonymous with `mem_fn(&C::p)(std::forward<V>(v)`. Only applicable if the
pointer to member is applicable.

### `composer::equal_to(a, b)`

Back binding version of [`std::ranges::equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/equal_to). 

### `composer::not_equal_to(a, b)`

Back binding version of [`std::ranges::not_equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/not_equal_to).

### `composer::less_than(a, b)`

Back binding version of [`std::ranges::less`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less).

### `composer::less_or_equal_to(a, b)`

Back binding version of [`std::ranges::less_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less_equal).

### `composer::greater_than(a, b)`

Back binding version of [`std::ranges::greater`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater).

### `composer::greater_or_equal_to(a, b)`

Back binding version of [`std::ranges::greater_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater_equal).

### `composer::compare_three_wayl(a, b)`

Back binding version of [`std::compare_three_way`](https://en.cppreference.com/w/cpp/utility/compare/compare_three_way).

### `composer::identity`

Composable version of [`std::identity`](https://en.cppreference.com/w/cpp/utility/functional/identity.html).

### `composer::dereference`

Composable function that returns the result of unary `operator*` on its argument.

### `composer::plus`

Back binding version of [`std::plus<>`](https://en.cppreference.com/w/cpp/utility/functional/plus_void.html)

### `composer::minus`

Back binding version of [`std::minus<>`](https://en.cppreference.com/w/cpp/utility/functional/minus_void.html)

### `composer::multiplies`

Back binding version of [`std::multiplies<>`](https://en.cppreference.com/w/cpp/utility/functional/multiplies_void.html)

### `composer::divides`

Back binding version of [`std::divides<>`](https://en.cppreference.com/w/cpp/utility/functional/divides_void.html)

### `composer::modulus`

Back binding version of [`std::modulus<>`](https://en.cppreference.com/w/cpp/utility/functional/modulus_void.html)

### `composer::negate`

Composable version of [`std::negate`](https://en.cppreference.com/w/cpp/utility/functional/negate.html)

### `composer::logical_and`

Back binding version of [`std::logical_and<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

### `composer::logical_or`

Back binding version of [`std::logical_or<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

### `composer::logical_not`

Composable version of [`std::logical_not<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_not_void.html)

### `composer::bit_and`

Back binding version of [`std::bit_and<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_and_void.html)

### `composer::bit_or`

Back binding version of [`std::bit_or<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_or_void.html)

### `composer::bit_xor`

Back binding version of [`std::bit_xor<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_xor_void.html)

### `composer::bit_not`

Composable version of [`std::bit_not<>](https://en.cppreference.com/w/cpp/utility/functional/bit_not_void.html)

## `composer/transform_args.hpp>`

### `composer::transform_args(transformation, arity_function)`

Creates a new version of the arity function that passes all args via the
transformation function. The returned function is of the same kind as the
arity function passed in, so e.g. calling transform_args on a Back binding
function returns a Back binding function.

`transform_args` is [front binding](#front_binding)

The `transformation` can be either a unary function or a
[pointer-to-member](https://isocpp.org/wiki/faq/pointers-to-members).

Example:
```c++
inline constexpr auto less_ptr = composer::transform_args(composer::dereference,
                                                          composer::less_than);
std::array<int*, N> values = ...
std::ranges::sort(values, less_ptr);
```



## `<composer/tuple.hpp>`

### `template <size_t I> composer::get`

Composable function to select the I:th member of a tuple-like type. See [`std::get`](https://en.cppreference.com/w/cpp/utility/tuple/get)

There is unfortunately no *type* overload since `get` needs to be an object, not a function template, and object templates cannot be overloaded.

### `composer::apply_to`

Back binding version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

### `composer::apply_using`

Front binding version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

## `<composer/ranges.hpp>`

### `composer::size`

Composable function object for [`std::ranges::size`](https://www.cppreference.com/w/cpp/ranges/size.html)

### `composer::ssize`

Composable function object for [`std::ranges::ssize`](https://www.cppreference.com/w/cpp/ranges/ssize.html)

### `composer::distance`

Composable function object for [`std::ranges::distance`](https://www.cppreference.com/w/cpp/iterator/ranges/distance.html)

`composable::distance` is callable either with a range, or with an iterator/sentinel pair.

## `<composer/algorithm.hpp>`

### `composer::all_of`

Back binding version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

### `composer::any_of`

Back binding version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

### `composer::none_of`

Back binding version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

### `composer::for_each`

Back binding version of [`std::ranges::for_each`](https://en.cppreference.com/w/cpp/algorithm/ranges/for_each.html)

### `composer::for_each_n`

Back binding version of [`std::ranges::for_each_n`](https://en.cppreference.com/w/cpp/algorithm/ranges/for_each_n.html)

### `composer::count`

Back binding version of [`std::ranges::count`](https://en.cppreference.com/w/cpp/algorithm/ranges/count.html)

### `composer::count_if`

Back binding version of [`std::ranges::count_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/count.html)

### `composer::find`

Back binding version of [`std::ranges::find`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

### `composer::find_if`

Back binding version of [`std::ranges::find_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

### `composer::find_if_not`

Back binding version of [`std::ranges::find_if_not`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

### `composer::find_last`

Back binding version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

### `composer::find_last_if`

Back binding version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

### `composer::find_last_if_not`

Back binding version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

### `composer::find_end`

Back binding version of [`std::ranges::find_end`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_end.html)

### `composer::find_first_of`

Back binding version of [`std::ranges::find_first_of](https://en.cppreference.com/w/cpp/algorithm/ranges/find_first_of.html)

### `composer::adjacent_find`

Back binding version of [`std::ranges::adjacent_find`](https://www.cppreference.com/w/cpp/algorithm/ranges/adjacent_find.html)

### `composer::search`

Back binding version of [`std::ranges::search`](https://www.cppreference.com/w/cpp/algorithm/ranges/search.html)

### `composer::search_n`

Back binding version of [`std::ranges::search_n`](https://www.cppreference.com/w/cpp/algorithm/ranges/search_n.html)

### `composer::contains`

Back binding version of [`std::ranges::contains`](https://www.cppreference.com/w/cpp/algorithm/ranges/contains.html)

### `composer::contains_subrange`

Back binding version of [`std::ranges::contains_subrange`](https://www.cppreference.com/w/cpp/algorithm/ranges/contains.html)

### `composer::starts_with`

Back binding version of [`std::ranges::starts_with`](https://en.cppreference.com/w/cpp/algorithm/ranges/starts_with.html)

**NOTE!** Not available in gcc-15 and lower

### `composer::ends_with`

Back binding version of [`std::ranges::ends_with`](https://en.cppreference.com/w/cpp/algorithm/ranges/ends_with.html)

**NOTE!** Not available in gcc-15 and lower

### `composer::is_partitioned`

Back binding version of [`std::ranges::is_partitioned`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_partitioned.html)

### `composer::partition_point`

Back binding version of [`std::ranges::partition_point`](https://en.cppreference.com/w/cpp/algorithm/ranges/partition_point)

### `composer::is_sorted`

Back binding version of [`std::ranges::is_sorted`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_sorted.html)

### `composer::is_sorted_until`

Back binding version of [`std::ranges::is_sorted_until`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_sorted_until.html)

### `composer::lower_bound`

Back binding version of [`std::ranges::lower_bound`](https://en.cppreference.com/w/cpp/algorithm/ranges/lower_bound.html)

### `composer::upper_bound`

Back binding version of [`std::ranges::upper_bound`](https://en.cppreference.com/w/cpp/algorithm/ranges/upper_bound.html)

### `composer::binary_search`

Back binding version of [`std::ranges::binary_search`](https://en.cppreference.com/w/cpp/algorithm/ranges/binary_search.html)

### `composer::equal_range`

Back binding version of [`std::ranges::equal_range`](https://en.cppreference.com/w/cpp/algorithm/ranges/equal_rangeh.html)

### `composer::includes`

Back binding version of [`std::ranges::includes`](https://en.cppreference.com/w/cpp/algorithm/ranges/includes.html)

### `composer::is_heap`

Back binding version of [`std::ranges::is_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_heap.html)

### `composer::is_heap_until`

Back binding version of [`std::ranges::is_heap_until`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_heap_until.html)
