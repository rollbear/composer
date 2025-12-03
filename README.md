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

### <A name="nodiscard"></A> `nodiscard<F>`

Used when defining a new function, to ensure that the return from it is marked
[`[[nodiscard]]`](https://en.cppreference.com/w/cpp/language/attributes/nodiscard).

Example:
```
auto minus = back_binding<2, nodiscard<std::minus<>>>{};
```

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

#### `composer::mem_fn(T C::*)`

Creates a [`nodiscard`](#nodiscard) composable function objects that calls
[`std::mem_fn`](https://en.cppreference.com/w/cpp/utility/functional/mem_fn.html)
on its argument.

#### `operator|(T C::*p, arity_function)`

Synonymous with `composer::mem_fn(<pointer_to_member>) | function`, i.e.
returns a composable function that calls the right hand side with the result of
picking the member via the pointer-to-member.

#### `operator|(V&& v, T C::*p)`

Synonymous with `mem_fn(&C::p)(std::forward<V>(v)`. Only applicable if the
pointer to member is applicable.

#### `composer::equal_to(a, b)`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/equal_to). 

#### `composer::not_equal_to(a, b)`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::not_equal_to`](https://en.cppreference.com/w/cpp/utility/functional/ranges/not_equal_to).

#### `composer::less_than(a, b)`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::less`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less).

#### `composer::less_or_equal_to(a, b)`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::less_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/less_equal).

#### `composer::greater_than(a, b)`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::greater`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater).

#### `composer::greater_or_equal_to(a, b)`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::greater_equal`](https://en.cppreference.com/w/cpp/utility/functional/ranges/greater_equal).

#### `composer::compare_three_wayl(a, b)`

Back binding [`nodiscard`](#nodiscard) version of [`std::compare_three_way`](https://en.cppreference.com/w/cpp/utility/compare/compare_three_way).

#### `composer::identity`

Composable [`nodiscard`](#nodiscard) version of [`std::identity`](https://en.cppreference.com/w/cpp/utility/functional/identity.html).

#### `composer::dereference`

Composable [`nodiscard`](#nodiscard) function that returns the result of unary `operator*` on its argument.

#### `composer::plus`

Back binding [`nodiscard`](#nodiscard) version of [`std::plus<>`](https://en.cppreference.com/w/cpp/utility/functional/plus_void.html)

#### `composer::minus`

Back binding [`nodiscard`](#nodiscard) version of [`std::minus<>`](https://en.cppreference.com/w/cpp/utility/functional/minus_void.html)

#### `composer::multiplies`

Back binding [`nodiscard`](#nodiscard) version of [`std::multiplies<>`](https://en.cppreference.com/w/cpp/utility/functional/multiplies_void.html)

#### `composer::divides`

Back binding [`nodiscard`](#nodiscard) version of [`std::divides<>`](https://en.cppreference.com/w/cpp/utility/functional/divides_void.html)

#### `composer::modulus`

Back binding [`nodiscard`](#nodiscard) version of [`std::modulus<>`](https://en.cppreference.com/w/cpp/utility/functional/modulus_void.html)

#### `composer::negate`

Composable [`nodiscard`](#nodiscard) version of [`std::negate`](https://en.cppreference.com/w/cpp/utility/functional/negate.html)

#### `composer::logical_and`

Back binding [`nodiscard`](#nodiscard) version of [`std::logical_and<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

#### `composer::logical_or`

Back binding [`nodiscard`](#nodiscard) version of [`std::logical_or<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_and_void.html)

#### `composer::logical_not`

Composable [`nodiscard`](#nodiscard) version of [`std::logical_not<>`](https://en.cppreference.com/w/cpp/utility/functional/logical_not_void.html)

#### `composer::bit_and`

Back binding [`nodiscard`](#nodiscard) version of [`std::bit_and<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_and_void.html)

#### `composer::bit_or`

Back binding [`nodiscard`](#nodiscard) version of [`std::bit_or<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_or_void.html)

#### `composer::bit_xor`

Back binding [`nodiscard`](#nodiscard) version of [`std::bit_xor<>`](https://en.cppreference.com/w/cpp/utility/functional/bit_xor_void.html)

#### `composer::bit_not`

Composable [`nodiscard`](#nodiscard) version of [`std::bit_not<>](https://en.cppreference.com/w/cpp/utility/functional/bit_not_void.html)

## `composer/transform_args.hpp>`

#### `composer::transform_args(transformation, arity_function)`

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

[`nodiscard`](#nodiscard) is transitive from the composed object to the result object.

## `<composer/tuple.hpp>`

#### `template <size_t I> composer::get`

Composable [`nodiscard`](#nodiscard) function to select the I:th member of a
tuple-like type. See [`std::get`](https://en.cppreference.com/w/cpp/utility/tuple/get)

There is unfortunately no *type* overload since `get` needs to be an object, not a function template, and object templates cannot be overloaded.

#### `composer::apply_to`

Back binding version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

#### `composer::apply_using`

Front binding version of [`std::apply`](https://en.cppreference.com/w/cpp/utility/apply.html)

## `<composer/ranges.hpp>`

#### `composer::size`

Composable [`nodiscard`](#nodiscard) function object for [`std::ranges::size`](https://www.cppreference.com/w/cpp/ranges/size.html)

#### `composer::ssize`

Composable [`nodiscard`](#nodiscard) function object for [`std::ranges::ssize`](https://www.cppreference.com/w/cpp/ranges/ssize.html)

#### `composer::distance`

Composable [`nodiscard`](#nodiscard) function object for [`std::ranges::distance`](https://www.cppreference.com/w/cpp/iterator/ranges/distance.html)

`composable::distance` is callable either with a range, or with an iterator/sentinel pair.

## `<composer/algorithm.hpp>`

### Non-modifying sequence operations

#### `composer::all_of`

Back binding [`nodiscard`](#nodiscard) version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

#### `composer::any_of`

Back binding [`nodiscard`](#nodiscard) version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

#### `composer::none_of`

Back binding [`nodiscard`](#nodiscard) version of [`std::any_of`](https://en.cppreference.com/w/cpp/algorithm/ranges/all_any_none_of.html)

#### `composer::for_each`

Back binding version of [`std::ranges::for_each`](https://en.cppreference.com/w/cpp/algorithm/ranges/for_each.html)

`composer::for_each` cannot be called with an r-value range.

#### `composer::for_each_n`

Back binding version of [`std::ranges::for_each_n`](https://en.cppreference.com/w/cpp/algorithm/ranges/for_each_n.html)

#### `composer::count`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::count`](https://en.cppreference.com/w/cpp/algorithm/ranges/count.html)

#### `composer::count_if`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::count_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/count.html)

#### `composer::find`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

`composer::find` cannot be called with an r-value range.

#### `composer::find_if`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find_if`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

`composer::find_if` cannot be called with an r-value range.

#### `composer::find_if_not`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find_if_not`](https://en.cppreference.com/w/cpp/algorithm/ranges/find.html)

`composer::find_if_not` cannot be called with an r-value range.

#### `composer::find_last`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

`composer::find_last` cannot be called with r-value range.

#### `composer::find_last_if`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

`composer::find_last_if` cannot be called with an r-value range.

#### `composer::find_last_if_not`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find_last`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_last.html)

`composer::find_last_if_not` cannot be called with an r-value range.

#### `composer::find_end`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find_end`](https://en.cppreference.com/w/cpp/algorithm/ranges/find_end.html)

`composer.:find_end` cannot be called with an r-value range.

#### `composer::find_first_of`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::find_first_of](https://en.cppreference.com/w/cpp/algorithm/ranges/find_first_of.html)

`composer::find_first_of` cannot be called with an r-value range.

#### `composer::adjacent_find`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::adjacent_find`](https://www.cppreference.com/w/cpp/algorithm/ranges/adjacent_find.html)

`composer::adjacent_find` cannot be called with an r-value range.

#### `composer::search`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::search`](https://www.cppreference.com/w/cpp/algorithm/ranges/search.html)

`composer::search` cannot be called with an r-value range.

#### `composer::search_n`

`composer::search_n` cannot be called with an r-value range.

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::search_n`](https://www.cppreference.com/w/cpp/algorithm/ranges/search_n.html)

#### `composer::contains`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::contains`](https://www.cppreference.com/w/cpp/algorithm/ranges/contains.html)

#### `composer::contains_subrange`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::contains_subrange`](https://www.cppreference.com/w/cpp/algorithm/ranges/contains.html)

#### `composer::starts_with`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::starts_with`](https://en.cppreference.com/w/cpp/algorithm/ranges/starts_with.html)

**NOTE!** Not available in gcc-15 and lower

#### `composer::ends_with`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::ends_with`](https://en.cppreference.com/w/cpp/algorithm/ranges/ends_with.html)

**NOTE!** Not available in gcc-15 and lower

### Modifying sequence operations

#### `composer::fill`

Back binding version of [`std::ranges::fill`](https://en.cppreference.com/w/cpp/algorithm/ranges/fill.html)

`composer::fill` cannot be called with an r-value range.

#### `composer::fill_n`

Back binding version of [`std::ranges::fill_n`](https://en.cppreference.com/w/cpp/algorithm/ranges/fill_n.html)

#### `composer::generate`

Back binding version of [`std::ranges::generate`](https://en.cppreference.com/w/cpp/algorithm/ranges/generate.html)

`composer::generate` cannot be called with an r-value range.

#### `composer::generate_n`

Back binding version of [`std::ranges::generate_n`](https://en.cppreference.com/w/cpp/algorithm/ranges/generate_n.html)

#### `composer::remove`

Back binding version of [`std::ranges::remove`](https://en.cppreference.com/w/cpp/algorithm/ranges/remove.html)

`composer::remove` cannot be called with an r-value range.

### Partitioninng operations

#### `composer::is_partitioned`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::is_partitioned`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_partitioned.html)

### `composer::partition`

Back binding version of [`std::ranges::partition`](https://en.cppreference.com/w/cpp/algorithm/ranges/partition.html)

`composer::partition` cannot be called with an r-value range.

### `composer::partition_copy`

Back binding version of [`std::ranges::partition_copy`](https://en.cppreference.com/w/cpp/algorithm/ranges/partition_copy.html)

`composer::partition_copy` cannot be called with an r-value range.

### `composer::stable_partition`

Back binding version of [`std::ranges::stabpartition`](https://en.cppreference.com/w/cpp/algorithm/ranges/stable_partition.html)

`composer::stable_partition` cannod be called with an r-value range.

#### `composer::partition_point`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::partition_point`](https://en.cppreference.com/w/cpp/algorithm/ranges/partition_point)

`composer::partition_point` cannot be called with an r-value range.

### Sorting operations

#### `composer::is_sorted`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::is_sorted`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_sorted.html)

#### `composer::is_sorted_until`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::is_sorted_until`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_sorted_until.html)

`composer::is_sorted_until` cannot be called with an r-value range.

#### `composer::sort`

Back binding version of [`std::ranges::sort`](https://en.cppreference.com/w/cpp/algorithm/ranges/sort.html)

`composer::sort` cannot be called with an r-value range.

#### `composer::partial_sort`

Back binding version of [`std::ranges::partial_sort`](https://en.cppreference.com/w/cpp/algorithm/ranges/partial_sort.html)

`composer::partial_sort` cannot be called with an r-value range.

#### `composer::partial_sort_copy`

Back binding version of [`std::ranges::partial_sort_copy`](https://en.cppreference.com/w/cpp/algorithm/ranges/partial_sort_copy.html)

`composer::partial_sort_copy` cannot be called with r-value ranges.

_Note!_ If you bind the output range by value, the result will end up in the
copy of the range owned by the function object. If you want to bind the output
range, do so via `composer::ref`.

#### `composer::stable_sort`

[Back binding](#back_binding) version of
[`std::ranges::stable_sort`](https://en.cppreference.com/w/cpp/algorithm/ranges/stable_sort.html)

`composer::stable_sort` cannot be called with r-value ranges.

#### `composer::nth_element`

[Back binding](#back_binding) version of
[`std::ranges::nth_elementt`](https://en.cppreference.com/w/cpp/algorithm/ranges/nth_element.html)

`composer::nth_element` cannot be called with r-value ranges.

### Binary search operations  (on sorted ranges)

#### `composer::lower_bound`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::lower_bound`](https://en.cppreference.com/w/cpp/algorithm/ranges/lower_bound.html)

`composer::lower_bound` cannot be called with an r-value range.

#### `composer::upper_bound`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::upper_bound`](https://en.cppreference.com/w/cpp/algorithm/ranges/upper_bound.html)

`composer::upper_bound` cannot be called with an r-value range.

#### `composer::binary_search`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::binary_search`](https://en.cppreference.com/w/cpp/algorithm/ranges/binary_search.html)

#### `composer::equal_range`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::equal_range`](https://en.cppreference.com/w/cpp/algorithm/ranges/equal_rangeh.html)

`composer::equal_range` cannot be called with an r-value range.

### Set operations (on sorted ranges)

#### `composer::merge`

Back binding version o [`std::ranges::merge`](https://en.cppreference.com/w/cpp/algorithm/ranges/merge.html)

`composer::merge` cannot be called with r-value ranges.

#### `composer::inplace_merge`

Back binding version of [`std::ranges::inplace_merge`](https://en.cppreference.com/w/cpp/algorithm/ranges/inplace_merge.html)

`composer::inplace_merge` cannot be called with an r-value range.

#### `composer::includes`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::includes`](https://en.cppreference.com/w/cpp/algorithm/ranges/includes.html)

#### `composer::set_difference`

Back binding version of [`std::ranges::set_difference`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_difference.html)

#### `composer::set_intersection`

Back binding version of [`std::ranges::set_intersection`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_intersection.html)

#### `composer::set_symmetric_difference`

Back binding version of [`std::ranges::set_symmetric_difference`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_symmetric_difference.html)

#### `composer::set_union`

Back binding version of [`std::ranges::set_union`](https://en.cppreference.com/w/cpp/algorithm/ranges/set_union.html)

### Heap operations

#### `composer::is_heap`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::is_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_heap.html)

#### `composer::is_heap_until`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::is_heap_until`](https://en.cppreference.com/w/cpp/algorithm/ranges/is_heap_until.html)

`composer::is_heap_until` cannot be called with an r-value range.

#### `composer::make_heap`

Back binding version of [`std::ranges::make_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/make_heap.html)

`composer::make_heap` cannot be called with an r-value range.

#### `composer::push_heap`

Back binding version of [`std::ranges::push_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/push_heap.html)

`composer::push_heap` cannot be called with an r-value range.

#### `composer::pop_heap`

Back binding version of [´std::ranges::pop_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/pop_heap.html)

`composer::pop_heap` cannot be called with an r-value range.

#### `composer::sort_heap`

Back binding version of [´std::ranges::sort_heap`](https://en.cppreference.com/w/cpp/algorithm/ranges/sort_heap.html)

`composer::sort_heap` cannot be called with an r-value range.

### Minimum/maximum operations

#### `composer::max`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::max`](https://en.cppreference.com/w/cpp/algorithm/ranges/max.html)

#### `composer::max_element`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::max_element`](https://en.cppreference.com/w/cpp/algorithm/ranges/max_element.html)

`composer::max_element` cannot be called with an r-value range.

#### `composer::min`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::min`](https://en.cppreference.com/w/cpp/algorithm/ranges/min.html)

#### `composer::min_element`

`composer::minmax_element` cannot be called with an r-value range.

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::min_element`](https://en.cppreference.com/w/cpp/algorithm/ranges/min_element.html)

#### `composer::minmax_element`

`composer::minmax_element` cannot be called with an r-value range.

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::minmax_element`](https://en.cppreference.com/w/cpp/algorithm/ranges/minmax_element.html)

#### `composer::clamp`

Back binding [`nodiscard`](#nodiscard) version of [`std::ranges::clamp`](https://en.cppreference.com/w/cpp/algorithm/ranges/clamp.html)


### Permutation operations
