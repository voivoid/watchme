#include "watchme/watchme.h"

#include <string>
#include <thread>

int main()
{
  int x           = 0;
  long long y     = 0;
  std::string str = "hello";

  WATCH_ME( x );
  WATCH_ME( y );
  WATCH_ME( str );

  for ( size_t i = 0; i < 10; ++i )
  {
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    x += 10;
    y += 20;
    str += '!';
  }

  return 0;
}