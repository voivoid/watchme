#pragma once

#include <map>
#include <mutex>
#include <typeinfo>

#define WATCH_ME( var ) watch_me::Watcher var##_watcher( var, #var );

namespace watch_me
{
class Var;
class Window;
class Watcher
{

public:
  template <typename T>
  Watcher( const T& var, const char* const var_name );

  template <typename T>
  Watcher( T* const var, const char* const var_name ) : Watcher( static_cast<const void*>( &var ), var_name, typeid( T ).name() )
  {
  }

  ~Watcher();


private:
  Watcher( const void* var, const char* const var_name, const char* const type_name );

  void create_window();

private:
  const void* var_ptr;
  std::shared_ptr<Window> window;
};
}  // namespace watch_me