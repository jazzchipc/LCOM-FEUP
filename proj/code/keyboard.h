#ifndef __KEYBOARD_H
#define __KEYBOARD_H

/** @defgroup keyboard keyboard
 * @{ Functions related to the keyboard/KBC input
 */

#include <minix/syslib.h>
#include <minix/drivers.h>

#include "i8042.h"

#define ESC_KEY 0x81

//WASD Keys

//Makecodes
#define KEY_W	0x0011
#define KEY_A 	0x001E
#define KEY_D	0x0020
#define KEY_S	0x001F

//Breakcodes
#define BREAK_KEY_W (KEY_W + 0x80)
#define BREAK_KEY_A (KEY_A + 0x80)
#define BREAK_KEY_D (KEY_D + 0x80)
#define BREAK_KEY_S (KEY_S + 0x80)

//Arrow Keys

#define KEY_ARROW_UP	0xE048
#define KEY_ARROW_DOWN 	0xE050

/**
 * @brief Subscribe KBC's IRQ and returns a bit mask with BIT(hook_id) set to 1
 * @return Bitmask created from subscribe.
 */
int kbc_subscribe(void);

/**
 * @brief Unsubscribes KBC's IRQ
 * @return -1 or -2 if it fails. 0 otherwise.
 */
int kbc_unsubscribe(void);

/**
 * @brief For every interrupt on the keyboard, it'll refresh "key" variable
 * @param irq_set IRQ_SET resulting from the subscribe function.
 * @param key Variable that'll store the data sent by the KBC.
 * @return 0 if it reaches the end.
 */
int kbd_get_input(int irq_set, unsigned long* key);

/**
 * @brief Loop that receives key input and returns the code of the pressed key
 * @return Value of the key pressed.
 * */
unsigned long kbc_read_key();

/**
 * @brief Returns 1 if data is a breakcode. 0 if it's a makecode*/
int is_breakcode(unsigned long key);

/**
 * @brief Prints the key code and returns value of scancode*/
int print_code(unsigned long key);

/*
 * @brief Returns 1 if the key is a two-byte key*/
int is_two_byte_code (unsigned long key);

#endif


