void display(){
	char* date_buffer[50];
	char* time_buffer[50];
	clr_lcd();

	sprintf(date_buffer, "%02d,%02d,%04d",lcd_date.month,lcd_date.day,lcd_date.year);
	pos_lcd(0,0);
	puts_lcd2(date_buffer);

	sprintf(time_buffer, "%02d,%02d,%02d",lcd_time.hour,lcd_time.minute,lcd_time.second);
	pos_lcd(1,0);
	puts_lcd2(time_buffer);
}

unsigned char setting(){
	unsigned char k = 0;
	unsigned char j = 0;
	while(k == 0)
		k = get_key();
	if(k == 10)
		return 0;
	while(j == 0)
		j = get_key();
	if(j == 10)
		return 0;
	else
		return k*10+j;

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

void edit_month(){
	unsigned char m = 0;
	unsigned char n = lcd_date.month;
	lcd_date.month = 99;
	display();
	m = setting();
	if(m != 0){
		if(m > 12)
			lcd_date.month = 12;
		else
			lcd_date.month = m;
	}
	else{
		lcd_date.month = n;
		display();
		edit_day;
	}
}

void edit_day(){
	unsigned char m = 0;
	unsigned char n = lcd_date.day;
	lcd_date.day = 99;
	display();
	m = setting();
	if(m != 0){
		if(m > 30)
			lcd_date.month = 30;
		else
			lcd_date.month = m;
	}
	else{
		lcd_date.day = n;
		display();
		edit_year;
	}
}

void edit_year(){
	unsigned short m = 0;
	unsigned short n = lcd_date.year;
	lcd_date.year = 9999;
	display();
	m = setting_short();
	if(m != 0){
		lcd_date.month = m;
	}
	else{
		lcd_date.year = n;
		display();
	}
}



void enter_ui(){
	edit_month();
}