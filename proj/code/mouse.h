#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/syslib.h>
#include <minix/sysutil.h>
#include <minix/drivers.h>

#include "i8042.h"
#include "sprite.h"

#define TRIES 15

/*Mouse commands*/

#define M_RESET					0xFF
#define M_RESEND				0xFE
#define M_SET_DEFAULT			0xF6
#define M_DISABLE_STREAM_MODE	0xF5 /*"If the device is in Stream mode (the default) and has been enabled with an Enable (0xF4) command,
 	 	 	 	 	 	 	 	 	 then the host should disable the device with a Disable (0xF5) command before sending any other command."*/
#define M_ENABLE_DATA_PACKETS	0xF4
#define M_SET_SAMPLE_RATE		0xF3
#define M_SET_REMOTE_MODE		0xF0
#define M_READ_DATA				0xEB
#define M_SET_STREAM_MODE		0xEA
#define M_STATUS_REQUEST		0xE9
#define M_SET_RESOLUTION		0xE8
#define M_SET_SCALING_2_1		0xE7
#define M_SET_SCALING_1_1		0xE6

#define M_WRITE_BYTE 			0xD4

/* M_WRITE_BYTE must be sent to KBC_CMD_REG and then the mouse command is sent to WRT_CMD_BYTE
 * This must be done for EVERY BYTE of information going and coming.*/

/*Mouse controller returns*/

#define M_ACK	0xFA
#define M_NACK	0xFE
#define M_ERROR	0xFC

/*The acknowledgment is read from port 0x60*/

/*"When the host gets an 0xFE response, it should retry the offending command.
 *If an argument byte elicits an 0xFE response, the host should retransmit the entire command, not just the argument byte."*/

/*Packet size*/

#define M_PACKET_SIZE	3

/*Mouse movement data packet*/

#define M_1B_Y_OF_BIT			7
#define M_1B_X_OF_BIT			6
#define M_1B_Y_SIGN_BIT			5
#define M_1B_X_SIGN_BIT			4
#define M_1B_ALWAYS_1_BIT		3
#define M_1B_MIDDLE_BTN_BIT		2
#define M_1B_RIGHT_BTN_BIT		1
#define M_1B_LEFT_BTN_BIT		0

/*STRUCTS*/

typedef struct
{
	unsigned char bytes[3];
	char lb; //left button
	char rb;
	char mb;
	char bit3;
	char xSign;
	char ySign;
	char xOF;
	char yOF;

	char xDelta;
	char yDelta;
}m_packet;

m_packet* newMousePacket();

void destroyMousePacket();

/*FUNCTIONS TO WRITE/READ COMMANDS*/

int mouse_write(unsigned char CMD);

/*FUNCTIONS*/

#define IS_1(n, bit)	(((n) & BIT(bit)) ? 1 : 0)

int mouse_subscribe();

int mouse_unsubscribe();

unsigned long kbc_read_data();

int write_kbc_cmd(unsigned long kbc_port, unsigned long CMD);

int write_mouse_cmd(unsigned char CMD);

int mouse_stream_mode (int mode);

unsigned long mouse_get_byte();

int mouse_enable_packets();

int mouse_set_packet(m_packet* originalPacket, unsigned char* info);

void mouse_get_packet(m_packet* mouse, int counter);

void packet_print (m_packet* packet);

/*Check if the 3rd bit of the first byte (array[0]) of the packet is 1*/

int test_packet(unsigned short cnt);

//int mouse_sync();

/*Mouse movement functions*/

int mouseIsStill(m_packet* m);
int mouseMovesUp(m_packet* m);
int mouseMovesDown(m_packet* m);
int mouseMovesLeft(m_packet* m);
int	mouseMovesRight(m_packet* m);
int mouseClicksLeft(m_packet* m);
int mouseClicksRight(m_packet* m);

/*CROSSHAIR TO BE USED ON QUEST*/
typedef struct
{
	m_packet* packet;

	int leftClick;
	int rightClick;

	Sprite* sprite;
}Crosshair;

Crosshair* newCrosshair();

void updateCrosshair(Crosshair* c, m_packet* p);

void destroyCrosshair(Crosshair* c);


#endif /*__MOUSE_H*/
