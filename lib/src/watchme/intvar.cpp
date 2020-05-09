#include "watchme/intvar.h"

#include "imgui.h"
#include <string>

namespace watch_me
{
IntVar::IntVar( const char* const name, const int* const ptr ) : Var( name, static_cast<const void*>( ptr ) ), int_ptr( ptr )
{
}

void IntVar::draw_frame_impl()
{
  auto str = std::to_string( *int_ptr );
  ImGui::InputText( "", &str[ 0 ], str.size() + 1 );
}

}  // namespace watch_me