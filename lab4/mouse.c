#include "mouse.h"

/*COMMAND FUNCTIONS*/

unsigned long kbc_read_data()
{
	int i = 0;
	while(i < 3)
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
		i++;
	}

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
			else
				return -1;
		}

		tickdelay(micros_to_ticks(DELAY_US)); // waits for next iteration

		i++;
	}
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

int mouse_get_packet(int nByte, m_packet* originalPacket)
{
	unsigned long data = mouse_get_byte();

	if (data == -1)
		return -1;

	originalPacket->bytes[nByte] = data;

	return 0;
}

int mouse_enable_packets()
{
	unsigned long feedback;

	if (mouse_stream_mode(0) != 0)
		return -10;

	if (write_mouse_cmd(M_ENABLE_DATA_PACKETS) != 0)
		return -1;

	if (sys_inb(OUT_BUF, &feedback) != OK)
		return -2;

	if (feedback != M_ACK)
		return -3;

	if (mouse_stream_mode(1))
		return -11;

	return 0;

}

/*Print mouse packets*/
void packet_print (m_packet packet)
{
	int i = 0;
	for (i ; i < 3; i++)
	{
		printf ("Byte %d: %c\n", i, packet.bytes[i]);
	}
}

