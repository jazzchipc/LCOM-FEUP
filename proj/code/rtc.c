#include "rtc.h"
#include "minix/driver.h"

int hook_id;
/*
 * Subscribes rtc mode
 */
int rtc_subscribe(void)
{

	hook_id = RTC_HOOK_ID;	// local variable with the value of the hook id

	if(sys_irqsetpolicy(RTC_IRQ_LINE, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id)!= OK)	// set KBC IRQ
		return -1;

	if(sys_irqenable(&hook_id) != OK)	// enable hook_id IRQ
		return -2;

	return BIT(RTC_HOOK_ID);	// returns a bit mask with BIT(hook_id) set to 1
}
/*
 * unsubscribes rtc mode
 */
int rtc_unsubscribe(void)
{
	if (sys_irqdisable(&hook_id) != OK)	// first disable IRQ of hook_id
		return -1;

	if (sys_irqrmpolicy(&hook_id) != OK)	// then remove it
		return -2;

	return 0;
}



