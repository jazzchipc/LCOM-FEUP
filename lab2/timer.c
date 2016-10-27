#include <minix/syslib.h>
#include <minix/drivers.h>

#include "timer.h"
#include "i8254.h"

#define MAX_VALUE 65535 // Max value for 16 bit (unsigned int) sized variable,
						// which is the maximum value the counter can hold

unsigned long int_counter = 0; // interrupt counter
const int id = 10;	// arbitrary hook_id


/*Converts and prints a byte sized decimal number to binary notation*/
void decimal_to_binary(unsigned char n)
{
	int bin;

	int i = 7;

	for ( ; i >= 0; --i)
	{
		int bin = n >> i;

		if (bin & 1)
		{printf ("1");}

		else
		{printf ("0");}
	}

}

int timer_set_square(unsigned long timer, unsigned long freq) {

	if (freq <= 0)
	{
		printf ("\nfrequency can't be null or negative");
		return -1; // invalid frequency (negative or null)
	}
    unsigned long div = TIMER_FREQ / freq;

    unsigned char lsb = (char) (div & 0xFF);	// lsb gets the 4 less significant bits
    unsigned char msb = (char) ((div >> 8) & 0xFF);

    if (div >= MAX_VALUE || freq > TIMER_FREQ) // so that div isn't below 1 or above max value, resulting in overflow
    {
    	printf ("\ninvalid frequency");
    	return -2; // the counter can only load a max value of 65535 (2 bytes)
    }


    char CMD;	// command for control register

    switch (timer) // switch for command to each timer
    {
    case 0: CMD = TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_BIN | TIMER_SEL0; break;
    case 1: CMD = TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_BIN | TIMER_SEL1; break;
    case 2: CMD = TIMER_SQR_WAVE | TIMER_LSB_MSB | TIMER_BIN | TIMER_SEL2; break;

    default: printf("\ninvalid timer"); break;
    }

    unsigned char st; // byte where the configuration of the timer will be stored


    if (sys_outb(TIMER_CTRL, CMD) != OK)	// send command to control

    {
            printf ("\nsys_outb error");
            return -3;
    }

    if (sys_outb(TIMER_0 + timer, lsb) != 0)	// send lsb to timer

    {
            printf ("\nsys_outb error");
            return -4;
    }

    if (sys_outb(TIMER_0 + timer, msb) != 0)	// send msb to timer

    {

            printf ("\nsys_outb error");
            return -5;
    }

    return 0;

}

int timer_subscribe_int(void) {

	int hook_id = id;	// local variable with the value of the hook id

	if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)!= OK)	// set timer IRQ policy and latch hook_id to it
		return -1;

	if(sys_irqenable(&hook_id) != OK)	// enable hook_id IRQ
		return -2;

	return BIT(id);	// returns a bit mask with BIT(hook_id) set to 1
}

int timer_unsubscribe_int() {

	int hook_id = id;	// again a local variable, so the original remains unchanged

	if (sys_irqdisable(&hook_id) != OK)	// first disable IRQ of hook_id
		return -1;

	if (sys_irqrmpolicy(&hook_id) != OK)	// then remove it
		return -2;

	return 0;
}

void timer_int_handler() {

	int_counter ++;	// increases interruption counter
}

/*Stores the configuration of selected timer in pointer*/
int timer_get_conf(unsigned long timer, unsigned char *st)
{
	char CMD = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_; // command for Control Register

	if (sys_outb(TIMER_CTRL, CMD) != 0) // if sys_outb returns an error
	{
		return -1;
	}

	unsigned long stLong;	// because sys_inb functions need an unsigned long variable

	if (sys_inb(TIMER_0 + timer, &stLong) != 0) // if sys_inb returns an error
	{
		return -2;
	}

	/* &stLong receives the address where the Timer status is.
	 * stLong value is the status itself.
	 * The status is stored in the address pointed by st*/

	*st = stLong;

	return 0;
}

int timer_display_conf(unsigned char conf)
{
	unsigned char bin = conf; // bin will be the counter status byte written in binary

	printf ("\nStatus: ");
	decimal_to_binary(conf);	// prints binary status, to confirm the function works
	printf ("\n");

	int output = (conf >> 7) & (BIT(0));
	printf ("\nOutput = %d\n", output);	// prints the Output value

	int nullCount = (conf >> 6) & (BIT(0));
	printf ("\nNull Count = %d\n", nullCount);	// prints the Null Count value


	int typeOfAccess = (conf >> 4) & (BIT(1) | BIT(0));

	switch (typeOfAccess)
	{
	case 0:(printf("\nCounter Latch Command\n")); break;

	case 1:(printf("\nType Of Access: LSB\n")); break;

	case 2:(printf("\nType Of Access: MSB\n")); break;

	case 3:(printf("\nType Of Access: LSB followed by MSB\n")); break;

	default: return -1;
	}


	int operatingMode = (conf >> 1) & (BIT(2) | BIT(1) | BIT(0));
	switch (operatingMode) {
	case 0 :(printf("\nOperating Mode : 0\n"));break;
	case 1 :(printf("\nOperating Mode : 1\n"));break;
	case 2 :(printf("\nOperating Mode : 2\n"));break;
	case 3 :(printf("\nOperating Mode : 3\n"));break;
	case 4 :(printf("\nOperating Mode : 4\n"));break;
	case 5 :(printf("\nOperating Mode : 5\n"));break;
	case 6 :(printf("\nOperating Mode : 2\n"));break;
	case 7 :(printf("\nOperating Mode : 3\n"));break;
	default: return -2; break;
	}

	int countingMode = conf & (BIT(0));
	if (countingMode == 0) {
		printf("\nCounting Mode : Binary\n");
	}
	if (countingMode == 1) {
		printf("\nCounting Mode : BCD\n");
	}

	return 0;
}

int timer_test_square(unsigned long freq) {

    if (timer_set_square(0, freq))
    	return -1;

    return 0;
}

int timer_test_int(unsigned long time)
{
	int ipc_status;
	message msg;
	int r;

	int irq_set = timer_subscribe_int();

	while (int_counter < 60*time) //freq * time
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf ("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{
					if (int_counter % 60 == 0)
						printf ("A second has passed.\n");
					timer_int_handler();
				}

				break;

			default: break;

			}
		}

	}

	timer_unsubscribe_int();

	return 0;
}

int timer_test_config(unsigned long timer) {

	unsigned char st;

	printf ("\n***TIMER TEST CONFIG***\n");

	printf ("\nSelected timer: %d\n", timer);

	if (timer_get_conf (timer, &st))
		return -1;

	if (timer_display_conf(st))
		return -2;

	printf ("\n---------------------------------\n");

	return 0;
}
