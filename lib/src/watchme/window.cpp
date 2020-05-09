#include "watchme/window.h"
#include "watchme/gui_thread.h"
#include "watchme/window_impl.h"

#include <functional>

#include "imgui.h"

namespace watch_me
{
Window::Window()
{
  send_gui_task( [ this ]() {
    assert_this_is_gui_thread();

    const auto* context = ImGui::CreateContext();
    assert( context );
    ImGui::StyleColorsLight();

    impl.reset( new WindowImpl );
    impl->start_timer( std::chrono::milliseconds{ 1000 / 60 }, std::bind( &Window::render, this ) );
  } );
}

Window::~Window()
{
  auto* pimpl = impl.release();
  send_gui_task( [ pimpl ]() {
    assert_this_is_gui_thread();

    delete pimpl;
    ImGui::DestroyContext();
  } );
}

void Window::render()
{
  assert_this_is_gui_thread();

  impl->start_frame();
  ImGui::NewFrame();

  make_frame();

  ImGui::EndFrame();
  impl->stop_frame();
}

void Window::make_frame()
{
  assert_this_is_gui_thread();

  for ( const auto& var : vars )
  {
    var.second->draw_frame();
  }
}

void Window::add_var( std::unique_ptr<Var> var )
{
  auto ptr = var->ptr_addr;
  vars.emplace( ptr, std::move( var ) );
}

void Window::remove_var( const void* addr )
{
  vars.erase( addr );
}
}  // namespace watch_me