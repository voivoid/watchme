#pragma once

namespace watch_me
{
struct Var
{
  Var( const char* n, const void* addr ) : name( n ), ptr_addr( addr )
  {
  }

  virtual ~Var() = default;

  void draw_frame();
  virtual void draw_frame_impl() = 0;

  const char* const name;
  const void* const ptr_addr;
};
}  // namespace watch_me