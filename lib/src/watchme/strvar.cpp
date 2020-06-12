#include "watchme/strvar.h"

#include "imgui.h"

namespace watch_me
{

StringVar::StringVar( const char* const name, const std::string* const ptr ) : Var( name, ptr ), var_ptr( ptr )
{
}

void StringVar::draw_frame_impl()
{
  ImGui::InputText( "std::string", const_cast<char*>( var_ptr->c_str() ), var_ptr->size() + 1 );
}

}  // namespace watch_me