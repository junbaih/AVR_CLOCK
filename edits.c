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
	m = setting();
	if(m != 0){
		if(m > 12)
			lcd_date.month = 12;
		else
			lcd_date.month = m;
	}
	else{
		edit_day;
	}
}

void edit_day(){
	unsigned char m = 0;
	m = setting();
	if(m != 0){
		if(m > 30)
			lcd_date.month = 30;
		else
			lcd_date.month = m;
	}
	else{
		edit_year;
	}
}

void edit_year(){
	unsigned short m = 0;
	m = setting_short();
	if(m != 0){
		lcd_date.month = m;
	}
}

void edit_day(){
	unsigned char new = 99;
    unsigned char k;
    k = get_key();
    if(k == 4){
    	edit_year();
    }
    else{
    	while(new > 12){
    		new = setting();
    	}
    	lcd_date.day = new;
	}
}


void enter_ui(){
	edit_month();
}