#pragma once

#include <chrono>
#include "watchme/window_timers_map.h"

#include <atlbase.h>
#include <atlwin.h>
#include <d3d9.h>

namespace watch_me
{
	class WindowImpl : public ATL::CWindowImpl<WindowImpl, ATL::CWindow, ATL::CFrameWinTraits>
	{
	public:
		DECLARE_WND_CLASS_EX(L"WatchMe Wnd Class", CS_CLASSDC, (HBRUSH)(COLOR_WINDOW + 1))
		WindowImpl();
		~WindowImpl();

		using TimersFunc = WindowTimersMap::TimersFunc;
		using TimerHandle = WindowTimersMap::TimerHandle;
		TimerHandle start_timer( std::chrono::milliseconds interval, TimersFunc on_timer );
		void stop_timer(TimerHandle handle);
		

		void start_frame();
		void stop_frame();

	private:
		void init_d3d();
		void init_imgui();

		void stop_d3d();
		void stop_imgui();

		void reset_device();

	private:
		struct ImGuiMsgHandler
		{
			BOOL ProcessWindowMessage(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam, LRESULT& result, DWORD msg_map = 0);
		};

		BEGIN_MSG_MAP(WindowImpl)
			MESSAGE_HANDLER( WM_SIZE, OnWmSize )
			CHAIN_MSG_MAP_MEMBER( imgui_msg_handler )
			CHAIN_MSG_MAP_MEMBER( timers_map )
		END_MSG_MAP()

		LRESULT OnWmSize(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled);

	private:
		LPDIRECT3DDEVICE9 d3d_device = nullptr;
		D3DPRESENT_PARAMETERS d3d_pp;

		WindowTimersMap timers_map;
		ImGuiMsgHandler imgui_msg_handler;
	};
}