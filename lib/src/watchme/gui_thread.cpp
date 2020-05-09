#include "watchme/gui_thread.h"
#include "watchme/gui_thread_impl.h"

#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>

#include <cassert>

namespace
{
std::once_flag g_gui_thread_once_flag;
std::thread g_gui_thread;
std::queue<watch_me::GuiTask> g_gui_tasks;
std::mutex g_gui_tasks_mutex;

struct gui_thread_stopper
{
  ~gui_thread_stopper()
  {
    if ( g_gui_thread.joinable() )
    {
      watch_me::stop_gui_thread( g_gui_thread.native_handle() );
      g_gui_thread.join();
    }
  }
} gui_thread_stopper;

void process_gui_tasks()
{
  watch_me::assert_this_is_gui_thread();

  std::unique_lock<std::mutex> lock( g_gui_tasks_mutex );
  while ( !g_gui_tasks.empty() )
  {
    const auto task = std::move( g_gui_tasks.front() );
    g_gui_tasks.pop();

    lock.unlock();
    task();
    lock.lock();
  }
}

void start_gui_thread()
{
  assert( !g_gui_thread.joinable() );

  std::promise<void> gui_thread_started;
  g_gui_thread = std::thread( std::bind( &watch_me::gui_thread_impl, std::ref( gui_thread_started ) ) );

  gui_thread_started.get_future().wait();
}

void notify_gui_thread_to_process_tasks()
{
  watch_me::process_tasks_queue( g_gui_thread.native_handle(), &process_gui_tasks );
}

}  // namespace

namespace watch_me
{
void send_gui_task( GuiTask task )
{
  std::call_once( g_gui_thread_once_flag, &start_gui_thread );

  std::promise<void> task_promise;
  auto task_future = task_promise.get_future();

  {
    std::lock_guard<std::mutex> lock( g_gui_tasks_mutex );
    g_gui_tasks.push( [ &task_promise, task = std::move( task ) ]() {
      task();
      task_promise.set_value();
    } );
  }

  notify_gui_thread_to_process_tasks();
  task_future.wait();
}

void post_gui_task( GuiTask task )
{
  std::call_once( g_gui_thread_once_flag, &start_gui_thread );

  {
    std::lock_guard<std::mutex> lock( g_gui_tasks_mutex );
    g_gui_tasks.push( std::move( task ) );
  }
  notify_gui_thread_to_process_tasks();
}

void assert_this_is_gui_thread()
{
  assert( g_gui_thread.get_id() == std::this_thread::get_id() );
}
}  // namespace watch_me