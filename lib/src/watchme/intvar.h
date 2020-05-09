#pragma once

#include "watchme/var.h"

namespace watch_me
{

struct IntVar : Var
{
  IntVar( const char* const name, const int* const ptr );

  virtual void draw_frame_impl() override;

  const int* const int_ptr;
};

}  // namespace watch_me