#pragma once

#include <functional>

namespace watch_me
{
void send_gui_task( std::function<void()> task );
void post_gui_task( std::function<void()> task );

void assert_this_is_gui_thread();
}  // namespace watch_me