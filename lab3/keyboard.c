#include <minix/sysutil.h>
#include "keyboard.h"
#include "kbc.h"


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

const int id = 1;

int kbc_subscribe(void)
{

	int hook_id = id;	// local variable with the value of the hook id

	if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)!= OK)	// set KBC IRQ
		return -1;

	if(sys_irqenable(&hook_id) != OK)	// enable hook_id IRQ
		return -2;

	return BIT(id);	// returns a bit mask with BIT(hook_id) set to 1
}

int kbc_unsubscribe(void)
{
	int hook_id = id;	// again a local variable, so the original remains unchanged

		if (sys_irqdisable(&hook_id) != OK)	// first disable IRQ of hook_id
			return -1;

		if (sys_irqrmpolicy(&hook_id) != OK)	// then remove it
			return -2;

		return 0;
}
