#include <stdio.h>
//#include <unistd.h>
#include "lcd.h"
#include "lcd.c"
#include "avr.h"
#include "avr.c"
#define DATE_R 0
#define TIME_R 1
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
	unsigned int temp;
	for(unsigned char r = 0;r<4;r++)
	for(unsigned char c=0;c<4;c++)
	{
		if(is_pressed(r,c))
		{
			temp = 4*r+c+1;
		if(temp<8 && temp>4)
			temp--;
		if(temp>8 && temp<12)
			temp-=2;
		if(temp == 14)
			temp = 0;
		else{
		switch(temp)
		{
			case 4:
				temp = 10;
				break;
			case 8:
				temp = 11;
				break;
			case 12:
				temp = 12;
				break;
			case 16:
				temp  =13;
				break;
			case 13:
				temp = 14;
				break;
			case 15:
				temp = 15;
				break;
			default:
				break;	
		}
		}
		//while(is_pressed(r,c));
		return temp;}
	}
	return 124;
}
struct time
{
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
} lcd_time;


struct date
{
	unsigned short year;
	unsigned char  day;
	unsigned char  month;
} lcd_date;


void display(){
	char date_buffer[50];
	char time_buffer[50];
	clr_lcd();

	sprintf(date_buffer, "%02d/%02d/%04d",lcd_date.month,lcd_date.day,lcd_date.year);
	pos_lcd(0,0);
	puts_lcd2(date_buffer);

	sprintf(time_buffer, "%02d:%02d:%02d",lcd_time.hour,lcd_time.minute,lcd_time.second);
	pos_lcd(1,0);
	puts_lcd2(time_buffer);
}
unsigned char is_num_key(unsigned char k)
{
	if( k>=0 && k<=9)
		return 1;
	return 0;
}
unsigned char setting(){
	unsigned char k = 0;
	unsigned char j = 0;
	while (1)
	{
		k = get_key();
		if (k==10)
			return 66;
		if(is_num_key(k))
		{	
			while(1)
			{
				j = get_key();
				if(is_num_key(j))
					return 10*k+j; 
			}
		}
	}

}



void editing_mode(unsigned char* set_mode)
{
	while(1)
	{
		char key = get_key();
		switch (key)
		{
			case 10:
			(*set_mode)++;
			display();
			wait_avr(1000);
			break;
			
			case 11:
						(*set_mode)--;
						display();
						wait_avr(1000);	
						break;
					
			
			case 12:
			return;
			
			default:
			break;	 
			
		}
	
	}
}


unsigned short setting_short(){
	unsigned char k = 0;
	unsigned char j = 0;
	unsigned char i = 0;
	unsigned char h = 0;
	while(k == 0)
	k = get_key();
	if(k == 10)
	return 0;
	while(j == 0)
	j = get_key();
	if(j == 10)
	return 0;
	while(i == 0)
	i = get_key();
	if(i == 10)
	return 0;
	while(h == 0)
	h = get_key();
	if(h == 10)
	return 0;
	else
	return k*1000 + j*100 + i*10 + h;
}









void enter_ui(){
	edit_month();
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
	if(d > 28){
		if(m == 2){
			if((y%4 == 0 && y%100 != 0) 
				|| y%400 == 0){
				if(d > 29){
					return 1;
				}
			}
			else{
				return 1;
			}
		}
	}
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
    	if(check_month()){
            lcd_date.day = 1;
		    lcd_date.month += 1;
    	}
    }
    if(lcd_date.month > 12){
    	lcd_date.month = 1;
    	lcd_date.year += 1;
    }
}


/* ----------------------------------------------- */
void set_hour()
{
	//if get_key()
	 
	 //nt 
}
void init_clock()
{
		lcd_time.minute = 0;
		lcd_time.second=0;
		lcd_time.hour=0;	
	    lcd_date.day = 1;
	    lcd_date.month = 1;
	    lcd_date.year = 2018;
	
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
//unsigned char* m = &(lcd_date.month);
			switch(input)
			{
				case 13:
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
		/*case 3:
			editing_mode(&(lcd_date.year));
			break;	*/		
		case 4:
					editing_mode(&(lcd_time.hour));
					break;
	    case 5:
					editing_mode(&(lcd_time.minute));
					break;
		case 11:
			break;
			
		default:
		break;
		}
		/*
11111111111111		clr_lcd();
		pos_lcd(0,0);
    	sprintf(date_buffer, "%02d/%02d/%4d",lcd_date.month,lcd_date.day,lcd_date.year);
		puts_lcd2(date_buffer);
		
		pos_lcd(1,0);
		sprintf(time_buffer, "%02d:%02d:%02d",lcd_time.hour,lcd_time.minute,lcd_time.second);

    	wait_avr(900);*/
		display();
		wait_avr(900);
   
    }

	return 0;
}