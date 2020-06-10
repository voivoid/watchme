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


}  // namespace

namespace watch_me
{
IntVar::IntVar( const char* const name, const int* const ptr ) : Var( name, static_cast<const void*>( ptr ) ), int_ptr( ptr )
{
}

void IntVar::draw_frame_impl()
{
  char buff[ count_digits<int>() + 1 ];
  *std::to_chars( std::begin( buff ), std::end( buff ) - 1, *int_ptr ).ptr = '\0';

  ImGui::InputText( "int", buff, std::size( buff ) );
}

}  // namespace watch_me