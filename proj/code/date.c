#include "rtc.h"
#include "date.h"
/*
 * enables interrupts
 */
void rtcEnInterrupts()
{
	asm("sti");
}

/*
 * Disables Interrupts
 */
void rtcDisInterrupts()
{
	asm("cli");
}

Date* createDate(){
	Date* data;
	data = (Date*) malloc(sizeof(Date));
	DateModify(data);
	return data;
}

static inline int bcdToDecimal(uint8_t hex)
{

    int dec = ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
    return dec;
}

void DateModify(Date* dt){

	rtcDisInterrupts();

	unsigned long y;
	unsigned long m;
	unsigned long d;

	unsigned long res;



	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_DAY_MONTH);
	sys_inb(RTC_DATA_REG, &d);


	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_MONTH);
	sys_inb(RTC_DATA_REG, &m);


	sys_outb(RTC_ADDR_REG, RTC_ADDRESS_YEAR);
	sys_inb(RTC_DATA_REG, &y);



	int y2 =  bcdToDecimal(y);
	dt->year = y2+2000;
	int m2 = bcdToDecimal(m);
	dt->month = m2;
	int d2 = bcdToDecimal(d);
	dt->day = d2;
	rtcEnInterrupts();
}


