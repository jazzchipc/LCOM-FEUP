#include <minix/sysutil.h>
#include "keyboard.h"

/**
 * Create variable "key" to be update everytime this executes
 * irq_set = kbc_subscribe();
 * Make a loop out of it
 */

int kbd_get_input(int irq_set, unsigned long* p)
{
	/*Condition to detect interruptions aimed for the keyboard*/

	int ipc_status;
	message msg;
	int r;


	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
	{
		printf ("driver_receive failed with: %d", r);
	}

	if (is_ipc_notify(ipc_status))
	{
		switch (_ENDPOINT_P(msg.m_source))
		{
		case HARDWARE:
			if (msg.NOTIFY_ARG & irq_set)
			{

				*p = kbc_read_key();

			}

			break;

		default: break;

		}
	}

	return 0;
}
/*
 * reads keyboard key
 */
unsigned long kbc_read_key()

{
	while( 1 )
	{
		unsigned long stat;	// will store KBC status
		unsigned long data;	// will return Make/Break code from KBC

		sys_inb(STAT_REG, &stat); /* assuming it returns OK */

		int OBF = BIT(0) & stat;
		int PAR_ERR = BIT(0) & (stat >> 7);
		int TO_ERR = BIT(0) & (stat >> 6);

		/* loop while 8042 output buffer is empty */
		if( stat & OBF ) {
			sys_inb(OUT_BUF, &data); /* assuming it returns OK */
			if ( (stat &(PAR_ERR | TO_ERR)) == 0 )
				return data;
			else
				return -1;
		}

		tickdelay(micros_to_ticks(DELAY_US)); // waits for next iteration
	}

}

int is_breakcode(unsigned long key)
{
	return (key & BIT(7));
}

int is_two_byte_code (unsigned long key)
{
	return (key == 0xE0);
}

int two_byte_code = 0; // "bool" variable
int two_byte_code_msb = 0xE0; // msb of most two byte codes

int print_code(unsigned long key)
{
	int breakcode = 0;
	unsigned int scancode; // will have 1 or 2 bytes

	if (is_two_byte_code(key))
	{
		two_byte_code = 1;
		return 0;	// will execute cycle again to receive second byte
	}

	else
		breakcode = (is_breakcode(key));

	if (two_byte_code)
	{
		scancode = (two_byte_code_msb << 8) + key;

		if (breakcode)
			printf ("Breakcode: 0x%X\n", scancode);
		else
			printf ("Makecode: 0x%X\n", scancode);

		two_byte_code = 0; // resets the key back to a default one byte key
	}
	else
	{
		scancode = key;

		if (breakcode)
			printf ("Breakcode: 0x%X\n", scancode);
		else
			printf ("Makecode: 0x%X\n", scancode);
	}

	return scancode;

}

int hook_id;

int kbc_subscribe(void)
{

	hook_id = KBD_HOOK_ID;	// local variable with the value of the hook id

	if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)!= OK)	// set KBC IRQ
		return -1;

	if(sys_irqenable(&hook_id) != OK)	// enable hook_id IRQ
		return -2;

	return BIT(KBD_HOOK_ID);	// returns a bit mask with BIT(hook_id) set to 1
}

int kbc_unsubscribe(void)
{
	if (sys_irqdisable(&hook_id) != OK)	// first disable IRQ of hook_id
		return -1;

	if (sys_irqrmpolicy(&hook_id) != OK)	// then remove it
		return -2;

	return 0;
}
