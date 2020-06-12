#include "watchme/window_impl.h"
#include "watchme/gui_thread_impl.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"

#include <thread>

#include <cassert>

namespace
{
std::thread::id gui_thread_id;

void assert_this_is_gui_thread()
{
  assert( gui_thread_id == std::this_thread::get_id() );
}
}  // namespace

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

namespace watch_me
{
WindowImpl::WindowImpl() : d3d_device( nullptr ), d3d_pp{}
{
  gui_thread_id = std::this_thread::get_id();

  const auto wnd = Create( NULL, ATL::CWindow::rcDefault, L"WatchMe", WS_OVERLAPPEDWINDOW );
  if ( !wnd )
  {
    assert( false );
  }

  init_d3d();
  init_imgui();

  ShowWindow( SW_SHOWDEFAULT );
  UpdateWindow();
}

WindowImpl::~WindowImpl()
{
  assert_this_is_gui_thread();

  stop_imgui();
  stop_d3d();

  if ( m_hWnd )
  {
    const BOOL window_destroyed = DestroyWindow();
    assert( window_destroyed );
  }
}

void WindowImpl::init_d3d()
{
  assert_this_is_gui_thread();

  d3d_pp.Windowed               = TRUE;
  d3d_pp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  d3d_pp.BackBufferFormat       = D3DFMT_UNKNOWN;
  d3d_pp.EnableAutoDepthStencil = TRUE;
  d3d_pp.AutoDepthStencilFormat = D3DFMT_D16;
  d3d_pp.PresentationInterval   = D3DPRESENT_INTERVAL_ONE;

  const auto device_created =
      get_d3d()->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3d_pp, &d3d_device );
  assert( device_created == D3D_OK );
  assert( d3d_device );
}

void WindowImpl::stop_d3d()
{
  assert_this_is_gui_thread();

  if ( d3d_device )
  {
    d3d_device->Release();
  }
}

void WindowImpl::init_imgui()
{
  assert_this_is_gui_thread();

  const bool win32_initialized = ImGui_ImplWin32_Init( m_hWnd );
  assert( win32_initialized );
  const bool dx9_initialized = ImGui_ImplDX9_Init( d3d_device );
  assert( dx9_initialized );
}

void WindowImpl::stop_imgui()
{
  assert_this_is_gui_thread();

  ImGui_ImplDX9_Shutdown();
  ImGui_ImplWin32_Shutdown();
}

void WindowImpl::start_frame()
{
  assert_this_is_gui_thread();

  ImGui_ImplDX9_NewFrame();
  ImGui_ImplWin32_NewFrame();
}

void WindowImpl::stop_frame()
{
  assert_this_is_gui_thread();

  d3d_device->SetRenderState( D3DRS_ZENABLE, false );
  d3d_device->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
  d3d_device->SetRenderState( D3DRS_SCISSORTESTENABLE, false );

  static const ImVec4 clear_color = ImVec4( 0.45f, 0.55f, 0.60f, 1.00f );

  D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(
      (int)( clear_color.x * 255.0f ), (int)( clear_color.y * 255.0f ), (int)( clear_color.z * 255.0f ), (int)( clear_color.w * 255.0f ) );
  d3d_device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0 );
  if ( d3d_device->BeginScene() >= 0 )
  {
    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData() );
    d3d_device->EndScene();
  }
  HRESULT result = d3d_device->Present( NULL, NULL, NULL, NULL );

  // Handle loss of D3D9 device
  if ( result == D3DERR_DEVICELOST && d3d_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET )
  {
    reset_device();
  }
}

WindowImpl::TimerHandle WindowImpl::start_timer( const std::chrono::milliseconds interval, TimersFunc on_timer )
{
  assert_this_is_gui_thread();
  return timers_map.start_timer( m_hWnd, interval, std::move( on_timer ) );
}

void WindowImpl::stop_timer( TimerHandle handle )
{
  assert_this_is_gui_thread();
  timers_map.stop_timer( m_hWnd, handle );
}


BOOL WindowImpl::ImGuiMsgHandler::ProcessWindowMessage( const HWND wnd,
                                                        const UINT msg,
                                                        const WPARAM wparam,
                                                        const LPARAM lparam,
                                                        LRESULT& result,
                                                        const DWORD /*msg_map*/ )
{
  result = ImGui_ImplWin32_WndProcHandler( wnd, msg, wparam, lparam );
  return false;
}

LRESULT WindowImpl::OnWmSize( UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled )
{
  if ( d3d_device && wParam != SIZE_MINIMIZED )
  {
    d3d_pp.BackBufferWidth  = LOWORD( lParam );
    d3d_pp.BackBufferHeight = HIWORD( lParam );
    reset_device();
  }

  handled = true;
  return 0;
}

void WindowImpl::reset_device()
{
  ImGui_ImplDX9_InvalidateDeviceObjects();

  const auto result = d3d_device->Reset( &d3d_pp );
  assert( result != D3DERR_INVALIDCALL );

  ImGui_ImplDX9_CreateDeviceObjects();
}
}  // namespace watch_me