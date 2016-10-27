#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/syslib.h>
#include <minix/drivers.h>

/*Subscribe KBC's IRQ and returns a bit mask with BIT(hook_id) set to 1*/
int kbc_subscribe(void);

/*Unsubscribes KBC's IRQ*/
int kbc_unsubscribe(void);

/*Loop that receives key input and returns the code of the pressed key*/
unsigned long kbc_read_key();

/*Returns 1 if data is a breakcode. 0 if it's a makecode*/
int is_breakcode(unsigned long key);

/*Prints the key code and returns value of scancode*/
int print_code(unsigned long key);

/*Returns 1 if the key is a two-byte key*/
int is_two_byte_code (unsigned long key);

#endif


