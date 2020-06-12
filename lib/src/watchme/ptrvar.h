#pragma once

#include "watchme/var.h"
#include <string>

namespace watch_me
{

struct PtrVar : Var
{
  PtrVar( const char* name, const void* ptr, const char* type_name );

  virtual void draw_frame_impl() override;

private:
  const void* const var_ptr;
  std::string type_name_str;
};

}  // namespace watch_me