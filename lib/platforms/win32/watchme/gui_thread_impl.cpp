#include "watchme/gui_thread_impl.h"

#include "imgui.h"

#include <atlbase.h>

#include <cassert>

namespace
{
LPDIRECT3D9 g_d3d = nullptr;

DWORD get_thread_id( const watch_me::GuiThreadHandle gui_thread_native_handle )
{
  assert( gui_thread_native_handle );

  const auto thread_id = GetThreadId( gui_thread_native_handle );
  assert( thread_id );

  return thread_id;
}

void post_thread_message( const watch_me::GuiThreadHandle gui_thread_native_handle,
                          const UINT msg,
                          const WPARAM wparam = {},
                          const LPARAM lparam = {} )
{
  const auto thread_id         = get_thread_id( gui_thread_native_handle );
  const auto is_message_posted = ::PostThreadMessage( thread_id, msg, wparam, lparam );
  assert( is_message_posted );
}

void init_d3d()
{
  LPDIRECT3D9 d3d = Direct3DCreate9( D3D_SDK_VERSION );
  assert( d3d );

  g_d3d = d3d;
}

void stop_d3d()
{
  watch_me::get_d3d()->Release();
}
}  // namespace

namespace watch_me
{
constexpr auto check_gui_tasks_message = WM_USER;

void gui_thread_impl( std::promise<void>& thread_started )
{
  const HINSTANCE module = ::GetModuleHandle( NULL );
  assert( module );
  ATL::_AtlBaseModule.AddResourceInstance( module );

  const BOOL convert_to_gui_thread = IsGUIThread( TRUE );
  assert( convert_to_gui_thread );

  IMGUI_CHECKVERSION();

  init_d3d();

  thread_started.set_value();

  MSG msg;
  while ( GetMessage( &msg, NULL, 0, 0 ) > 0 )
  {
    if ( msg.message == check_gui_tasks_message )
    {
      auto process_gui_tasks = reinterpret_cast<ProcessGuiTasksFuncPtr>( msg.wParam );
      process_gui_tasks();
      continue;
    }

    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  stop_d3d();
}

void stop_gui_thread( GuiThreadHandle gui_thread_native_handle )
{
  post_thread_message( gui_thread_native_handle, WM_QUIT );
}

void process_tasks_queue( const GuiThreadHandle gui_thread_native_handle, const ProcessGuiTasksFuncPtr process_tasks )
{
  assert( process_tasks );
  post_thread_message( gui_thread_native_handle, check_gui_tasks_message, reinterpret_cast<WPARAM>( process_tasks ) );
}

LPDIRECT3D9 get_d3d()
{
  assert( g_d3d );
  return g_d3d;
}
}  // namespace watch_me