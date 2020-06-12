#pragma once

#include "watchme/var.h"

#include <string>

namespace watch_me
{

struct StringVar : Var
{
  StringVar( const char* name, const std::string* ptr );

  virtual void draw_frame_impl() override;

private:
  const std::string* const var_ptr;
};

}  // namespace watch_me