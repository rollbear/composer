<a href="https://www.buymeacoffee.com/bjornfahller"> <img src="https://cdn.buymeacoffee.com/buttons/v2/default-orange.png" height="50" width="210" alt="Buy me a coffee"/></a>
<a href="https://github.com/rollbear/composer/actions">
 <img src="https://github.com/rollbear/composer/actions/workflows/build.yml/badge.svg" height="50"/>
</a>
<a href="https://codecov.io/gh/rollbear/composer" > 
 <img src="https://codecov.io/gh/rollbear/composer/graph/badge.svg?token=VGGH860X9Z" height="50"/> 
 </a>

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

Some function templates in the standard library returns
[`std::ranges::dangling`](https://www.cppreference.com/w/cpp/ranges/dangling.html)
when called with an r-value range. The `composer` version of these function
templates disallow those calls because they are almost always bugs that are
difficult to find.


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

# Table of Contents

|                                                          |                                                     |                                                         |
|----------------------------------------------------------|-----------------------------------------------------|---------------------------------------------------------|
| [`arity_function`](#arity_function)                      | [**`<ranges.hpp>`**](#ranges_hpp)                   | [_Partitioning operations_](#partitioning)              |
| [`front_binding`](#front_binding)                        | [`size`](#size)                                     | [`is_partitioned`](#is_partitioned)                     |
| [`back_binding`](#back_binding)                          | [`ssize`](#ssize)                                   | [`partition`](#partition)                               |
| [`nodiscard`](#nodiscard)                                | [`distance`](#ssize)                                | [`partition_copy`](#partition_copy)                     |
| [`make_arity_function`](#make_arity_function)            | [**`<algorithm.hpp>`**](#algorithm_hpp)             | [`stable_partition`](#stable_partition)                 |
| [`ref`](#ref)                                            | [_Non-modifying sequence operations_](#non_mod_seq) | [`partition_point`](#partition_point)                   |
| [`cref`](#cref)                                          | [`all_of`](#all_of)                                 | [_Sorting operations_](#sorting)                        |
| [**`<functional.hpp>`**](#functional_hpp)                | [`any_of`](#any_of)                                 | [`is_sorted`](#is_sorted)                               |
| [`mem_fn`](#mem_fn)                                      | [`none_of`](#none_of)                               | [`is_sorted_until`](#is_sorted_until)                   |
| [`operator\|(T C::*, arity_function)`](#pipe_from_memfn) | [`for_each`](#for_each)                             | [`sort`](#sort)                                         |
| [`operator\| (V&&, T C::*p)`](#pipe_to_memfn)            | [`for_each_t`](#for_each_n)                         | [`partial_sort`](#partial_sort)                         |                         
| [`equal_to`](#equal_to)                                  | [`count`](#count)                                   | [`partial_sort_copy`](#partial_sort_copy)               |
| [`not_equal_to`](#not_equal_to)                          | [`count_if`](#count_if)                             | [`stable_sort`](#stable_sort)                           |
| [`less_than`](#less_than)                                | [`find`](#find)                                     | [`nth_element`](#nth_element)                           |
| [`less_or_equal_to`](#less_or_equal_to)                  | [`find_if`](#find_if)                               | [_Binary search operations_](#binsearch)                |
| [`greater_than`](#greater_than)                          | [`find_if_not`](#find_if_not)                       | [`lower_bound`](#lower_bound)                           |
| [`greater_or_equal_to`](#greater_or_equal_to)            | [`find_last`](#find_last)                           | [`upper_bound`](#upper_bound)                           |
| [`compare_three_way`](#compare_three_way)                | [`find_last_if`](#find_last_if)                     | [`binary_search`](#binary_search)                       |
| [`identity`](#indentity)                                 | [`find_last_if_not`](#find_last_if_not)             | [`equal_range`](#equal_range)                           |
| [`dereference`](#dereference)                            | [`find_end`](#find_end)                             | [_Set operations_](#setops)                             |
| [`plus`](#plus)                                          | [`find_first_of`](#find_first_of)                   | [`merge`](#merge)                                       |
| [`minus`](#minus)                                        | [`adjacent_find`](#adjacent_find)                   | [`inplace_merge`](#inplace_merge)                       |
| [`multiplies`](#multiplies)                              | [`search`](#search)                                 | [`includes`](#includes)                                 |
| [`divides`](#divides)                                    | [`search_n`](#search_n)                             | [`set_difference`](#set_difference)                     |
| [`modulus`](#modulus)                                    | [`contains`](#contains)                             | [`set_intersection`](#set_intersection)                 |
| [`negate`](#negate)                                      | [`contains_subrange`](#contains_subrange)           | [`set_symmetric_difference`](#set_symmetric_difference) |
| [`logical_and`](#logical_and)                            | [`starts_with`](#starts_with)                       | [`set_union`](#set_union)                               |
| [`logical_or`](#logical_or)                              | [`ends_with`](#ends_with)                           | [_Heap operations_](#heap_ops)                          |
| [`logical_not`](#logical_not)                            | [_Modifying seuence operations_](#mod_seq)          | [`is_heap`](#is_heap)                                   |
| [`bit_and`](#bit_and)                                    | [`fill`](#fill)                                     | [`is_heap_until`](#is_heap_until)                       |
| [`bit_or`](#bit_or)                                      | [`fill_n`](#fill_n)                                 | [`make_heap`](#make_heap)                               |
| [`bit_xor`](#bit_xor)                                    | [`generate`](#generate)                             | [`push_heap`](#push_heap)                               |
| [`bit_not`](#bin_not)                                    | [`generate_n`](#generate_n)                         | [`pop_heap`](#pop_heap)                                 |
| [**`<transform_args.hpp>`**](#transform_args_hpp)        | [`remove`](#remove)                                 | [`sort_heap`](#sort_heap)                               |
| [`transform_args`](#transform_args)                      | [`remove_if`](#remove_if)                           | [_Minimum/maximum operations_](#minmax_ops)             |
| [**`<tuple.hpp>`**](#tuple_hpp)                          | [`replace`](#replace)                               | [`max`](#max)                                           |
| [`get<I>`](#get)                                         | [`replace_if`](#replace_if)                         | [`max_element`](#max_element)                           |
| [`apply_to`](#apply_to)                                  | [`unique`](#unique)                                 | [`min`](#min)                                           |
| [`apply_using`](#apply_using)                            |                                                     | [`min_element`](#min_element)                           |
|                                                          |                                                     | [`minmax_element`](#minmax_element)                     |
|                                                          |                                                     | [`clamp`](#clamp)                                       |


# Building blocks

## type templates

### <A name="arity_function"></A> `composer::arity_function<N, F>`

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

### <A name="nodiscard"></A> `nodiscard<F>`

Used when defining a new function, to ensure that the return from it is marked
[`[[nodiscard]]`](https://en.cppreference.com/w/cpp/language/attributes/nodiscard).

Example:
```
auto minus = back_binding<2, nodiscard<std::minus<>>>{};
```

## helper function template objects

### <A name="make_arity_function"></A>`composer::make_arity_function<N, Kind = arity_function>(F&& f)`

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

## <A name="functional_hpp"></A> `<composer/functional.hpp>`

#### <A name="mem_fn"></A> `composer::mem_fn(T C::*)`

Creates a [`nodiscard`](#nodiscard) composable function objects that calls
[`std::mem_fn`](https://en.cppreference.com/w/cpp/utility/functional/mem_fn.html)
on its argument.

#### <A name="pipe_from_memfn"></A> `operator|(T C::*p, arity_function)`

Synonymous with [`composer::mem_fn`](#mem_fn)`(<pointer_to_member>) | function`,
i.e. returns a composable function that calls the right hand side with the
result of picking the member via the pointer-to-member.

#### <A name="pipe_to_memfn"></A> `operator|(V&& v, T C::*p)`

Synonymous with [`mem_fn(&C::p)`](#mem_fn)`(std::forward<V>(v)`. Only
applicable if the pointer to member is applicable.

#### <A name="equal_to"></A> `composer::equal_to(a, b)`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/equal_to). 

#### <A name="not_equal_to"></A> `composer::not_equal_to(a, b)`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::not_equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/not_equal_to).

#### <A name="less_than"></A> `composer::less_than(a, b)`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::less`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less).

#### <A name="less_or_equal_to"></A>`composer::less_or_equal_to(a, b)`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::less_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less_equal).

#### <A name="greater_than"></A> `composer::greater_than(a, b)`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::greater`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater).

#### <A name="greater_or_equal_to"></A> `composer::greater_or_equal_to(a, b)`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::greater_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater_equal).

#### <A name="compare_three_way"></A> `composer::compare_three_wayl(a, b)`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::compare_three_way`](https://en.cppreference.com/w/cpp/utility/compare/compare_three_way).

#### <A name="identity"></A> `composer::identity`

Composable [`nodiscard`](#nodiscard) version of [`std::identity`](https://en.cppreference.com/w/cpp/utility/functional/identity.html).

#### <A name="dereference"></A> `composer::dereference`

Composable [`nodiscard`](#nodiscard) function that returns the result of unary `operator*` on its argument.

#### <A name="plus"></A> `composer::plus`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::plus<>`](https://en.cppreference.com/w/cpp/utility/functional/plus_void.html)

#### <A name="minus"></A> `composer::minus`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::minus<>`](https://en.cppreference.com/w/cpp/utility/functional/minus_void.html)

#### <A name="multiplies"></A> `composer::multiplies`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::multiplies<>`](https://en.cppreference.com/w/cpp/utility/functional/multiplies_void.html)

#### <A name="divides"></A> `composer::divides`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::divides<>`](https://en.cppreference.com/w/cpp/utility/functional/divides_void.html)

#### <A name="modulus"></A> `composer::modulus`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::modulus<>`](https://en.cppreference.com/w/cpp/utility/functional/modulus_void.html)

#### <A name="negate"></A> `composer::negate`

Composable [`nodiscard`](#nodiscard) version of [`std::negate`](https://en.cppreference.com/w/cpp/utility/functional/negate.html)

#### <A name="logical_and"></A> `composer::logical_and`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::logical_and<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

#### <A name="logical_or"></A> `composer::logical_or`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::logical_or<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

#### <A name="logical_not"></A> `composer::logical_not`

Composable [`nodiscard`](#nodiscard) version of [`std::logical_not<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_not_void.html)

#### <A name="bit_and"></A> `composer::bit_and`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::bit_and<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_and_void.html)

#### <A name="bit_or"></A> `composer::bit_or`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::bit_or<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_or_void.html)

#### <A name="bit_xor"></A> `composer::bit_xor`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::bit_xor<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_xor_void.html)

#### <A name="bit_not"></A> `composer::bit_not`

Composable [`nodiscard`](#nodiscard) version of [`std::bit_not<>](https://en.cppreference.com/w/cpp/utility/functional/bit_not_void.html)

## <A name="transform_args_hpp"></A> `<composer/transform_args.hpp>`

#### <A name="transform_args"></A> `composer::transform_args(transformation, arity_function)`

Creates a new version of the arity function that passes all args via the
transformation function. The returned function is of the same kind as the
arity function passed in, so e.g. calling `transform_args` on a [Back binding](#back_binding)
function returns a [Back binding](#back_binding) function.

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

[`nodiscard`](#nodiscard) is transitive from the composed object to the result object.

## <A name="tuple_hpp"></A> `<composer/tuple.hpp>`

#### <A name="get"></A> `template <size_t I> composer::get`

Composable [`nodiscard`](#nodiscard) function to select the I:th member of a
tuple-like type. See [`std::get`](https://en.cppreference.com/w/cpp/utility/tuple/get)

There is unfortunately no *type* overload since `get` needs to be an object, not a function template, and object templates cannot be overloaded.

#### <A name="apply_to"></A> `composer::apply_to`

[Back binding](#back_binding) version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

#### <A name="apply_using"></A> `composer::apply_using`

Front binding version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

## <A name="ranges_hpp"></A> `<composer/ranges.hpp>`

#### <A name="size"></A> `composer::size`

Composable [`nodiscard`](#nodiscard) function object for [`std::ranges::size`](https://www.cppreference.com/w/cpp/ranges/size.html)

#### <A name="ssize"></A> `composer::ssize`

Composable [`nodiscard`](#nodiscard) function object for [`std::ranges::ssize`](https://www.cppreference.com/w/cpp/ranges/ssize.html)

#### <A name="distance"></A> `composer::distance`

Composable [`nodiscard`](#nodiscard) function object for [`std::ranges::distance`](https://www.cppreference.com/w/cpp/iterator/ranges/distance.html)

`composable::distance` is callable either with a range, or with an iterator/sentinel pair.

## <A name="algorithm_hpp></A> `<composer/algo/rithm.hpp>`

### <A name="non_mod_seq"></A> Non-modifying sequence operations

#### <A name="all_of"></A> `composer::all_of`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

#### <A name="any_of"></A> `composer::any_of`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

#### <A name="none_of"></A> `composer::none_of`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

#### <A name="for_each"></A> `composer::for_each`

[Back binding](#back_binding) version of [`std::ranges::for_each`](https://en.cppreference.com/w/cpp/algorithm/ranges/for_each.html)

`composer::for_each` cannot be called with an r-value range.

#### <A name="for_each_n"></A> `composer::for_each_n`

[Back binding](#back_binding) version of [`std::ranges::for_each_n`](https://en.cppreference.com/w/cpp/algorithm/ranges/for_each_n.html)

#### <A name="count"></A> `composer::count`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::count`](https://en.cppreference.com/w/cpp/algorithm/ranges/count.html)

#### <A name="count_if"></A> `composer::count_if`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::count_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/count.html)

#### <A name="find"></A> `composer::find`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

`composer::find` cannot be called with an r-value range.

#### <A name="find_if"></A> `composer::find_if`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

`composer::find_if` cannot be called with an r-value range.

#### <A name="find_if_not"></A> `composer::find_if_not`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find_if_not`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

`composer::find_if_not` cannot be called with an r-value range.

#### <A name="find_last"></A> `composer::find_last`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

`composer::find_last` cannot be called with r-value range.

#### <A name="find_last_if"></A> `composer::find_last_if`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

`composer::find_last_if` cannot be called with an r-value range.

#### <A name="find_last_if_not"></A> `composer::find_last_if_not`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

`composer::find_last_if_not` cannot be called with an r-value range.

#### <A name="find_end"></A> `composer::find_end`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find_end`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_end.html)

`composer.:find_end` cannot be called with an r-value range.

#### <A name="find_first_of"></A> `composer::find_first_of`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::find_first_of](https://en.cppreference.com/w/cpp/algorithm/ranges/find_first_of.html)

`composer::find_first_of` cannot be called with an r-value range.

#### <A name="adjacent_find"></A> `composer::adjacent_find`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::adjacent_find`](https://www.cppreference.com/w/cpp/algorithm/ranges/adjacent_find.html)

`composer::adjacent_find` cannot be called with an r-value range.

#### <A name="search"></A> `composer::search`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::search`](https://www.cppreference.com/w/cpp/algorithm/ranges/search.html)

`composer::search` cannot be called with an r-value range.

#### <A name="search_n"></A> `composer::search_n`

`composer::search_n` cannot be called with an r-value range.

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::search_n`](https://www.cppreference.com/w/cpp/algorithm/ranges/search_n.html)

#### <A name="contains"></A> `composer::contains`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::contains`](https://www.cppreference.com/w/cpp/algorithm/ranges/contains.html)

#### <A name="contains_subrange"></A> `composer::contains_subrange`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::contains_subrange`](https://www.cppreference.com/w/cpp/algorithm/ranges/contains.html)

#### <A name="starts_with"></A> `composer::starts_with`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::starts_with`](https://en.cppreference.com/w/cpp/algorithm/ranges/starts_with.html)

**NOTE!** Not available in gcc-15 and lower

#### <A name="ends_with"></A> `composer::ends_with`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::ends_with`](https://en.cppreference.com/w/cpp/algorithm/ranges/ends_with.html)

**NOTE!** Not available in gcc-15 and lower

### <A name="mod_seq"></A> Modifying sequence operations

#### <A name="fill"></A> `composer::fill`

[Back binding](#back_binding) version of [`std::ranges::fill`](https://en.cppreference.com/w/cpp/algorithm/ranges/fill.html)

`composer::fill` cannot be called with an r-value range.

#### <A name="fill_n"></A> `composer::fill_n`

[Back binding](#back_binding) version of [`std::ranges::fill_n`](https://en.cppreference.com/w/cpp/algorithm/ranges/fill_n.html)

#### <A name="generate"></A> `composer::generate`

[Back binding](#back_binding) version of [`std::ranges::generate`](https://en.cppreference.com/w/cpp/algorithm/ranges/generate.html)

`composer::generate` cannot be called with an r-value range.

#### <A name="generate_n"></A> `composer::generate_n`

[Back binding](#back_binding) version of [`std::ranges::generate_n`](https://en.cppreference.com/w/cpp/algorithm/ranges/generate_n.html)

#### <A name="remove"></A> `composer::remove`

[Back binding](#back_binding) version of [`std::ranges::remove`](https://en.cppreference.com/w/cpp/algorithm/ranges/remove.html)

`composer::remove` cannot be called with an r-value range.

#### <A name="remove_if></A> `composer::remove_if`

[Back binding](#back_binding) version of [`std::ranges::remove_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/remove.html)

`composer::remove_if` cannot be called with an r-value range.

#### <A name="replace"></A> `composer::replace`

[Back binding](#back_binding) version of [`std::ranges::replace`](https://en.cppreference.com/w/cpp/algorithm/ranges/replace.html)

`composer::replace` cannot be called with an r-value range.

#### <A name="replace_if"></A> `composer::replace_if`

[Back binding](#back_binding) version of [`std::ranges::replace_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/replace.html)

`composer::replace_if` cannot be called with an r-value range.

#### <A name="unique"></A> `composer::unique`

[Back binding](#back_binding) version of [`std::ranges::unique`](https://en.cppreference.com/w/cpp/algorithm/ranges/unique.html)

`composer::unique` cannot be colled with an r-value range.

### <A name="partitioning"></A> Partitioning operations

#### <A name="is_portitioned"></A> `composer::is_partitioned`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::is_partitioned`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_partitioned.html)

#### <A name="partition"></A> `composer::partition`

[Back binding](#back_binding) version of [`std::ranges::partition`](https://en.cppreference.com/w/cpp/algorithm/ranges/partition.html)

`composer::partition` cannot be called with an r-value range.

#### <A name="partition_copy"></A> `composer::partition_copy`

[Back binding](#back_binding) version of [`std::ranges::partition_copy`](https://en.cppreference.com/w/cpp/algorithm/ranges/partition_copy.html)

`composer::partition_copy` cannot be called with an r-value range.

#### <A name="stable_partition"></A> `composer::stable_partition`

[Back binding](#back_binding) version of [`std::ranges::stabpartition`](https://en.cppreference.com/w/cpp/algorithm/ranges/stable_partition.html)

`composer::stable_partition` cannod be called with an r-value range.

#### <A name="partition_point"></A> `composer::partition_point`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::partition_point`](https://en.cppreference.com/w/cpp/algorithm/ranges/partition_point)

`composer::partition_point` cannot be called with an r-value range.

### <A name="sorting"></A> Sorting operations

#### <A name="is_sorted"></A> `composer::is_sorted`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::is_sorted`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_sorted.html)

#### <A name="is_sorted_until"></A> `composer::is_sorted_until`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::is_sorted_until`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_sorted_until.html)

`composer::is_sorted_until` cannot be called with an r-value range.

#### <A name="sort"></A> `composer::sort`

[Back binding](#back_binding) version of [`std::ranges::sort`](https://en.cppreference.com/w/cpp/algorithm/ranges/sort.html)

`composer::sort` cannot be called with an r-value range.

#### <A name="partial_sort"></A> `composer::partial_sort`

[Back binding](#back_binding) version of [`std::ranges::partial_sort`](https://en.cppreference.com/w/cpp/algorithm/ranges/partial_sort.html)

`composer::partial_sort` cannot be called with an r-value range.

#### <A name="partial_sort_copy"></A> `composer::partial_sort_copy`

[Back binding](#back_binding) version of [`std::ranges::partial_sort_copy`](https://en.cppreference.com/w/cpp/algorithm/ranges/partial_sort_copy.html)

`composer::partial_sort_copy` cannot be called with r-value ranges.

_Note!_ If you bind the output range by value, the result will end up in the
copy of the range owned by the function object. If you want to bind the output
range, do so via `composer::ref`.

#### <A name="stable_sort"></A> `composer::stable_sort`

[[Back binding](#back_binding)](#back_binding) version of
[`std::ranges::stable_sort`](https://en.cppreference.com/w/cpp/algorithm/ranges/stable_sort.html)

`composer::stable_sort` cannot be called with r-value ranges.

#### <A name="nth_element"></A> `composer::nth_element`

[Back binding](#back_binding) version of
[`std::ranges::nth_elementt`](https://en.cppreference.com/w/cpp/algorithm/ranges/nth_element.html)

`composer::nth_element` cannot be called with r-value ranges.

### <A name="binsearch"></A> Binary search operations  (on sorted ranges)

#### <A name="lower_bound"></A> `composer::lower_bound`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::lower_bound`](https://en.cppreference.com/w/cpp/algorithm/ranges/lower_bound.html)

`composer::lower_bound` cannot be called with an r-value range.

#### <A name="upper_bound"></A> `composer::upper_bound`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::upper_bound`](https://en.cppreference.com/w/cpp/algorithm/ranges/upper_bound.html)

`composer::upper_bound` cannot be called with an r-value range.

#### <A name="binary_search"></A> `composer::binary_search`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::binary_search`](https://en.cppreference.com/w/cpp/algorithm/ranges/binary_search.html)

#### <A name="equal_range"></A> `composer::equal_range`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::equal_range`](https://en.cppreference.com/w/cpp/algorithm/ranges/equal_rangeh.html)

`composer::equal_range` cannot be called with an r-value range.

### <A name="setops"></A> Set operations (on sorted ranges)

#### <A name="merge"></A> `composer::merge`

[Back binding](#back_binding) version o [`std::ranges::merge`](https://en.cppreference.com/w/cpp/algorithm/ranges/merge.html)

`composer::merge` cannot be called with r-value ranges.

#### <A name="inplace_merge"></A> `composer::inplace_merge`

[Back binding](#back_binding) version of [`std::ranges::inplace_merge`](https://en.cppreference.com/w/cpp/algorithm/ranges/inplace_merge.html)

`composer::inplace_merge` cannot be called with an r-value range.

#### <A name="includes"></A> `composer::includes`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::includes`](https://en.cppreference.com/w/cpp/algorithm/ranges/includes.html)

#### <A name="set_difference"></A> `composer::set_difference`

[Back binding](#back_binding) version of [`std::ranges::set_difference`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_difference.html)

#### <A name="set_intersection"></A> `composer::set_intersection`

[Back binding](#back_binding) version of [`std::ranges::set_intersection`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_intersection.html)

#### <A name="set_symmetric_difference"></A> `composer::set_symmetric_difference`

[Back binding](#back_binding) version of [`std::ranges::set_symmetric_difference`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_symmetric_difference.html)

#### <A name="set_union"></A> `composer::set_union`

[Back binding](#back_binding) version of [`std::ranges::set_union`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_union.html)

### <A name="heap_ops"></A> Heap operations

#### <A name="is_heap"></A> `composer::is_heap`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::is_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_heap.html)

#### <A name="is_heap_until"></A> `composer::is_heap_until`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::is_heap_until`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_heap_until.html)

`composer::is_heap_until` cannot be called with an r-value range.

#### <A name="make_heap"></A> `composer::make_heap`

[Back binding](#back_binding) version of [`std::ranges::make_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/make_heap.html)

`composer::make_heap` cannot be called with an r-value range.

#### <A name="push_heap"></A> `composer::push_heap`

[Back binding](#back_binding) version of [`std::ranges::push_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/push_heap.html)

`composer::push_heap` cannot be called with an r-value range.

#### <A name="pop_heap"></A> `composer::pop_heap`

[Back binding](#back_binding) version of [`std::ranges::pop_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/pop_heap.html)

`composer::pop_heap` cannot be called with an r-value range.

#### <A name="sort_heap"></A> `composer::sort_heap`

[Back binding](#back_binding) version of [`std::ranges::sort_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/sort_heap.html)

`composer::sort_heap` cannot be called with an r-value range.

### <A name="minmax_ops"></A> Minimum/maximum operations

#### <A name="max"></A> `composer::max`
[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::max`](https://en.cppreference.com/w/cpp/algorithm/ranges/max.html)

#### <A name="max_element"></A> `composer::max_element`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::max_element`](https://en.cppreference.com/w/cpp/algorithm/ranges/max_element.html)

`composer::max_element` cannot be called with an r-value range.

#### <A name="min"></A> `composer::min`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::min`](https://en.cppreference.com/w/cpp/algorithm/ranges/min.html)

#### <A name="min_element"></A> `composer::min_element`

`composer::minmax_element` cannot be called with an r-value range.

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::min_element`](https://en.cppreference.com/w/cpp/algorithm/ranges/min_element.html)

#### <A name="minmax_element"></A> `composer::minmax_element`

`composer::minmax_element` cannot be called with an r-value range.

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::minmax_element`](https://en.cppreference.com/w/cpp/algorithm/ranges/minmax_element.html)

#### <A name="clamp"></A> `composer::clamp`

[Back binding](#back_binding) [`nodiscard`](#nodiscard) version of [`std::ranges::clamp`](https://en.cppreference.com/w/cpp/algorithm/ranges/clamp.html)

