#pragma once

#include <map>
#include <mutex>

#define WATCH_ME( var ) watch_me::Watcher var##_watcher( var, #var );
#define UPDATE_ME( var ) watch_me::Watcher::update( var );

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
  Watcher( T* const var, const char* const var_name ) : Watcher( static_cast<const void*>( &var ), var_name )
  {
  }

  Watcher( const void* var, const char* const var_name );

  ~Watcher();

  template <typename T>
  static void update( const T& var )
  {
  }

private:
  template <typename T>
  void init( const T& var, const char* const var_name );
  void create_window();

private:
  const void* var_ptr;
  std::shared_ptr<Window> window;
};
}  // namespace watch_me