#include "watchme/watchme.h"
#include "watchme/intvar.h"
#include "watchme/window.h"

#include <cassert>
#include <memory>
#include <string>

#include "imgui.h"

namespace watch_me
{
namespace
{
std::weak_ptr<Window> window_ptr;
std::once_flag window_created_flag;
}  // namespace

Watcher::~Watcher()
{
  remove_window();
}

void Watcher::add_window( const int* const var_ptr, const char* const var_name )
{
  std::call_once( window_created_flag, [ this ] {
    auto wnd             = std::make_shared<Window>();
    watch_me::window_ptr = wnd;
    this->window         = wnd;
  } );

  if ( !window )
  {
    window = window_ptr.lock();
  }

  window->add_var( std::make_unique<IntVar>( var_name, var_ptr ) );
}

void Watcher::remove_window()
{
  assert( window );
  window->remove_var( static_cast<const void*>( var_ptr ) );
}
}  // namespace watch_me