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
 void fire_msg();
 void intruder_msg();
 void lpg_msg();

 void delay(int); 
 void buzzer(unsigned char b);


 unsigned char read();

 sbit rs=P0^0;
 sbit en=P0^1;

 sbit wr=P3^5;
 sbit intr=P3^6;
 sbit rd=P3^7;

 sbit buz=P0^2;

 unsigned char v,f=0,s=0,t=0,m=3;

//-------------------ISR--------------------//
 void ext0() interrupt 0
 {
	 	cmd(0x01);
		cmd(0x80);
		string("Danger!!!");
		cmd(0xc0);
		string("LPG Gas Leakage");
		buzzer(2);
		if(s==0)
		lpg_msg();	 
 }
 
 void ext1() interrupt 2
 {	
	 cmd(0x01);
	 cmd(0x80);
	 string("Be alert!");
	 delay(2000);
	 cmd(0x80);
	 string("Intruder at");
	 cmd(0xc0);
	 string("The Gate!");
	 buzzer(2);

	 if(t==0)
	 intruder_msg();
 }
 //-------------------ISR End-----------------------//
													   
void main()
{
	uc_init();
	lcd_init();
	uart_init();
	cmd(0x01);
	string(" Home Security  ");
	delay(2000);
 	cmd(0x01);
	while(1)
	{
		v=read();  					//check for the fire
		while(v<40 && s==0 && t==0)
		{
			cmd(0x80);
			string(" Home is Safe");		 
		}

		if(v>40)
		{
			cmd(0x01);
			string("Home is not Safe");
			cmd(0xc0);
			string("Fire Detected");

			if(f==0)
			fire_msg();
			delay(2000);
		} 

		if(s==1)
		{
			cmd(0x01);
			string("Home is not Safe");
			cmd(0xc0);
			string("LPG Gas Leakage !");
			delay(2000);
		}
		
		if(t==1)
		{
			cmd(0x01);
			string("Home is not Safe");
			cmd(0xc0);
			string("Intruder Outside");
			delay(2000);
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
//configuring interrupts
	EA=1;
	EX0=1;
	EX1=1;
}

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

//----------------------ADC 0804 Code-------------------------//
unsigned char read()
{
	unsigned char value;
	wr=0;
	rd=1;
	delay(2);
	wr=1;			   //Low to High Pulse to start conversion
	while(intr==1);	   //wait till the conversion is done
	rd=0;			   //take (or read) the data by making rd=0
	delay(2);
	value=P1;		   
	return value;

}
//---------------End of ADC 0804 Code-----------------------//

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
void fire_msg()
{
	f=1;
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
	
	gstring("Fire in your Home");
	Tx(0x1a);										//To send the message
	delay(2000);
	Tx(0x0d);
	
	cmd(0x01);
	string("Message Sent!");
	delay(2000);

	cmd(0x01);
	string("Message has been");
	cmd(0xc0);
	string("Sent to Owner");
	delay(2000);
}

void lpg_msg()
{
	s=1;
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
	
	gstring("LPG GAS Leakage !");
	Tx(0x1a);										//To send the message
	delay(2000);
	Tx(0x0d);
	
	cmd(0x01);
	string("Message Sent!");
	delay(2000);

	cmd(0x01);
	string("Message has been");
	cmd(0xc0);
	string("Sent to Owner");
	delay(2000);
}

void intruder_msg()
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
	
	gstring("Intruder Outside");
	Tx(0x1a);										//To send the message
	delay(2000);
	Tx(0x0d);
	
	cmd(0x01);
	string("Message Sent!");
	delay(2000);

	cmd(0x01);
	string("Message has been");
	cmd(0xc0);
	string("Sent to Owner");
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




