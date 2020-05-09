#include "watchme/watchme.h"

#include <thread>

int main()
{

  int x = 0;
  int y = 0;

  WATCH_ME( x );
  WATCH_ME( y );

  for ( size_t i = 0; i < 5; ++i )
  {
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    x += 10;
    y += 20;
  }

  return 0;
}