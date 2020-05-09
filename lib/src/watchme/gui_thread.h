#pragma once

#include <functional>

namespace watch_me
{
using GuiTask = std::function<void( void )>;
void send_gui_task( GuiTask task );
void post_gui_task( GuiTask task );

void assert_this_is_gui_thread();
}  // namespace watch_me