#include <stdio.h>
#include <unistd.h>
#include <lcd.h>
#include <avr.h>

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


unsigned char get_input(){
    return 1;
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

/* UI SECTION ---------------------------------------- */
/*
unsigned char setting(){
	unsigned char k;
	unsigned char count;
	unsigned char result = 0;
	while(count < 2){
		k = get_key();
		switch(k){
			case 1: result = result*10 + 1;count++;break;
			case 2: result = result*10 + 2;count++;break;
			case 3: result = result*10 + 3;count++;break;
			case 4: result = result*10 + 4;count++;break;
			case 5: result = result*10 + 5;count++;break;
			case 6: result = result*10 + 6;count++;break;
			case 7: result = result*10 + 7;count++;break;
			case 8: result = result*10 + 8;count++;break;
			case 9: result = result*10 + 9;count++;break;
		}
	}
	return result;
}

unsigned short setting_short(){
	unsigned char k;
	unsigned char count;
	unsigned short result = 0;
	while(count < 4){
		k = get_key();
		switch(k){
			case 1: result = result*10 + 1;count++;break;
			case 2: result = result*10 + 2;count++;break;
			case 3: result = result*10 + 3;count++;break;
			case 4: result = result*10 + 4;count++;break;
			case 5: result = result*10 + 5;count++;break;
			case 6: result = result*10 + 6;count++;break;
			case 7: result = result*10 + 7;count++;break;
			case 8: result = result*10 + 8;count++;break;
			case 9: result = result*10 + 9;count++;break;
		}
	}
	return result;
}
void edit_month(unsigned char position){
    lcd_date.month = 99;
    unsigned char new = 99;
    while(new > 12){
    	new = setting();
    }
}



void enter_ui(){
	printf("hello ui");
	unsigned char k;
	unsigned char g = 1;
	while(g){
		k = get_key();
		switch(k){
			case 1:edit_month();break;
			case 2:edit_day();break;
			case 3:edit_year();break;
			case 4:edit_hour();break;
			case 5:edit_minute();break;
			case 6:edit_second();break;
			case 16:g = 0;break;
		}
	}

}
*/

/* ----------------------------------------------- */

int main(int argc, char const *argv[])
{
	unsigned char mode = 0; /*mode = 0: self-running; mode = 1: modifying*/
    unsigned char input = 0;/*keyboard input*/
    ini_lcd();
    char buffer[100];
    lcd_date.day = 1;
    lcd_date.month = 1;
    lcd_date.year = 2018;

    while(1){
    	input = get_input();/*check the input from keyboard*/
    	
    	if(input < 15){/*check current mode*/
    		self_run();/*run clock*/
    	}
    	else{
    		/*enter_ui();*//*enter UI*/
    	}
    	sprintf(buffer, "%d,%d,%d   %d:%d:%d\n",lcd_date.month,lcd_date.day,lcd_date.year
    		,lcd_time.hour,lcd_time.minute,lcd_time.second);
		clr_lcd();
		puts_lcd2(buffer);
    	/*printf("%d,%d,%d   %d:%d:%d\n",lcd_date.month,lcd_date.day,lcd_date.year
    		,lcd_time.hour,lcd_time.minute,lcd_time.second);*/
    	wait_avr(900);
    	/*sleep(1);*/
    }

	return 0;
}