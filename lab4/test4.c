#include "mouse.h"
m_packet* packet;

#define id 1

int hook_id = id;

/*Declared variable in mouse.h to link id's between subscribe and unsubscribe*/
int mouse_subscribe(void)
{
	if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)!= OK)	// set KBC IRQ
		return -1;

	if(sys_irqenable(&hook_id) != OK)	// enable hook_id IRQ
		return -2;


	return BIT(id);	// returns a bit mask with BIT(hook_id) set to 1
}

int mouse_unsubscribe(void)
{
		if (sys_irqdisable(&hook_id) != OK)	// first disable IRQ of hook_id
			return -1;

		if (sys_irqrmpolicy(&hook_id) != OK)	// then remove it
			return -2;

		return 0;
}


int test_packet(unsigned short cnt)
{

	/*First we need to subscribe the mouse interrupts*/
	int irq_set = mouse_subscribe();
	mouse_enable_packets();

	packet = (m_packet*) malloc (sizeof (m_packet));

	/*Then a cycle to detect interruptions aimed for the mouse*/

	int ipc_status;
	message msg;
	int r;

	int i = 0;

	int error_get_packet;

	while (i < (cnt*3))
	{
		unsigned long data;
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
					packet->bytes[i % 3] = mouse_get_byte();
					packet_print (*packet);
					printf ("RATO\n");
					i++;
				}

				break;

			default: {
				printf("nao deu\n");
				i++;
				break;}

			}
		}

	}

	printf ("out of loop\n");

	int a = mouse_stream_mode (0);
	printf ("%d\n", a);

	int j = mouse_unsubscribe();
	printf ("%d\n", j);

	return 0;

}

int test_async(unsigned short idle_time) {
    /* To be completed ... */
}

int test_config(void) {
    /* To be completed ... */
}

int test_gesture(short length, unsigned short tolerance) {
    /* To be completed ... */
}
