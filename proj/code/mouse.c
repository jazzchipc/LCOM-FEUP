#include "mouse.h"
#include "video_gr.h"

#define IS_BIT_SET(n, bit)	(((n) & BIT(bit)) ? 1 : 0)

unsigned char info[M_PACKET_SIZE];	// Array that'll store the 3 bytes sent by the mouse
int currentByte;	// Tells which byte of the "packet" array to analyze next
m_packet* packet;	// Mouse packet that'll store changes to the mouse position/input

/***********************
 * CROSSHAIR FUNCTIONS *
 ***********************/

Crosshair* newCrosshair()
{
	Crosshair* c = (Crosshair*)malloc(sizeof(Crosshair*));

	m_packet* p = newMousePacket();

	c->packet = p;

	Bitmap* bmp = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/Crosshair.bmp");
	Sprite* s = createSprite(bmp ,512, 388, RED);

	c->sprite = s;

	return c;
}

void updateCrosshair(Crosshair* c, m_packet* p)
{
	c->packet = p;

	c->sprite->xi = c->sprite->xi + c->packet->xDelta;
	c->sprite->yi = c->sprite->yi - c->packet->yDelta;	// - sign, (0,0) for mouse is bot left, not top left

	c->leftClick = mouseClicksLeft(c->packet);
	c->rightClick = mouseClicksRight(c->packet);

	//Limiting crosshair to screen
	if (c->sprite->xi > 1024)
		c->sprite->xi = 1024;

	if (c->sprite->xi < 0)
		c->sprite->xi = 0;

	if (c->sprite->xi > 768)
		c->sprite->xi= 768;

	if (c->sprite->yi < 0)
		c->sprite->yi = 0;
}

void destroyCrosshair(Crosshair* c)
{
	destroyMousePacket(c->packet);

	free(c);
}

m_packet* newMousePacket()
{
	m_packet* packet = (m_packet*)malloc(sizeof(m_packet));

	return packet;
}

void destroyMousePacket(m_packet* packet)
{
	free(packet);
}


/*Subscribing functions*/

int hook_id = MOUSE_HOOK_ID;

int mouse_subscribe()
{
	if(sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)!= OK)	// set KBC IRQ
		return -1;

	if(sys_irqenable(&hook_id) != OK)	// enable hook_id IRQ
		return -2;

	return BIT(MOUSE_HOOK_ID);	// returns a bit mask with BIT(hook_id) set to 1
}

int mouse_unsubscribe()
{
	if (sys_irqdisable(&hook_id) != OK)	// first disable IRQ of hook_id
		return -1;

	if (sys_irqrmpolicy(&hook_id) != OK)	// then remove it
		return -2;

	return 0;
}



/*COMMAND FUNCTIONS*/

unsigned long kbc_read_data()
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

int write_kbc_cmd(unsigned long kbc_port, unsigned long CMD)
{
	int i = 0;

	while(i < 3)
	{
		unsigned long stat;	// will store KBC status
		unsigned long data;	// will return Make/Break code from KBC

		sys_inb(STAT_REG, &stat); /* assuming it returns OK */

		int IBF = BIT(1) & stat;

		/* loop while 8042 output buffer is empty */
		if((stat & IBF) == 0)
		{
			if(sys_outb(kbc_port, CMD) == OK)
				return 0;/* assuming it returns OK */
		}

		tickdelay(micros_to_ticks(DELAY_US)); // waits for next iteration

		i++;
	}

	return -1;
}


int write_mouse_cmd(unsigned char CMD)
{
	if (write_kbc_cmd(KBC_CMD_REG,M_WRITE_BYTE) != 0)
		return -1;

	if (write_kbc_cmd(OUT_BUF, CMD) != 0)
		return -2;

	return 0;
}

int mouse_stream_mode (int mode)
{
	int return_value;
	unsigned long feedback;

	if (mode == 0)
	{
		if(write_mouse_cmd(M_DISABLE_STREAM_MODE))
			return -1;
	}

	else
	{
		if (write_mouse_cmd(M_SET_STREAM_MODE))
			return -2;
	}

	return_value = sys_inb(OUT_BUF, &feedback);

	if (return_value != OK)
		return -3;


	if (kbc_read_data() != M_ACK)
		return -4;

	return 0;
}

/*Get data from mouse*/
unsigned long mouse_get_byte()
{
	unsigned long data = kbc_read_data();

	if (data == -1)
		return -1;
	else
		return data;
}

