#pragma once

#include "watchme/var.h"

namespace watch_me
{

struct PtrVar : Var
{
  PtrVar( const char* name, const void* ptr );

  virtual void draw_frame_impl() override;

private:
  const void* const var_ptr;
};

}  // namespace watch_me