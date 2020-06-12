#include "watchme/watchme.h"

#include "watchme/numvar.h"
#include "watchme/ptrvar.h"
#include "watchme/strvar.h"
#include "watchme/window.h"

#include <cassert>
#include <memory>
#include <string>
#include <type_traits>

#include "imgui.h"

namespace watch_me
{
namespace
{
std::weak_ptr<Window> window_ptr;
std::once_flag window_created_flag;

void add( Window& window, const std::string& var, const char* const var_name )
{
  window.add_var( std::make_unique<watch_me::StringVar>( var_name, &var ) );
}

template <typename T, std::enable_if_t<std::is_integral_v<T> || std::is_floating_point_v<T>>* = nullptr>
void add( Window& window, const T& var, const char* const var_name )
{
  window.add_var( std::make_unique<watch_me::NumericVar<T>>( var_name, &var ) );
}

void add( Window& window, const void* var, const char* const var_name, const char* const type_name )
{
  window.add_var( std::make_unique<watch_me::PtrVar>( var_name, var, type_name ) );
}

}  // namespace

template <typename T>
Watcher::Watcher( const T& var, const char* const var_name ) : var_ptr( static_cast<const void*>( &var ) )
{
  create_window();

  assert( window );
  add( *window, var, var_name );
}

Watcher::Watcher( const void* var, const char* const var_name, const char* const type_name )
{
  create_window();

  assert( window );
  add( *window, var, var_name, type_name );
}

Watcher::~Watcher()
{
  assert( window );
  window->remove_var( var_ptr );
}

void Watcher::create_window()
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
}

#define DEFINE_TYPE( type ) template Watcher::Watcher( const type& var, const char* const var_name );

DEFINE_TYPE( short );
DEFINE_TYPE( unsigned short );
DEFINE_TYPE( int );
DEFINE_TYPE( unsigned int );
DEFINE_TYPE( long );
DEFINE_TYPE( unsigned long );
DEFINE_TYPE( long long );
DEFINE_TYPE( unsigned long long );
DEFINE_TYPE( float );
DEFINE_TYPE( double );
DEFINE_TYPE( long double );
DEFINE_TYPE( std::string );

}  // namespace watch_me