int mouse_set_packet(m_packet* originalPacket, unsigned char* info)
{
	originalPacket->bytes[0] = info[0];
	originalPacket->bytes[1] = info[1];
	originalPacket->bytes[2] = info[2];

	originalPacket->xOF = IS_BIT_SET(originalPacket->bytes[0], M_1B_X_OF_BIT);
	originalPacket->yOF = IS_BIT_SET(originalPacket->bytes[0], M_1B_Y_OF_BIT);
	originalPacket->lb = IS_BIT_SET(originalPacket->bytes[0], M_1B_LEFT_BTN_BIT);
	originalPacket->mb = IS_BIT_SET(originalPacket->bytes[0], M_1B_MIDDLE_BTN_BIT);
	originalPacket->rb = IS_BIT_SET(originalPacket->bytes[0], M_1B_RIGHT_BTN_BIT);

	if (originalPacket->xOF)
	{
		if (originalPacket->bytes[0] & BIT(M_1B_X_SIGN_BIT))
		{
			originalPacket->xDelta = (1 << 8) - 1;
		}
		else
		{
			originalPacket->xDelta = (-1 << 8) + 1;
		}
	}
	else
	{
		originalPacket->xDelta = IS_BIT_SET(originalPacket->bytes[0], M_1B_X_SIGN_BIT) ? ((-1 << 8) | originalPacket->bytes[1]) : originalPacket->bytes[1];
	}

	if (originalPacket->yOF)
	{
		if (originalPacket->bytes[0] & BIT(M_1B_Y_SIGN_BIT))
		{
			originalPacket->yDelta = (1 << 8) - 1;
		}
		else
		{
			originalPacket->yDelta = (-1 << 8) + 1;
		}
	}
	else
	{
		originalPacket->yDelta = IS_BIT_SET(originalPacket->bytes[0], M_1B_Y_SIGN_BIT) ? ((-1 << 8) | originalPacket->bytes[2]) : originalPacket->bytes[2];
	}

	return 0;
}

void mouse_get_packet(m_packet* mouse, int counter)
{
	info[counter%3] = mouse_get_byte();

	if (counter % 3 == 0)
	{
		if (!(info[counter%3] & BIT(3)))	//to sync
			return;

		mouse_set_packet(packet, info);
		mouse = packet;
	}

}

int mouse_enable_packets()
{
	unsigned long feedback;

	if (write_mouse_cmd(M_ENABLE_DATA_PACKETS) != 0)
		return -1;

	if (sys_inb(OUT_BUF, &feedback) != OK)
		return -2;

	if (feedback != M_ACK)
		return -3;

	return 0;

}

/*Print mouse packets. For debug.*/
void packet_print (m_packet* packet)
{
	printf("B1=0x%X\t", packet->bytes[0]);
	printf("B2=0x%X\t", packet->bytes[1]);
	printf("B3=0x%X\t", packet->bytes[2]);
	printf("LB=%d\t", packet->lb);
	printf("MB=%d\t", packet->mb);
	printf("RB=%d\t", packet->rb);
	printf("XOV=%d\t", packet->xOF);
	printf("YOV=%d\t", packet->yOF);
	printf("X=%d\t", packet->xDelta);
	printf("Y=%d", packet->yDelta);
	printf("\n");
	return;
}



/*Test functions*/
int test_packet(unsigned short cnt)
{

	/*First we need to subscribe the mouse interrupts*/

	int irq_set = mouse_subscribe();

	mouse_stream_mode(1);

	mouse_enable_packets();

	packet = (m_packet*) malloc (sizeof (m_packet));
	Crosshair* c = newCrosshair();

	/*Then a cycle to detect interruptions aimed for the mouse*/

	int ipc_status;
	message msg;
	int r;

	int i = 0;

	int error_get_packet;

	while (i < cnt * 3)
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
					//packet->bytes[i % 3] = mouse_get_byte();
					//packet_print (*packet);

					info[i%3] = mouse_get_byte();

					if (i % 3 == 0)
					{
						if (!(info[i%3] & BIT(3)))	//to sync
							continue;

						mouse_set_packet(packet, info);
						packet_print(packet);
						updateCrosshair(c, packet);
						printf("%d %d", c->sprite->xi, c->sprite->yi);
					}

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


	destroyCrosshair(c);


	int a = mouse_stream_mode(0);
	printf ("%d\n", a);

	int j = mouse_unsubscribe();
	printf ("%d\n", j);


	return 0;

}


/*Mouse movement functions*/

int mouseIsStill(m_packet* m)
{
	return (m->xDelta == 0 && m->yDelta == 0);
}

int mouseMovesUp(m_packet* m)
{
	return (m->yDelta > 0);
}

int mouseMovesDown(m_packet* m)
{
	return (m->yDelta < 0);
}

int mouseMovesLeft(m_packet* m)
{
	return (m->xDelta < 0);
}

int mouseMovesRight(m_packet* m)
{
	return (m->xDelta > 0);
}

int mouseClicksLeft(m_packet* m)
{
	return (m->lb);
}

int mouseClicksRight(m_packet* m)
{
	return (m->rb);
}
