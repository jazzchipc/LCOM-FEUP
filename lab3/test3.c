#include "test3.h"
#include "keyboard.h"


extern char read_key_asm();
extern char key_asm;

int kbd_test_scan(unsigned short ass) {

	/*First we need to subscribe the KBC interrupts*/
	int irq_set = kbc_subscribe();

	/*Then a cycle to detect interruptions aimed for the keyboard*/

	int ipc_status;
	message msg;
	int r;

	unsigned long key;

	while (key != 0x81)
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
						if (ass == 0)
						{
							key = kbc_read_key();	// use C program
							print_code(key);
						}

						else
						{
							read_key_asm();
							key = key_asm;
							key = (key & 0xFF); // so only the two last bytes are considered
							print_code(key);
						}
					}

					break;

				default: break;

				}
			}

		}

	return 0;

}
int kbd_test_leds(unsigned short n, unsigned short *leds) {
    /* To be completed */
}
int kbd_test_timed_scan(unsigned short n) {
    /* To be completed */
}
