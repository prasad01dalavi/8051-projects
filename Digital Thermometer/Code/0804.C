 #include<reg51.h>
 #define lcd P2

 //functions' declaration
 void init();
 void delay(int);
 void character(unsigned char);
 void cmd(unsigned char);
 void string(unsigned char *);
 void number(int);

 unsigned char read();

 sbit rs=P3^0;
 sbit en=P3^1;

 sbit wr=P3^2;
 sbit intr=P3^3;
 sbit rd=P3^4;

 unsigned char v;
													   
void main()
{
	unsigned char m=3;
	lcd=0x00;
	rs=0;
	en=0;
	init();
	cmd(0x01);
	string("     DIGITAL  ");
	cmd(0xc0);
	string("   THERMOMETER");
	delay(2000);
 	cmd(0x01);
	string("Temperature:");
	while(1)
	{
		cmd(0x8c);
		v=read();		 
		number(v);
		string("C");

	}

 }

//--------------------------LCD Code-------------------//
void init()
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




