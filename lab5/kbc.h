#ifndef __KBC_H
#define __KBC_H



#define DELAY_US    20000	// delay need to wait for next input

#define KBC_IRQ 1   // KBC's IRQ is 1

#define OUT_BUF 0x60		// keyboard's IC output port

#define STAT_REG 	0x64	// status register
#define KBC_CMD_REG 0x64	// commands to the KBC

#define BIT(n) (0x01<<(n))

/*Definition of commands for the KBC*/

#define READ_CMD_BYTE 	0x20	// returns command byte
#define WRT_CMD_BYTE  	0x60	//

#define CHECK_KBC		0xAA 	// returns 0x55, if OK; 0xFC, if error

#define CHECK_KBC1 		0xAB	// returns 0 if OK

#define DISABLE_KBD		0xAD
#define ENABLE_KBD		0xAE

/*Definition of status bits*/




























#endif
