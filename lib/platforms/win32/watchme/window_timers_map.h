#pragma once

#include <atlbase.h>
#include <atlwin.h>

#include <chrono>
#include <functional>
#include <map>

namespace watch_me
{
class WindowTimersMap
{
public:
  using TimersFunc = std::function<void( void )>;
  BEGIN_MSG_MAP( WindowTimersMap )
  MESSAGE_HANDLER( WM_TIMER, on_timer )
  END_MSG_MAP()

  using TimerHandle = UINT_PTR;

  TimerHandle start_timer( HWND wnd, const std::chrono::milliseconds interval, TimersFunc on_timer );
  void stop_timer( HWND wnd, TimerHandle );

private:
  LRESULT on_timer( UINT msg, WPARAM wparam, LPARAM lparam, BOOL& handled );

private:
  std::map<UINT_PTR, TimersFunc> timer_handlers;
};
}  // namespace watch_me