#pragma once

#include <map>
#include <mutex>

#define WATCH_ME( var ) watch_me::Watcher var##_watcher( var, #var );
#define UPDATE_ME( var ) watch_me::Watcher::update( var );

namespace watch_me
{
class Window;
class Watcher
{

public:
  Watcher( const int& var, const char* const var_name ) : var_ptr( &var )
  {
    add_window( var_ptr, var_name );
  }

  ~Watcher();

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