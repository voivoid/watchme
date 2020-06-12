#pragma once

#include "watchme/var.h"

namespace watch_me
{

template <typename T>
struct IntegerVar : Var
{
  IntegerVar( const char* name, const T* ptr );

  virtual void draw_frame_impl() override;

private:
  const T* const var_ptr;
};

}  // namespace watch_me