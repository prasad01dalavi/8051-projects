#include<reg51.h>
#define lcd P2

 //functions' declaration
 void lcd_init();
 void uc_init();
 void character(unsigned char);
 void cmd(unsigned char);
 void string(unsigned char *);
 void number(int);

 void uart_init();
 void gstring(unsigned char *p);
 void Tx(unsigned char ch);
 void bin_overflow_msg();

 void delay(int); 
 void buzzer(unsigned char b);

 sbit rs=P0^0;
 sbit en=P0^1;

 sbit buz=P3^5;
 sbit bin=P1^0;
 sbit alarm=P1^1;

 unsigned char t=0;

void main()
{
 	uc_init();
	lcd_init();
	uart_init();

	string("Garbage Overflow");
	cmd(0xc0);
	string("Indicator.");
	delay(2000);

	cmd(0x01);
	cmd(0x80);
	string("System is Ready!");
	delay(1000);

	cmd(0x01);
	cmd(0x80);

	for(;;)
	{

	    while(bin==1)
		{
			cmd(0x80);
			string("No Overflow !");
			delay(2000);
		}
	
		if(bin==0)
		{
			delay(5000);
			while(bin==0)
			{
				alarm=1;
				cmd(0x01);
				cmd(0x80);
				string("Bin Overflow");
				cmd(0xc0);
				string("Detected !");
				delay(2000);

				if(t==0)
				bin_overflow_msg();			
			}
		}
	}
}

//---------------microcontroller initialzation-----------//
void uc_init()
{
	lcd=0x00;
	rs=0;
	en=0;
	buz=0;
	bin=1;
	alarm=0;	
}
//-----------------microcontroller has been initialized-----------//

//--------------------------LCD Code-------------------//
void lcd_init()
{
//	cmd(0x20);
//	cmd(0x28);
//	cmd(0x30);
	cmd(0x01);	   //clear the screen c

	cmd(0x38);	   //8-bit mode with 5*7 matrix
	cmd(0x0c);	   //display on with cursor =>(0ch=curson off; 0fh=cursor blinking; 0eh=curson on)
	
	
	cmd(0x06);	   //shift cursor =>(06h right--> ; 04h <---left; default=right--->)
	cmd(0x80);	   //position =>(80=1st row 1st position; c0=2nd row 1st position)
}

void cmd(unsigned char ch)
{
	lcd=ch;
	rs=0;
	en=1;
	delay(2);
	en=0;
}

void character(unsigned char ch)
{
	lcd=ch;
	rs=1;
	en=1;
	delay(2);
	en=0;
}

void string(unsigned char *p)
{
	while(*p)
	character(*p++);
}

void number(int n)
{	
	unsigned char i=1,arr[5];

	if(n<0)
	{
		character('-');		  //signed representation for negative number
		n=-n;				  //taking magnitude 
	}
	if(n==0)
		character('0');
	while(n>0)
	{
		arr[i]=(n%10)+48;
		n=n/10;
		i++;
	}
	
	for(--i;i>0;i--)		 //problem with >=; remedy=> initialize i=1 and stop at i>0
	{
	character(arr[i]);
	}
}
//-----------------------End of LCD Code------------------------// 

//----------------------GSM Code-----------------------//
void uart_init()
{
	TMOD=0x20;
	TH1=0xFD;
	SCON=0x50;
	TR1=1;
}

void gstring(unsigned char *p)
{
 	while(*p)
	Tx(*p++);	
}

void Tx(unsigned char ch)
{
	SBUF=ch;
	while(TI==0);
	TI=0;
}

void bin_overflow_msg()
{
	t=1;
	buzzer(2);										//to send the message once
	gstring("AT");
	delay(2000);
	Tx(0x0d);
	
	gstring("AT+CMGF=1");						//Configuring as text mode i.e.
	delay(2000);								//if '0'=>PDU and if '1'=>text mode 
	Tx(0x0d);									//0x0d or 13d =>Enter i.e. Enter key
	delay(2000);

	cmd(0x01);
	string("Sending");
	cmd(0xc0);
	string("Message...");
	
	gstring("AT+CMGS=");					//'s' for sending message
	Tx('"');
	gstring("8007817602");			
	Tx('"');
	delay(2000);

	Tx(0x0d);
	delay(2000);
	
	gstring("Bin Overflow !");
	Tx(0x1a);										//To send the message
	delay(2000);
	Tx(0x0d);
	
	cmd(0x01);
	string("Message Sent!");
	delay(2000);
}
//--------------------End of GSM Code------------------//

//--------------delay function-------------------------//
void delay(int d)
{
	unsigned char i;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);		//500usec
		for(i=250;i>0;i--);		//500usec
	}
}
//-----------------End  of delay function---------------------//


//------------------------alarm system-----------------------//
void buzzer(unsigned char b)
{
 	for(;b>0;b--)
	{
		buz=1;
		delay(500);
		buz=0;
		delay(500);
	}
}
//---------------End of alarm system-------------------//

