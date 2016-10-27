#include <minix/drivers.h>
#include <minix/com.h>

#include "test5.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static long parse_long(char *str, int base);
static void print_usage(char *argv[]);


// *argc = number of arguments written.
// For example, in: service run <directory> [-args "<function> <arg1> <arg2>"]
// <directory>, <function>, <arg1> and <arg2> are considered arguments

// **argv = pointer to array of chars containing strings corresponding to arguments*/
int main(int argc, char **argv) {

	/* Initialize service */
	sef_startup();

	/* Enable IO-sensitive operations for ourselves */
	sys_enable_iop(SELF);

	if ( argc == 1 ) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;

}

/*In each line specify the way the user shall write the functions and arguments*/
/*argv[0] is the string (%s) containing the directory*/
static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"init <mode> <delay>\" \n"
	 "\t service run %s -args \"square <x> <y> <size> <color>\" \n"
	 "\t service run %s -args \"line <xi> <yi> <xf> <yf> <color>\" \n"
	 "\t service run %s -args \"xpm <xi> <yi> <*xpm[]>\" \n"
	 "\t service run %s -args \"move <xi> <yi> <*xpm[]> <hor> <delta> <time>\" \n"
	 "\t service run %s -args \"controller\" \n"
	 ,argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);
}


static int proc_args(int argc, char *argv[]) {

/*Initialization of the arguments used in each function*/
  unsigned short mode, delay, x, y, size, xi, yi, xf, yf, hor, time;
  unsigned long color;
  unsigned short xpm_id;
  short delta;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "init", strlen("init")) == 0) {	// argv[1] contains the function's name. strncmp returns 0 if argv[1] == "test_scan"
	  if( argc != 4) {
		  printf("Wrong number of arguments for test of test_init() \n");
		  return 1;
	  }

	  if ((mode = parse_ulong (argv[2], 10)) == USHRT_MAX)	// parse_ulong (string, n) converts a string to an integer with base n.
		  return 1; // If this exceeds USHRT_MAX , return error.
	  if ((delay = parse_ulong (argv[3], 10)) == USHRT_MAX)
		  return 1;

	  printf ("test_init(%hu, %hu)\n", mode, delay); // Asked function with parameter

	  test_init(mode, delay); // Return function value
	  return 0;

  } else if (strncmp(argv[1], "square", strlen("square")) == 0) {
	  if( argc != 6 ) {
		  printf("Wrong no of arguments for test of test_async() \n");
		  return 1;
	  }

	  if( (x = parse_ulong(argv[2], 10)) == USHRT_MAX) // if n exceeds ULONG_MAX
		  return 1;
	  if( (y = parse_ulong(argv[3], 10)) == USHRT_MAX)
		  return 1;
	  if( (size = parse_ulong(argv[4], 10)) == USHRT_MAX)
	  	  return 1;
	  if( (color = parse_ulong(argv[5], 10)) == ULONG_MAX)
	  	  return 1;

	  printf ("test_square(%hu, %hu, %hu, %lu)\n", x, y, size, color);

	  return test_square(x, y, size, color);

  } else if (strncmp(argv[1], "line", strlen("line")) == 0) {
	  if( argc != 7 ) {
		  printf("Wrong no of arguments for test of test_config() \n");
		  return 1;
	  }

	  	  if( (xi = parse_ulong(argv[2], 10)) == USHRT_MAX)
	  		  return 1;
	  	  if( (yi = parse_ulong(argv[3], 10)) == USHRT_MAX)
	  		  return 1;
	  	  if( (xf = parse_ulong(argv[4], 10)) == USHRT_MAX)
	  	  	  return 1;
	  	  if( (yf = parse_ulong(argv[5], 10)) == USHRT_MAX)
	  		  return 1;
	  	  if( (color = parse_ulong(argv[6], 10)) == ULONG_MAX)
	  	  	  return 1;

	  printf ("test_line(%hu, %hu, %hu, %hu, %lu)\n", xi, yi, xf, yf, color);

	  return test_line(xi, yi, xf, yf, color);

  } else if (strncmp(argv[1], "xpm", strlen("xpm")) == 0) {
	  if( argc != 5 ) {
		  printf("Wrong no of arguments for test of test_xpm() \n");
		  return 1;
	  }

	  if( (xi = parse_ulong(argv[2], 10)) == USHRT_MAX)
		  return 1;

	  if( (yi = parse_ulong(argv[3], 10)) == USHRT_MAX)
		  return 1;

	  if( (xpm_id = parse_ulong(argv[4], 10)) == USHRT_MAX)
		  return 1;

	  printf ("test_xpm(%hu, %hu, %hu)\n", xi, yi, xpm_id);

	  return test_xpm (xi, yi, pixmap_id(xpm_id));

	  return 0;

}	else if (strncmp(argv[1], "move", strlen("move")) == 0) {
	  if( argc != 8 ) {
		  printf("Wrong no of arguments for test of test_move() \n");
		  return 1;
	  }

	  if( (xi = parse_ulong(argv[2], 10)) == USHRT_MAX)
	  		  return 1;

	  if( (yi = parse_ulong(argv[3], 10)) == USHRT_MAX)
	  		  return 1;

	  if( (xpm_id = parse_ulong(argv[4], 10)) == USHRT_MAX)
	  		  return 1;

	  if( (hor = parse_ulong(argv[5], 10)) == USHRT_MAX)
		  return 1;

	  if( (delta = parse_ulong(argv[6], 10)) == SHRT_MAX)
		  return 1;

	  if( (time = parse_ulong(argv[7], 10)) == CHAR_MAX)
		  return 1;

	  printf ("test_move(%hu, %hu, %hu, %hu, %h, %hu)\n", xi, yi, xpm_id, hor, delta, time);

	  return test_move(xi, yi, pixmap_id(xpm_id), hor, delta, time);


}	else if (strncmp(argv[1], "controller", strlen("controller")) == 0) {
	  if( argc != 2 ) {
		  printf("Wrong no of arguments for test of test_move() \n");
		  return 1;
	  }

	  printf ("test_controller()\n");

	  return test_controller();

} 	else // if no function corresponds
  {
	  printf ("Invalid function \n"); return 1;
  }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("mouse: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}

static long parse_long(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtol(str, &endptr, base);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return LONG_MAX;
  }

  if (endptr == str) {
	  printf("mouse: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
