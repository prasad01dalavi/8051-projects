#include<reg51.h>
#define lcd P2

//functions' declaration
void controller_init();
void lcd_init();
void character(unsigned char);
void cmd(unsigned char);
void string(unsigned char *);
void number(int);
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
unsigned int password[];	
int d, password_digit1, password_digit2, password_digit3, password_digit4, flag=0;

void main()
{
	controller_init();
	lcd_init();
	string("PASSWD PROTECTED");
	cmd(0xc0);
	string("   GARAGE DOOR");
	delay(1000);
	in1=in2=0;
	P3 = 0xff;
	delay(1000);
	
	while(1)  // Execute the following commands infinitely(continueously)
	{
		if((P3 & 0x0f) == 10)   // if * is pressed to open the door
		{
			//while((P3 & 0x0f) == 10);      // waiting for next digit to press			
			check_and_open();              // call this function to check the code and open the door
		}

		if((P3 & 0x0f) == 11)   // if # is pressed to close the door
		{
			//while((P3 & 0x0f) == 11);     // waiting for next digit to press
			check_and_close();           // call this function to check the code and open the door
		}		
	}	
}

void check_and_open()
{
	while((P3 & 0x0f) == 10);
	
	password_digit1=(P3 & 0x0f);      // store first digit of password
	while((P3 & 0x0f) == 1);      // wait till the next digit is pressed
	
	password_digit2=(P3 & 0x0f);      // store second digit of password
	while((P3 & 0x0f) == 2);
	
	password_digit3=(P3 & 0x0f);      // store third digit of password
	while((P3 & 0x0f) == 3);
	
	password_digit4=(P3 & 0x0f);      // store second digit of password
	while((P3 & 0x0f) == 4);
	
	if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && flag == 0)	
	{
		cmd(0x01);
		cmd(0x80);
		string("    OPENING");
		cmd(0xc0);
		string("THE GARAGE DOOR");
		in1=1; in2=0;  // starting motor anti-clockwise
		delay(5000);  //open the door shutter for 5 sec and then close
		in1=in2=0;      // stop the motor
		cmd(0x01);
		string("  DOOR OPENED!");
		flag = 1;     // remember that the door is open, do not reopen it
	}	
	else if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && flag == 1)
	{
		cmd(0x01);
		cmd(0x80);
		string("DOOR IS ");
		cmd(0xc0);
		string("ALREADY OPENED!");
		delay(2000);
	}
	
}

void check_and_close()
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
	
	if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && flag == 1)
	{	
		cmd(0x01);
		cmd(0x80);
		string("    CLOSING");
		cmd(0xc0);
		string("THE GARAGE DOOR");
		in1=0; in2=1;  		// starting motor clockwise
		delay(5000);  		// close the door shutter in 5 sec and then stop the motor
		in1=in2=0;        // stop the motor
		cmd(0x01);
		string("  DOOR CLOSED!");
		flag = 0; 
	}
	else if(password_digit1 == 1 && password_digit2 == 2 && password_digit3 == 3 && password_digit4 == 4 && flag == 0)
	{
		cmd(0x01);
		cmd(0x80);
		string("DOOR IS");
		cmd(0xc0);
		string("ALREADY CLOSED!");
		delay(2000);
	}
}

//---------------microcontroller initialzation-----------//
void controller_init()
{
	// Make controller port pins as output pins
	lcd=0x00;
	rs=0;
	en=0;
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

void number(int n)
{	
	unsigned char i=1,arr[5];

	if(n<0)
	{
		character('-');		  //signed representation for negative number
		n=-n;				  			//taking magnitude 
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

void delay(int d)
{
	unsigned char i;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);		//500usec
		for(i=250;i>0;i--);		//500usec
	}
}


