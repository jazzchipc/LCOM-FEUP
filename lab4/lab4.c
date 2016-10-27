#include <minix/drivers.h>
#include <minix/com.h>

#include "test4.h"

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
	 "\t service run %s -args \"packet <cnt>\" \n"
	 "\t service run %s -args \"async <idle_time> \" \n"
	 "\t service run %s -args \"config\" \n"
	 "\t service run %s -args \"gesture <length> <tolerance>\" \n"
	 ,argv[0], argv[0], argv[0]);
}


static int proc_args(int argc, char *argv[]) {

/*Initialization of the arguments used in each function*/
  unsigned short cnt, idle_time, tolerance;
  short length;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "packet", strlen("packet")) == 0) {	// argv[1] contains the function's name. strncmp returns 0 if argv[1] == "test_scan"
	  if( argc != 3 ) {
		  printf("Wrong number of arguments for test of test_packet() \n");
		  return 1;
	  }

	  if ((cnt = parse_ulong (argv[3], 10)) == USHRT_MAX)	// parse_ulong (string, n) converts a string to an integer with base n.
		  return 1; // If this exceeds USHRT_MAX , return error.

	  printf ("test_packet(%hu)\n", cnt); // Asked function with parameter

	  return test_packet(cnt); // Return function value

  } else if (strncmp(argv[1], "async", strlen("async")) == 0) {
	  if( argc != 3 ) {
		  printf("Wrong no of arguments for test of test_async() \n");
		  return 1;
	  }

	  if( (idle_time = parse_ulong(argv[2], 10)) == USHRT_MAX) // if n exceeds ULONG_MAX
		  return 1;

	  printf ("test_async()\n");

	  return test_async(idle_time);

  } else if (strncmp(argv[1], "config", strlen("config")) == 0) {
	  if( argc != 2 ) {
		  printf("Wrong no of arguments for test of test_config() \n");
		  return 1;
	  }

	  printf ("test_config()\n");

	  return test_config();

  } else if (strncmp(argv[1], "gesture", strlen("gesture")) == 0) {
	  if( argc != 4 ) {
		  printf("Wrong no of arguments for test of test_gesture() \n");
		  return 1;
	  }

	  if( (length = parse_ulong(argv[2], 10)) == SHRT_MAX) // if n exceeds SHRT_MAX
		  return 1;

	  if( (tolerance = parse_ulong(argv[2], 10)) == USHRT_MAX) // if n exceeds SHRT_MAX
	  		  return 1;

	  printf ("test_gesture(%h, %hu)\n", length, tolerance);

	  return test_gesture(length, tolerance);
}

  else // if no function corresponds
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
