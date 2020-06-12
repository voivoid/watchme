#include "watchme/window.h"

#include "watchme/var.h"
#include "watchme/gui_thread.h"
#include "watchme/window_impl.h"

#include <functional>

#include "imgui.h"

namespace watch_me
{
Window::Window() : timer( 0 )
{
  send_gui_task( [ this ]() {
    assert_this_is_gui_thread();

    const auto* context = ImGui::CreateContext();
    assert( context );
    ImGui::StyleColorsLight();

    impl.reset( new WindowImpl );
    this->timer = impl->start_timer( std::chrono::milliseconds{ 1000 / 60 }, std::bind( &Window::render, this ) );
  } );
}

Window::~Window()
{
  send_gui_task( [ this ]() {
    assert_this_is_gui_thread();
    impl->stop_timer( timer );
  } );

  send_gui_task( [ this ]() {
    assert_this_is_gui_thread();
    impl.reset();
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
  assert( var );

  // std::function is copyable so std::unique_ptr can't be captured since it's move-only
  post_gui_task( [ this, v = var.get() ] { vars.emplace( v->ptr_addr, std::unique_ptr<Var>( v ) ); } );

  var.release();
}

void Window::remove_var( const void* addr )
{
  post_gui_task( [ this, addr ] { this->vars.erase( addr ); } );
}
}  // namespace watch_me