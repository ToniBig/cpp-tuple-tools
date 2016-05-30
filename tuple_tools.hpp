#ifndef tuple_tools_hpp_
#define tuple_tools_hpp_

#include <iosfwd>
#include <tuple>

namespace tuple_tools {

template<typename Tuple>
constexpr std::size_t size( Tuple const & t )
{
  return std::tuple_size<Tuple>::value;
}

namespace detail {

template<std::size_t Start, std::size_t End>
struct map_tuple_impl
{
  template<typename Tuple, typename Mapping>
  constexpr static auto invoke( Tuple const & tuple,
                                Mapping&& mapping ) -> decltype( std::tuple_cat( map_tuple_impl<Start, End-1>::invoke( tuple, mapping ), std::make_tuple( mapping( std::get<End>( tuple ) ) ) ) )
  {
    return std::tuple_cat( map_tuple_impl<Start, End - 1>::invoke( tuple, mapping ), std::make_tuple( mapping( std::get<End>( tuple ) ) ) );
  }
};

template<std::size_t Start>
struct map_tuple_impl<Start, Start>
{
  template<typename Tuple, typename Mapping>
  constexpr static auto invoke( Tuple const & tuple,
                                Mapping&& mapping ) -> decltype(std::make_tuple( mapping(std::get<Start>( tuple ) ) ) )
  {
    return std::make_tuple( mapping( std::get<Start>( tuple ) ) );
  }
};

template<size_t N>
struct print_callback
{
  template<typename T>
  void operator()( T const & element,
                   size_t i )
  {
    std::cout << element;

    if ( i != N )
    {
      std::cout << ", ";
    }
  }
};

template<std::size_t Start, std::size_t End>
struct tuple_for_each_impl
{
  template<typename Tuple, typename Mapping>
  constexpr static void invoke( Tuple const & tuple,
                                Mapping&& mapping )
  {
    mapping( std::get<Start>( tuple ), Start );
    tuple_for_each_impl<Start + 1, End>::invoke( tuple, mapping );
  }
};

template<std::size_t End>
struct tuple_for_each_impl<End, End>
{
  template<typename Tuple, typename Mapping>
  constexpr static void invoke( Tuple const & tuple,
                                Mapping&& mapping )
  {
    mapping( std::get<End>( tuple ), End );
  }
};

} // namespace detail

template<typename Tuple, typename Mapping>
auto map_tuple( Tuple const & tuple,
                Mapping&& mapping ) -> decltype(detail::map_tuple_impl<0,std::tuple_size<Tuple>::value-1>::invoke( tuple, mapping ) )
{
  constexpr size_t N = std::tuple_size<Tuple>::value;
  return detail::map_tuple_impl<0, N - 1>::invoke( tuple, mapping );
}

template<size_t Start, size_t End, typename Tuple, typename Mapping>
auto map_tuple_slice( Tuple const & tuple,
                      Mapping&& mapping ) -> decltype(detail::map_tuple_impl<Start,End>::invoke( tuple, mapping ) )
{
  return detail::map_tuple_impl<Start, End>::invoke( tuple, mapping );
}

template<typename Tuple, typename Callback>
void tuple_for_each( Tuple const & tuple,
                     Callback&& callback )
{
  constexpr size_t N = std::tuple_size<Tuple>::value;
  detail::tuple_for_each_impl<0, N - 1>::invoke( tuple, callback );
}

template<size_t Start, size_t End, typename Tuple, typename Callback>
void tuple_for_each_slice( Tuple const & tuple,
                           Callback&& callback )
{
  detail::tuple_for_each_impl<Start, End>::invoke( tuple, callback );
}

template<typename Tuple>
void print_tuple( Tuple const & tuple )
{
  std::cout << "(";
  tuple_for_each( tuple, detail::print_callback<std::tuple_size<Tuple>::value - 1> { } );
  std::cout << ")" << std::endl;
}
}
// namespace tuple_tools

#endif // tuple_tools_hpp_
