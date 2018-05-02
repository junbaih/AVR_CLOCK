/*
 * hw2.c
 *
 * Created: 2018/4/25 20:14:05
 * Author : Houjunbai
 */ 

#include <avr/io.h>
#include "avr.h"
#include "avr.c"
#include "lcd.h"
#include "lcd.c"
#include <stdio.h>


unsigned char is_pressed(unsigned char r, unsigned char c)
{
	DDRC = 0;
	PORTC = 0;
	
	/* set r strong 0 */
	SET_BIT(DDRC,r); /* set  strong port*/
	CLR_BIT(PORTC,r); /* set strong 0*/
	/* set c weak 1 */
	SET_BIT(PORTC,c+4);
	wait_avr(5);
	/* get key*/
	if(!GET_BIT(PINC,c+4))
		return 1;
	return 0;
}
unsigned int get_key()
{
	for(unsigned char r = 0;r<4;r++)
		for(unsigned char c=0;c<4;c++)
			{
				if(is_pressed(r,c))
					return 4*r+c+1;
			}
	return 0;
}
/*
typedef struct {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned int year;
	unsigned char month;
	unsigned char day;	
	} timeclock;
	
void ini_timeclock(timeclock* tclk)
{
	tclk->second = 0;
	tclk->minute = 0;
	tclk->hour = 0;
	tclk->day = 1;
	tclk->month = 1;
	tclk->year =2018;
	
}

void run_timeclock(timeclock* tclk)
{
	wait_avr(1000);
	tclk->second++;
	if(60 <= tclk->second)
		{
			tclk->minute++;
			tclk->second=0;
		}
	if(60 <= tclk->minute)
		{
			tclk->hour++;
			tclk->minute=0;
		}
	if(24 <=tclk->hour)
		{
			tclk->day++;
			tclk->hour=0;
		}
	
	
	/* TODO */
	/*
	SET_BIT(LCD.sec,tclk.sec)
	SET_BIT(LCD.min,tclk.min)
	set_lcd2(LCD.hour,tclk.hour)	

} */ 

	
int main(void)
{ /*
	timeclock mytimeclock;
	    
	/* Replace with your application code */
	//ini_avr();
	ini_lcd();
	clr_lcd();
	char buf[17];
	while (1) 
    {
		unsigned int key = get_key();
		/*SET_BIT(DDRB,0);
		for(unsigned char i = 0;i<key;++i)
		{
							SET_BIT(PORTB,0);
							//for(volatile int i = 0; i<30000;++i);   //
							wait_avr(500);
							CLR_BIT(PORTB,0);
							wait_avr(500);
		}*/
		
		pos_lcd(0,0);
		sprintf(buf,"%02i",key);
		puts_lcd2(buf);
    }
}
