# cpp-tuple-tools

Elementwise processing and mapping of C++ tuples

## Why

Suppose you have a tuple of objects of possibly different types on which you want to apply a compile-time polymorphic function and store the result in another tuple. To achieve this, `cpp-tuple-tools` provides the functions `map_tuple` and `map_tuple_slice`, where the former processes a tuple completly and the latter works on a provided index range. There are also two functions `tuple_for_each` and `tuple_for_each_slice` to process a tuple elementwise without returning a tuple result.
```c++
#include <iostream>
#include <tuple>

#include "tuple_tools.hpp"

namespace tt = tuple_tools;

template<typename T>
struct holder
{
  T _value;

  holder( T value ) :
          _value( value )
  {
  }

  T get( ) const
  {
    return _value;
  }
};

template<typename T>
std::ostream& operator<<( std::ostream& stream,
                          holder<T> const & h )
{
  stream << "holder has: " << h.get( );

  return stream;
}

struct holder_get
{
  template<typename T>
  T operator()( holder<T> const & h )
  {
    return h.get( );
  }
};

struct invoke_callback
{
  template<typename F>
  auto operator()( F&& f ) ->decltype(f())
  {
    return f( );
  }
};

int main( )
{
  auto tuple = std::make_tuple( holder<int> { 42 }, holder<double> { 1.23 }, holder<bool> { false }, holder<std::string> { "abc" } );

  tt::print_tuple( tuple );
  tt::print_tuple( tt::map_tuple( tuple, holder_get { } ) );
  tt::print_tuple( tt::map_tuple_slice<1, 3>( tuple, holder_get { } ) );

  auto f1 = [](void)
  { return "Hi there!";};
  auto f2 = [](void)
  { return true;};
  auto f3 = [](void)
  { return 1.23;};
  auto f4 = [](void)
  { return 777;};

  tt::print_tuple( tt::map_tuple( std::make_tuple( f1, f2, f3, f4 ), invoke_callback { } ) );

  return 0;
}
```
Output:
```
(holder has: 42, holder has: 1.23, holder has: 0, holder has: abc)
(42, 1.23, 0, abc)
(1.23, 0, abc)
(Hi there!, 1, 1.23, 777)
```
