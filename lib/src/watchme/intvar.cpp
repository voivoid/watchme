#include "watchme/intvar.h"

#include "imgui.h"

#include <charconv>
#include <limits>
#include <string>

namespace
{
template <typename T>
constexpr size_t count_digits()
{
  auto n        = std::numeric_limits<T>::max();
  size_t digits = 0;
  while ( n )
  {
    n /= 10;
    ++digits;
  }

  size_t sign = std::numeric_limits<T>::is_signed ? 1 : 0;

  return digits + sign;
}

template <typename T>
const char* const type_name;

#define DEFINE_TYPE( type )                                                                                                                \
  template <>                                                                                                                              \
  const char* const type_name<type> = #type;                                                                                               \
  template watch_me::IntegerVar<type>

DEFINE_TYPE( short );
DEFINE_TYPE( unsigned short );
DEFINE_TYPE( int );
DEFINE_TYPE( unsigned int );
DEFINE_TYPE( long );
DEFINE_TYPE( unsigned long );
DEFINE_TYPE( long long );
DEFINE_TYPE( unsigned long long );


}  // namespace

namespace watch_me
{
template <typename T>
IntegerVar<T>::IntegerVar( const char* const name, const T* const ptr ) : Var( name, ptr ), var_ptr( ptr )
{
}

template <typename T>
void IntegerVar<T>::draw_frame_impl()
{
  char buff[ count_digits<T>() + 1 ];
  *std::to_chars( std::begin( buff ), std::end( buff ) - 1, *var_ptr ).ptr = '\0';

  ImGui::InputText( type_name<T>, buff, std::size( buff ) );
}

}  // namespace watch_me