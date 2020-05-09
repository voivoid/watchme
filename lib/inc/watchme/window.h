#pragma

#include "watchme/var.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace watch_me
{
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
};
}  // namespace watch_me