#pragma

#include "watchme/window_timers_map.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace watch_me
{
struct Var;
class WindowImpl;
class Window
{
public:
  Window();
  ~Window();

  void add_var( std::unique_ptr<Var> var );
  void remove_var( const void* addr );

private:
  void render();
  virtual void make_frame();

private:
  std::unique_ptr<WindowImpl> impl;
  std::unordered_map<const void*, std::unique_ptr<Var>> vars;

  WindowTimersMap::TimerHandle timer;
};
}  // namespace watch_me