#include<reg51.h>
sbit r1=P2^4;
sbit r2=P2^3;
sbit r3=P2^2;
sbit output=P1^0;  		//Output to motor
sbit rs = P2^6;
sbit rw = P2^5;
sbit en = P2^7;
sbit btn = P3^0;
sbit btn1 = P3^1;


void values_on_ssd(void);
void display_num_7seg(unsigned char num);
unsigned char values[4];
unsigned char index=0;
unsigned int select[]  =  {0x0C,0x08,0x04,0x00};
void fun(unsigned char x);
void lcd_data(unsigned char item);
void lcd_cmd(unsigned char item);
void lcd_data_int(int time_val)	;
void lcd_DisplayString(char *string_ptr);
unsigned int message[] = {0x3f,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,};
void shape()					 // Function to make degree sign
{
	lcd_cmd(0xC4);
	lcd_data('D');
	lcd_data('E');
	lcd_data('G');
	lcd_data('R');
	lcd_data('E');
	lcd_data('E');

		
}

void delay(unsigned int msec)   // Function for delay
{
	int i,j;
	for(i=0;i<msec;i++)
		for(j=0;j<1275;j++);
} 

void timer(int msec)	 // Function for timer
{
	int i;
	TR1=1;
	for(i=0;i<msec;i++)
	{
		while(TF1==0);
		TF1=0;
	}
	TR1=0;
}
void main()
{
	int i,p;
	char count;
			lcd_cmd(0x38);
	lcd_cmd(0x0E);
	lcd_cmd(0x01);
	TMOD=0x20; 		// Mode2
	TH1= -23;		
	output=0;
	count=21;
lcd_DisplayString(" SERVO ANGLE ");
	shape();
while(1)
	{
		if(count>=104)
			count=21;
		else 
		{
			if(btn==0)
			{count=count+1;}
			if(btn1==0)
			{count=count-1;}
		}
		lcd_cmd(0xC0);
		p=2.5*(count-21);
				lcd_data_int(p);
		
		for(i=0;i<50;i++)
		{
				
			output=1;
			timer(count);		 
			output=0;
			timer(360);
    	display_num_7seg(p);
			if(p>180)
			{p=0;}
 			values_on_ssd();
		}

	}
}

void lcd_data_int(int time_val)			// Function to send three digit number
{
	int a;
	a = time_val/100;
	  
	lcd_data(a+48);
	time_val = time_val%100;
	a = time_val/10;
	
	lcd_data(a+48);
	a = time_val%10;	
	 
	lcd_data(a+48);
}


void lcd_cmd(unsigned char item)		// Function to send command to LCD
{
	P0 = item;
	rs= 0;
	rw=0;
	en=1;
	delay(1);
	en=0;
	return;
} 

void lcd_data(unsigned char item)		// Function to send one byte data to LCD
{
	P0 = item;
	rs= 1;
	rw=0;
	en=1;
	delay(1);
	en=0;
}
void lcd_DisplayString(char *string_ptr)
{
   while(*string_ptr)
    lcd_data(*string_ptr++);
    }
 
void display_num_7seg(unsigned char num)
{
 int i=3;
 while(num!=0)
 {
  values[i]=num%10;
	 num=num/10;
	 i--;
 }}
void values_on_ssd(void)
 {
  switch(values[index])
	{
		case 0:
		   {
			 P2=select[index];
			 P0=message[0];	
              				 
			 }
	     break;
		case 1:
		   {
			 P2=select[index];
			 P0=message[1];		 
			 }
	     break;
		case 2:
		   {
			 P2=select[index];
			 P0=message[2];		 
			 }
	     break;
		case 3:
		   {
			  P2=select[index];
			 P0=message[3];		 
			 }
	     break;
		case 4:
		   {
			 P2=select[index];
			 P0=message[4];		 
			 }
	     break;
		case 5:
		   {
			  P2=select[index];
			 P0=message[5];		 
			 }
	     break;
		case 6:
		   {
			  P2=select[index];
			 P0=message[6];		 
			 }
	     break;
		case 7:
		   {
			 P2=select[index];
			 P0=message[7];		 
			 }
	     break;
		case 8:
		   {
			 P2=select[index];
			 P0=message[8];		 
			 }
	     break;
		case 9:
		   {
			 P2=select[index];
			 P0=message[9];		 
			 }
	     break;
	default:
		break;
	}
 index+=1;
	if(index > 3)
					{
						index=0;
					}
				}