#include <stdio.h>
//#include <unistd.h>
#include "lcd.h"
#include "lcd.c"
#include "avr.h"
#include "avr.c"
#define A 10
#define B 11
#define C 12 
#define D 13
#define STAR 14
#define Hash 15
#define None 255

struct time
{
	unsigned short hour;
	unsigned short minute;
	unsigned short second;
	unsigned char time_mode;
	unsigned char aORp;
} lcd_time;


struct date
{
	unsigned short year;
	unsigned short  day;
	unsigned short  month;
} lcd_date;


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
 unsigned char get_key()
{
	unsigned char temp;
	for(unsigned char r = 0;r<4;r++)
	for(unsigned char c=0;c<4;c++)
	{
		if(is_pressed(r,c))
		{
			temp = 4*r+c+1;

		switch(temp)
		{
			case 1:
				temp = 1;
				break;
			case 2:
				temp =2;
				break;
			case 3:
				temp =3;
				break;
			case 4:
				temp = A;
				break;
			case 5:
				temp =4;
				break;
			case 6:
				temp =5;
				break;
			case 7:
				temp = 6;
				break;
			case 8:
				temp = B;
				break;
			case 9:
				temp = 7;
				break;
			case 10:
				temp = 8;
				break;
			case 11:
				temp = 9;
				break;	
			case 12:
				temp = C;
				break;
			case 16:
				temp  =D;
				break;
			case 13:
				temp = STAR;
				break;
			case 15:
				temp = Hash;
				break;
			case 14:
				temp = 0;
				break;
			default:
				break;	
		}
		return temp;
		}
	}
	return None;
}


void display(){
	char date_buffer[50];
	char time_buffer[50];
	clr_lcd();

	sprintf(date_buffer, "%02d/%02d/%04d",lcd_date.month,lcd_date.day,lcd_date.year);
	pos_lcd(0,0);
	puts_lcd2(date_buffer);
	//char AP[2] = {'P','M'};
	sprintf(time_buffer, "%02d:%02d:%02d",lcd_time.hour,lcd_time.minute,lcd_time.second);
	pos_lcd(1,0);
	puts_lcd2(time_buffer);
}





void editing_mode(unsigned short* set_mode)
{
	while(1)
	{
		char key = get_key();
		switch (key)
		{
			case A:
			(*set_mode)++;
			display();
			wait_avr(300);
			break;
			
			case B:
			(*set_mode)--;
			display();
			wait_avr(300);	
		    break;
					
			
			case C:
			//exit
			return;
			
			default:
			break;	 
			
		}
	
	}
}
void time_M2R()
{
	lcd_time.aORp = lcd_time.hour>12?0:1; 
	lcd_time.hour=lcd_time.hour%12;
	lcd_time.time_mode=1;
}
void time_R2M()
{
	if(!lcd_time.aORp)
		lcd_time.hour+=12;
	lcd_time.time_mode=0;
}
void time_mode_switch()
{
	if(lcd_time.time_mode)
		time_R2M();
	time_M2R();
	
}

unsigned char check_month(){
	unsigned char d = lcd_date.day;
	unsigned char m = lcd_date.month;
	unsigned char y = lcd_date.year;
	if (d > 31)
	{
		return 1;
	}
	if (d > 30)
	{
		unsigned char change = 0;
		switch(m){
			case 4: change = 1;break;
			case 6: change = 1;break;
			case 9: change = 1;break;
			case 11:change = 1;break;
		}
		if(change){
			return 1;
		}
	}
	if(d > 28)
	{
		if(m == 2)
		{
			if((y%4 == 0 && y%100 != 0) || y%400 == 0)
			{
				if(d > 29)
				{
					return 1;
				}
			}

			return 0;
		
		}
	}
	return 0;
}

char leap()
{
	
	unsigned short y = lcd_date.year;
	if((!(y%4) && (y%100))|| !(y%400))
	return 1;
	return 0;
}
void self_run(){
    lcd_time.second += 1;
    if (lcd_time.second >= 60)
    {
    	lcd_time.second = 0;
    	lcd_time.minute += 1;
    }
    if (lcd_time.minute >= 60)
    {
    	lcd_time.minute = 0;
    	lcd_time.hour  += 1;
    }
    if (lcd_time.hour >= 24)
    {
    	lcd_time.hour = 0;
    	lcd_date.day += 1;
    /*	if(check_month()){
            lcd_date.day = 1;
		    lcd_date.month += 1;
    	}*/
    }
	if(lcd_date.day>28)
	{
		if(lcd_date.month==2)
		{
			if(!leap(lcd_date.year))
				{
					lcd_date.day=1;
					lcd_date.month++;
				}
			else if(lcd_date.month>29)
			{
					lcd_date.day=1;
					lcd_date.month++;
			}
			
		}
		else
		{
			if(lcd_date.month == 4 || lcd_date.month == 6 ||lcd_date.month == 9||lcd_date.month == 11)
			{
				if(lcd_date.day>30)
				{
					lcd_date.day=1;
					lcd_date.month++;
				}
			}
			else
			{
				if(lcd_date.day>31)
				{
					lcd_date.day=1;
					lcd_date.month++;
				}
				
			}
		}
	}
    if(lcd_date.month > 12){
    	lcd_date.month = 1;
    	lcd_date.year += 1;
    }
}




/* ----------------------------------------------- */

void init_clock()
{
		lcd_time.minute = 0;
		lcd_time.second=0;
		lcd_time.hour=0;	
		lcd_time.time_mode=0;
	    lcd_date.day = 1;
	    lcd_date.month = 1;
	    lcd_date.year = 2018;
	
}

void sim1()
{
	lcd_time.minute = 59;
	lcd_time.second = 50;
	lcd_time.hour = 23;
	lcd_date.year=2008;
	lcd_date.month=2;
	lcd_date.day=28;
}

void sim2()
{
	lcd_time.minute = 59;
	lcd_time.second = 50;
	lcd_time.hour = 23;
	lcd_date.year=2000;
	lcd_date.month=2;
	lcd_date.day=28;
}

void sim3()
{
		lcd_time.minute = 59;
		lcd_time.second = 50;
		lcd_time.hour = 23;
		lcd_date.year=1900;
		lcd_date.month=2;
		lcd_date.day=28;

}


int main(void)
{
	unsigned char mode = 0; /*mode = 0: self-running; mode = 1: modifying*/
    unsigned char input = 0;/*keyboard input*/
    ini_lcd();
	//clr_lcd();
	
    //char date_buffer[50];
	//char time_buffer[50];
	init_clock();

    while(1){
    	input = get_key();/*check the input from keyboard*/
    
    		self_run();/*run clock*/

			switch(input)
			{
				case D:
				init_clock();
				break;
			
    	//}		
				case 1:
			// EDIT
			
				editing_mode(&(lcd_date.month));
				break;
			
				case 2:
				editing_mode(&(lcd_date.day));
				break;
				case 3:
				editing_mode(&(lcd_date.year));
				break;	 		
				case 4:
				editing_mode(&(lcd_time.hour));
				break;
				
				case 5:
				editing_mode(&(lcd_time.minute));
				break;
				
				case STAR:
				sim1();
				display();
				break;
				/*
				time_mode_switch();
				display();
				break;
				
				
*/
			case Hash:
			sim2();
			display();
			break;
			case 0:
				sim3();
				display();
				break;
			default:
			break;
		}

		display();
		wait_avr(900);
   
    }

	return 0;
}