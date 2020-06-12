#include "watchme/ptrvar.h"

#include "imgui.h"

#include <cstdio>

namespace watch_me
{

PtrVar::PtrVar( const char* name, const void* ptr ) : Var( name, ptr ), var_ptr( ptr )
{
}

void PtrVar::draw_frame_impl()
{
  char buff[ sizeof( void* ) * 2 + 1 ];
  sprintf( buff, "%p", &var_ptr );

  ImGui::InputText( "pointer", buff, std::size_t( buff ) );
}

}  // namespace watch_me