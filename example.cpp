//============================================================================
// Name        : tuple_loop.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

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
