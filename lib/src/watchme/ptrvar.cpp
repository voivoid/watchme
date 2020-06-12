#include "watchme/ptrvar.h"

#include "imgui.h"

#include <cstdio>

namespace watch_me
{

PtrVar::PtrVar( const char* name, const void* ptr, const char* type_name ) : Var( name, ptr ), var_ptr( ptr ), type_name_str( type_name )
{
  type_name_str += '*';
}

void PtrVar::draw_frame_impl()
{
  char buff[ ( 2 /* "0x" */ ) + sizeof( void* ) * 2 + ( 1 /* zero terminator */ ) ];
  sprintf( buff, "0x%p", &var_ptr );

  ImGui::InputText( type_name_str.c_str(), buff, std::size_t( buff ) );
}

}  // namespace watch_me