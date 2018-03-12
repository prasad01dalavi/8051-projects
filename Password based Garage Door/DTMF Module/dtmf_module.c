#include<reg51.h>
#define lcd P2

 //functions' declaration
void init();
void delay(int);
void character(unsigned char);
void cmd(unsigned char);
void string(unsigned char *);
void number(int);

void display(int a);

sbit rs=P3^0;
sbit en=P3^1;

sbit r0=P1^0;
sbit r1=P1^1;
sbit r2=P1^2;
sbit r3=P1^3;

sbit c0=P1^4;
sbit c1=P1^5;
sbit c2=P1^6;
sbit c3=P1^7;

// DTMF output simulator
sbit q0=P3^4;
sbit q1=P3^5;
sbit q2=P3^6;
sbit q3=P3^7;

void main()
{
	unsigned char m=3;
	lcd=0x00;
	rs=0;
	en=0;
	init();
	string("  MOBILE PHONE");
	delay(2000);
	
	cmd(0x01);
	string("DIALLING...");
	delay(2000);
	
	cmd(0x01);
	cmd(0x80);
	string("ENTER THE CODE:");
		
	r0=r1=r2=r3=0; 			//o/p port
	c0=c1=c2=c3=1;		  //i/p port
	q0=q1=q2=q3=0;      // make dtmf output simulator pins as output

//-------------------Keypad Code--------------------//
	while(1)
	{
	  r0=r1=r2=r3=0;
	  
	  if(c0==0)
	  {
			r0=r1=r2=r3=1;			   //to start making each row=0 one by one
			r0=0;
			if(c0==0)
			display(1);
			r0=1;
			r1=0;
			if(c0==0)
			display(5);
			r1=1;
			r2=0;
			if(c0==0)
			display(9);
			r2=1;
			r3=0;
			if(c0==0)
			display(13); 
	  }
	  	
	  else if(c1==0)
	  {
			r0=r1=r2=r3=1;			   //to start making each row=0 one by one
			r0=0;
			if(c1==0)
			display(2);
			r0=1;
			r1=0;
			if(c1==0)
			display(6);
			r1=1;
			r2=0;
			if(c1==0)
			display(10);
			r2=1;
			r3=0;
			if(c1==0)
			display(14);
	  }
		
	  else if(c2==0)
	  {
			r0=r1=r2=r3=1;			   //to start making each row=0 one by one
			r0=0;
			if(c2==0)
			display(3);
			r0=1;
			r1=0;
			if(c2==0)
			display(7);
			r1=1;
			r2=0;
			if(c2==0)
			display(11);
			r2=1;
			r3=0;
			if(c2==0)
			display(15);
	  }
	  else if(c3==0)
	  {
			r0=r1=r2=r3=1;			   //to start making each row=0 one by one
			r0=0;
			if(c3==0)
			display(4);
			r0=1;
			r1=0;
			if(c3==0)
			display(8);
			r1=1;
			r2=0;
			if(c3==0)
			display(12);
			r2=1;
			r3=0;
			if(c3==0)
			display(16);
	  }	
	}
}

void display(int a) 
{
	switch(a)
	{
	  case 1:{
		  		 cmd(0x01);
				   string("1");
				 	 q3=0; q2=0; q1=0; q0=1;
				
		   		 break;
			 }
	  case 2:{
		  		 cmd(0x01);
				   string("2");
				   q3=0; q2=0; q1=1; q0=0;
		   		 break;
			 }
	  case 3:{
		  		 cmd(0x01);
				   string("3");
					 q3=0; q2=0; q1=1; q0=1;
		   		 break;
			 }
	  case 5:{
		  		 cmd(0x01);
				   string("4");
			     q3=0; q2=1; q1=0; q0=0;
		   		 break;
			 }
	  case 6:{
		  		 cmd(0x01);
				   string("5");
					 q3=0; q2=1; q1=0; q0=1;
		   		 break;
			 }
	  case 7:{
		  		 cmd(0x01);
				   string("6");
					 q3=0; q2=1; q1=1; q0=0;
		   		 break;
			 }
	  case 9:{
		  		 cmd(0x01);
				   string("7");
			     q3=0; q2=1; q1=1; q0=1;
		   		 break;
			 }
	  case 10:{
		  		 cmd(0x01);
				   string("8");
				   q3=1; q2=0; q1=0; q0=0;
		   		 break;
			 }
	  case 11:{
		  		 cmd(0x01);
				   string("9");
			     q3=1; q2=0; q1=0; q0=1;
		   		 break;
			  }
	  case 13:{
		  		 cmd(0x01);
				   string("*");
			     q3=1; q2=0; q1=1; q0=1;
		   		 break;
			  }
	  case 14:{
		  		 cmd(0x01);
				   string("0");
					 q3=0; q2=0; q1=0; q0=0;
		   		 break;
			  }
	  case 15:{
		  		 cmd(0x01);
				   string("#");
			     q3=1; q2=1; q1=0; q0=0;
		   		 break;
			  }		
	}
}
//-----------End of Keypad Code----------------//

//-----------------LCD Code---------------------//

void init()
{

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

void delay(int d)
{
	unsigned char i;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);		//500usec
		for(i=250;i>0;i--);		//500usec
	}
}


//---------------End of LCD Code----------------------//