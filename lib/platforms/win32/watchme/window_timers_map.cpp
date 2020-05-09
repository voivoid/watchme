#include "watchme/window_timers_map.h"

#include <atomic>
#include <cassert>


namespace watch_me
{
	WindowTimersMap::TimerHandle WindowTimersMap::start_timer(const HWND wnd, const std::chrono::milliseconds interval, TimersFunc on_timer)
	{
		static std::atomic<UINT_PTR> timer_id_counter = 0;
		const auto timer_id = ++timer_id_counter;

		assert(wnd);
		timer_handlers.emplace(timer_id, std::move(on_timer));
		auto timer_result = ::SetTimer(wnd, timer_id, static_cast<UINT>( interval.count() ), nullptr);
		assert(timer_result);
		assert(timer_result == timer_id);

		return { timer_id };
	}

	LRESULT WindowTimersMap::on_timer(const UINT /*msg*/, const WPARAM wparam, const LPARAM /*lparam*/, BOOL& handled)
	{
		const auto timer_id = static_cast<UINT_PTR>(wparam);
		auto handler_iter = timer_handlers.find(timer_id);
		if (handler_iter != timer_handlers.cend())
		{
			handler_iter->second();
		}
		else
		{
			assert(false);
		}

		handled = true;
		return 0;
	}

	void WindowTimersMap::stop_timer(const HWND wnd, const TimerHandle handle)
	{
		const auto timer_stopped = KillTimer(wnd, handle);
		assert(timer_stopped);
	}



}