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
	//unsigned char alarm;
} lcd_time,alarm_time;



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
	wait_avrm(5);
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


void display(struct time l_time ){
	char date_buffer[50];
	char time_buffer[50];
	clr_lcd();

	sprintf(date_buffer, "%02d/%02d/%04d",lcd_date.month,lcd_date.day,lcd_date.year);
	pos_lcd(0,0);
	puts_lcd2(date_buffer);
	//char AP[2] = {'P','M'};
	if(!l_time.time_mode)
		sprintf(time_buffer, "%02d:%02d:%02d",l_time.hour,l_time.minute,l_time.second);
	else{
/*		if(lcd_time.hour<12)
			sprintf(time_buffer, "%02d:%02d:%02d AM",lcd_time.hour,lcd_time.minute,lcd_time.second);
		else if(!lcd_time.hour>=12){
			short temp = lcd_time.hour > 12?lcd_time.hour-12:lcd_time.hour;
			sprintf(time_buffer, "%02d:%02d:%02d PM",temp,lcd_time.minute,lcd_time.second);*/
	if(l_time.aORp)
				sprintf(time_buffer, "%02d:%02d:%02d AM",l_time.hour,l_time.minute,l_time.second);
	else
						sprintf(time_buffer, "%02d:%02d:%02d PM",l_time.hour,l_time.minute,l_time.second);
		
		}
	
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
			display(lcd_time);
			wait_avrm(300);
			break;
			
			case B:
			(*set_mode)--;
			display(lcd_time);
			wait_avrm(300);	
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
	if(lcd_time.time_mode) // if regular time switch to military time
			time_R2M();
	else
	time_M2R();
	
	//display();
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
void hit();
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
	//if(!lcd_time.time_mode)
    if (lcd_time.hour >= 24)
    {
    	lcd_time.hour = 0;
    	lcd_date.day += 1;
    /*	if(check_month()){
            lcd_date.day = 1;
		    lcd_date.month += 1;
    	}*/
	}
	/*}
	else
	{
		if(lcd_time.aORp)
		{
			if(lcd_time.hour==12)
			{
				lcd_time.aORp =0;
			}
			else if(lcd_time.hour>12)
			{
				
			}
		}
		
	}*/
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
	hit();
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
		alarm_time.minute = 0;
		alarm_time.hour = 0;
		alarm_time.second = 0;
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

void alarm_timecheck()
{
	if(alarm_time.minute>=60)
		{
			alarm_time.hour++;
			alarm_time.minute=0;
		}
	if(alarm_time.hour>=24)
		{
			alarm_time.hour = 0;
		}
}
void alarm_SM()
{
	char alarm_state = 0;
	while(1)
	{
		switch(alarm_state)
		{
			case 0:
				display(alarm_time);
				alarm_state = get_key();
				break;
			case A:
				return;
			case 1:
				alarm_time.minute++;
				alarm_timecheck();
				display(alarm_time);
				wait_avrm(300);
				alarm_state = get_key();
				break;
			case 2:
				alarm_time.hour++;
				alarm_timecheck();
				display(alarm_time);
				wait_avrm(300);
				alarm_state = get_key();
				break;
			default:
				alarm_state = 0;
				break;
					
		
		}
		
	}	
	
}
#define Am 440.00
#define Asharp 466.16
#define Bm 493.88
#define Cm 261.63
#define Csharp 277.18
#define Dm 293.66
#define Dsharp 311.13
#define E 329.63
#define F 349.23
#define Fsharp 369.99
#define G 392
#define Gsharp 415.3
#define Al 220.0
#define Bl 246.94
#define Cl 130.81
#define Dl 146.83
#define El 164.81
#define Fl 174.61
#define Gl 196
#define Ah 880
#define Bh 987.77
#define Ch 523.25
#define Dh 587.33
#define Eh 659.26
#define Fh 698.46
#define Gh 783.99
#define blank {0,0.8}
#define blank2 {0,4}
#define Ak 10
#define Bk 11
#define Ck 12
#define Dk 13
#define STAR 14
#define Hash 15
#define None 255

typedef struct note
{
	float freq;
	float duration;
} note;

const note song1[] =
{
	{Eh,0.8},{Dh,0.8},{Ch,0.8},{Bm,0.8},{Am,0.8},{G,0.8},{Am,0.8},
	{Bm,0.8},{G,0.8},{F,0.8},{E,0.5},{Dm,0.3},{Al,0.8},{Gl,0.8},
	{Al,0.8},{Bl,0.8},{E,0.7},{G,0.1},{F,0.2},{E,0.2},
	{Dm,0.2},{Cm,0.15},{Bl,0.15},{Cm,0.3},{E,0.2},{Dm,0.2},
	{Cm,0.2},{Bl,0.2},{Al,0.1},{Bl,0.1},{Cm,0.2},{Cm,0.3},
	{Dm,0.1},{E,0.2},{Cm,0.2},{E,0.2},{G,0.2},{A,0.1},
	{G,0.1},{Am,0.3},{F,0.1},{G,0.1},{Cm,0.1},
	{Dm,0.1},{Cm,0.1},{Bl,0.1},{Cm,0.1},{Dm,0.2},
	{G,0.1},{F,0.1},{E,0.8},{G,0.2},{F,0.2},
	{E,0.2},{Dm,0.2},{Cm,0.2},{Bl,0.2},{Cm,0.4},
	{E,0.2},{Dm,0.2},{Cm,0.2},{Bl,0.2},{Al,0.1},{Bl,0.1},{Cm,0.2},
	{Cm,0.3},{Dm,0.1},{E,0.2},{Cm,0.2},{E,0.2},{G,0.2},
	{Am,0.4},{Ch,0.4},{Bm,0.1},{Am,0.1},{Bm,0.1},{Cm,0.1},
	{Dh,0.2},{G,0.1},{F,0.1}
};
typedef struct music
{
	note* song;
	short length;
	char* name;
} music;

const music canon = {song1,79};

note song2[] = {
	{Cm,0.4},blank,{Cm,0.4},blank,{G,0.4},blank,{G,0.4},blank,{Am,0.4},
	blank,{Am,0.4},blank,{G,0.4},blank2,blank2,
	{F,0.4},blank,{F,0.4},blank,{E,0.4},blank,{E,0.4},blank,{Dm,0.4},
	blank,{Dm,0.4},blank,{Cm,0.4}
};
music littlestar = {song2,28,"little start"};
note song3[] = {
	{Cm,0.4},blank2,{Dm,0.4},blank2,{E,0.4},blank2,{F,0.4},blank2,{G,0.4},blank2,{Am,0.4},blank2,{Bm,0.4},blank2,{Ch,0.4}
};
music ladder = {song3,15,"something"};
void play_note(float f,float d)
{
	int k =f*d;
	float t = 1/f;
	if(k==0)
	wait_avrm(d*100);
	else
	for(int i=0;i<k;i++)
	{
		SET_BIT(PORTA,0);
		wait_avr(t/2 *100000);
		CLR_BIT(PORTA,0);
		wait_avr(t/2 *100000);
	}
	
}
void play_song(music m)
{
	for(int i =0;i<m.length;++i)
	play_note(m.song[i].freq,m.song[i].duration);
}

void hit()
{
	if(lcd_time.second==0 && lcd_time.minute == alarm_time.minute && lcd_time.hour==alarm_time.hour)
		{				char message[10] ;
			clr_lcd();
			sprintf(message,"alarm!!!");
			pos_lcd(0,0);
			puts_lcd2(message);
			//play_music();
			play_song(littlestar);
			lcd_time.second+=15;
			display(lcd_time);
		}
}

int main(void)
{
	unsigned char mode = 0; /*mode = 0: self-running; mode = 1: modifying*/
    unsigned char input = 0;/*keyboard input*/
	char alarm_flag = 0;
 // alarm off
		SET_BIT(DDRA,0);
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
						//	play_song(littlestar);

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
				case 7:
				alarm_flag = ~alarm_flag;
				break;
				case C:
				alarm_SM();
				break;
				
								
				case STAR:
				
				sim1();
				display(lcd_time);
				break;
				case 9:
				time_mode_switch();
				display(lcd_time);
				break;
				
				

			case Hash:
			sim2();
			display(lcd_time);
			break;
			case 0:
				sim3();
				display(lcd_time);
				break;
			default:
			break;
		}

		display(lcd_time);
		wait_avrm(900);
   
    }

	return 0;
}