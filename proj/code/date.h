#include "minix/syslib.h"
#include "minix/driver.h"

/** @defgroup date date
 * @{ Functions related to the strcuted used to store RTC data
 */

typedef struct{
	unsigned long  year;
	unsigned long month;
	unsigned long day;


}Date;

/**
 * @brief Enables RTC interrupts.
 * @return Has no return.
 */
void rtcEnInterrupts();

/**
 * @brief Disables RTC interrupts.
 * @return Has no return.
 */
void rtcDisInterrupts();

/**
 * @brief Creates a new Data object.
 * @return The object created.
 */
Date* createDate();

/**
 * @brief Puts the information of the RTC on the Date object.
 * @param dt Date object to be changed.
 * @return Has no return.
 */
void DateModify(Date* dt);

/**
 * @brief Converts a hexadecimal number in BCD notation to decimal notation.
 * @param hex Hexadecimal to convert.
 * @return The Decimal number resulted from the conversion.
 */
static inline int bcdToDecimal(uint8_t hex);
