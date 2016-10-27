#include <minix/drivers.h>


int main()
{


  /* Initialize service */

  sef_startup();

  timer_test_config(0);
  timer_test_square(60);
  timer_test_int(5);

  return 0;

}

