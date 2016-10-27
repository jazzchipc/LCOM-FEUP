#include "i8042.h"

/** @defgroup rtc rtc
 * @{ Functions related to the rtc
 */


#ifndef _RTC_H
#define _RTC_H

#define RTC_CONTROL_REG_A 		0x0A
#define RTC_CONTROL_REG_B  		0x0B
#define RTC_CONTROL_REG_C  		0x0C
#define RTC_CONTROL_REG_D  		0x0D

#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0x71

#define RTC_ADDRESS_SECONDS 		0x00
#define RTC_ADDRESS_MINUTES			0x02
#define RTC_ADDRESS_HOURS 			0x04
#define RTC_ADDRESS_DAY_WEEK 		0x06
#define RTC_ADDRESS_DAY_MONTH		0x07
#define RTC_ADDRESS_MONTH			0x08
#define RTC_ADDRESS_YEAR			0x09

#define RTC_IRQ_LINE 8 //irq_line

/**
 * @brief Subscribes the RTC.
 * @return Bitmask that results from the subscription.
 */
int rtc_subscribe(void);

/**
 * @brief Unsubscribes the RTC.
 * @return -1 or -2 if it fails. 0 otherwise.
 */
int rtc_unsubscribe(void);

#endif
