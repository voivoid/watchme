#pragma once

#include <future>

#include <d3d9.h>

namespace watch_me
{
using GuiThreadHandle = void*;

void gui_thread_impl( std::promise<void>& thread_started );
void stop_gui_thread( GuiThreadHandle gui_thread_native_handle );

using ProcessGuiTasksFuncPtr = void ( * )();
void process_tasks_queue( GuiThreadHandle gui_thread_native_handle, ProcessGuiTasksFuncPtr process_tasks );

LPDIRECT3D9 get_d3d();
}  // namespace watch_me