#include<reg51.h>
#define lcd P2

//functions' declaration
void controller_init();
void lcd_init();
void character(unsigned char);
void cmd(unsigned char);
void string(unsigned char *);
void delay(int);

void check_and_open();
void check_and_close();
	
// Motor Driver IC Input pins
sbit in1=P1^0;
sbit in2=P1^1;	 

// lcd control pins
sbit rs=P1^6;
sbit en=P1^7;

unsigned char m=3;
int d, password_digit1, password_digit2, password_digit3, password_digit4, flag=0, end_star, end_hash;

void main()
{
	controller_init();
	lcd_init();
	string("PASSWD PROTECTED");
	cmd(0xc0);
	string("   GARAGE DOOR");
	delay(2000);
	
	while(1)  // Execute the following commands infinitely(continueously)
	{
		if((P3 & 0x0f) == 11)   // if * is pressed to open the door
		{
			check_and_open();              // call this function to check the code and open the door
		}

		if((P3 & 0x0f) == 12)   // if # is pressed to close the door
		{
			check_and_close();           // call this function to check the code and open the door
		}		
	}	
}

void check_and_open()
{
	while((P3 & 0x0f) == 11);
	
	password_digit1=(P3 & 0x0f);      // store first digit of password
	while((P3 & 0x0f) == 1);      // wait till the next digit is pressed
	
	password_digit2=(P3 & 0x0f);      // store second digit of password
	while((P3 & 0x0f) == 2);
	
	password_digit3=(P3 & 0x0f);      // store third digit of password
	while((P3 & 0x0f) == 3);
	
	password_digit4=(P3 & 0x0f);      // store second digit of password
	while((P3 & 0x0f) == 4);
	
	end_star=(P3 & 0x0f);
	
	if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && end_star == 11 && flag == 0)	
	{
		cmd(0x01);
		cmd(0x80);
		string("    OPENING");
		cmd(0xc0);
		string("THE GARAGE DOOR");
		in1=1; in2=0;  // starting motor anti-clockwise
		delay(3000);  //open the door shutter for 5 sec and then close
		in1=in2=0;      // stop the motor
		cmd(0x01);
		string("  DOOR OPENED!");
		delay(3000);
		flag = 1;     // remember that the door is open, do not reopen it		
	}	
	else if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && end_star == 11 && flag == 1)
	{
		cmd(0x01);
		cmd(0x80);
		string("DOOR IS ");
		cmd(0xc0);
		string("ALREADY OPENED!");
		delay(3000);
	}	
}

void check_and_close()
{
	while((P3 & 0x0f) == 12);
	
	password_digit1=(P3 & 0x0f);      // store first digit of password
	while((P3 & 0x0f) == 1);      // wait till the next digit is pressed
	
	password_digit2=(P3 & 0x0f);      // store second digit of password
	while((P3 & 0x0f) == 2);
	
	password_digit3=(P3 & 0x0f);      // store third digit of password
	while((P3 & 0x0f) == 3);
	
	password_digit4=(P3 & 0x0f);      // store second digit of password
	while((P3 & 0x0f) == 4);
	
	end_hash=(P3 & 0x0f);
	
	if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && end_hash == 12 && flag == 1)
	{	
		cmd(0x01);
		cmd(0x80);
		string("    CLOSING");
		cmd(0xc0);
		string("THE GARAGE DOOR");
		in1=0; in2=1;  		// starting motor clockwise
		delay(3000);  		// close the door shutter in 5 sec and then stop the motor
		in1=in2=0;        // stop the motor
		cmd(0x01);
		string("  DOOR CLOSED!");
		delay(3000);
		flag = 0; 
	}
	else if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && end_hash == 12 && flag == 0)
	{
		cmd(0x01);
		cmd(0x80);
		string("DOOR IS");
		cmd(0xc0);
		string("ALREADY CLOSED!");
		delay(3000);
	}
}

//---------------microcontroller initialzation-----------//
void controller_init()
{
	lcd=0x00;    // make lcd connected pins as output
	rs=0;
	en=0;
	in1=in2=0;  // make motor driver ic pins as output
	P3 = 0xff;  // make dtmf module pins as inputs
}

//--------------------------LCD Code-------------------//
void lcd_init()
{
	cmd(0x01);	   //clear the screen 
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
//-----------------------End of LCD Code------------------------//  

void delay(int d)
{
	unsigned char i;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);		//500usec
		for(i=250;i>0;i--);		//500usec
	}
}


