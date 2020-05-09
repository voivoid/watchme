#include "watchme/var.h"

#include "imgui.h"

namespace watch_me
{
void Var::draw_frame()
{
  ImGui::Begin( name );
  draw_frame_impl();
  ImGui::End();
}
}  // namespace watch_me