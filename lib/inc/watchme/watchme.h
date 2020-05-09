#pragma once

#include <map>
#include <mutex>

#define WATCH_ME( var ) watch_me::watcher var##_watcher( var, #var );
#define UPDATE_ME( var ) watch_me::watcher::update( var );

namespace watch_me
{
class Window;
class watcher
{

public:
  watcher( const int& var, const char* const var_name ) : var_ptr( &var )
  {
    add_window( var_ptr, var_name );
  }

  ~watcher();

  template <typename T>
  static void update( const T& var )
  {
  }

private:
  void add_window( const int* var_ptr, const char* var_name );
  void remove_window();

private:
  const int* var_ptr;
  std::shared_ptr<Window> window;
};
}  // namespace watch_me