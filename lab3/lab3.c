#include <minix/drivers.h>
#include <minix/com.h>

#include "test3.h"

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
	 "\t service run %s -args \"test_scan <ass>\" \n"
	 "\t service run %s -args \"test_leds <n> <*leds>\" \n"
	 "\t service run %s -args \"test_timed_scan <n>\" \n"
	 ,argv[0], argv[0], argv[0]);
}


static int proc_args(int argc, char *argv[]) {

/*Initialization of the arguments used in each function*/
  unsigned short ass, n;
  unsigned short* leds;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "test_scan", strlen("test_scan")) == 0) {	// argv[1] contains the function's name. strncmp returns 0 if argv[1] == "test_scan"
	  if( argc != 3 ) {
		  printf("Wrong number of arguments for test of kbd_test_scan() \n");
		  return 1;
	  }

	  if ((ass = parse_ulong (argv[2], 10)) == USHRT_MAX)	// parse_ulong (string, n) converts a string to an integer with base n.
		  return 1; // If this exceeds USHRT_MAX , return error.

	  printf ("kbd_test_scan(%hu)\n", ass); // Asked function with parameter

	  return kbd_test_scan(ass); // Return function value

  } else if (strncmp(argv[1], "test_leds", strlen("test_leds")) == 0) {
	  if( argc != 4 ) {
		  printf("Wrong no of arguments for test of kbd_test_leds() \n");
		  return 1;
	  }

	  if( (n = parse_ulong(argv[2], 10)) == USHRT_MAX) // if n exceeds ULONG_MAX
		  return 1;

	  if ((leds = malloc(n * sizeof(char))) == NULL)	// if leds is empty
		  return 1;

	  printf ("kbd_test_leds()\n");

	  return kbd_test_leds(n, leds);

  } else if (strncmp(argv[1], "test_timed_scan", strlen("test_timed_scan")) == 0) {
	  if( argc != 3 ) {
		  printf("Wrong no of arguments for test of kbd_test_timed_scan() \n");
		  return 1;
	  }
	  if( (n = parse_ulong(argv[2], 10)) == USHRT_MAX )
		  return 1;

	  printf ("kbd_test_timed_scan (%hu)\n", n);

	  return kbd_test_timed_scan(n);

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
	  printf("kbd: parse_ulong: no digits were found in %s \n", str);
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
	  printf("kbd: parse_long: no digits were found in %s \n", str);
	  return LONG_MAX;
  }

  /* Successful conversion */
  return val;
}
