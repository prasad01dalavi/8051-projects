#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define lcd P2 //DB0-DB7

void uc_init();
void lcd_init();
void delay(int);
void character(unsigned char);
void cmd(unsigned char);
void lcdPrint(unsigned char *);
void number(int);

void uart_init();
void Tx(unsigned char);
void serialPrint(unsigned char *ptr);

//lcd control signals
sbit rs = P3^6;
sbit en = P3^7;
sbit pirPin = P3^4;

sbit fan = P1^0;
sbit light1 = P1^1;
sbit light2 = P1^2;

char myData[20],myMsg[70];
unsigned char d = 1, m = 1,msgRead=0, msgInfo=0,pirFlag=0; //For counting the received SBUF Data bytes
unsigned char i1=1,i2=2,i3=3,i4=4,i5=5,i6=6,i7=7,i8=8,i9=9,i10=10;

void main()
{
	uc_init(); //Configuring I/O Ports 
	lcd_init(); //initializing lcd
	uart_init(); //Configuring serial communication

	lcdPrint("    WELCOME!");
	delay(1000);                //delay of 1 sec i.e. 1000msec
	cmd(0x01);
	lcdPrint("HOME AUTOMATION");
	delay(2000);
	
	cmd(0x01);
	cmd(0x80);
	lcdPrint("Initializing The");
	cmd(0xc0);
	lcdPrint("System.");
	delay(1000);
	cmd(0xc7);
	lcdPrint(".");
	delay(1000);
	cmd(0xc8);
	lcdPrint(".");
	delay(2000);	
	cmd(0x01);
	lcdPrint("8051 is Ready");
	delay(1000);
	
	serialPrint("AT");
	Tx(0x0d);
	delay(500);

	serialPrint("AT+CMGF=1");
	Tx(0x0d);
	delay(500);

	serialPrint("AT+CNMI=2,2,0,0,0"); 
	Tx(0x0d);
	delay(500);
	
	cmd(0x01);
	lcdPrint("GSM is Ready");
	delay(2000);
	cmd(0x01);
	
	lcdPrint("HOME AUTOMATION");
	
	//The received message is in the form of
	//+CMF: "+91xxxxxxxxxx","","dd/mm/yy,hh:mm:ss+22"
	//my message contents
		
	while(1)					  //do this process continuously
	{
		
		if(RI==1)   //when any data is received at rxd line
		{	 
			myData[d]=SBUF;
			if(myData[d]=='*')
			{
				msgRead=1;
				d=1;
				m=1;
				myData[d]='*';
			}

			if(msgRead==1)
			{
				myMsg[m]=myData[d];
				m++;
			}
			
			if(myMsg[i1]=='*' && myMsg[i2]=='L' && myMsg[i3]=='1' && myMsg[i4]=='O' && myMsg[i5]=='N') //If L1ON message is received   
			{
				cmd(0x01);
				lcdPrint("LIGHT1 ON");
				light1 = 1;
			 // light2 = 0;
			 // fan = 0;
				
				m=1;
				d=1;
				msgRead=0;
				myMsg[i5]='#';   //this is for non detection of the same message again
			}
			
			if(myMsg[i1]=='*' && myMsg[i2]=='L' && myMsg[i3]=='1' && myMsg[i4]=='O' && myMsg[i5]=='F' && myMsg[i6]=='F') //If L1OFF message is received
			{
				cmd(0x01);
				lcdPrint("LIGHT1 OFF");
				light1 = 0;
			 // light2 = 0;
			 // fan = 0;
				
				m=1;
				d=1;
				msgRead=0;
				myMsg[i5]='#';
			}
			
			if(myMsg[i1]=='*' && myMsg[i2]=='L' && myMsg[i3]=='2' && myMsg[i4]=='O' && myMsg[i5]=='N') //If L2ON message is received   
			{
				cmd(0x01);
				lcdPrint("LIGHT2 ON");
			//	light1 = 0;
			  light2 = 1;
			//  fan = 0;
				
				m=1;
				d=1;
				msgRead=0;
				myMsg[i5]='#';   //this is for non detection of the same message again
			}
			
			if(myMsg[i1]=='*' && myMsg[i2]=='L' && myMsg[i3]=='2' && myMsg[i4]=='O' && myMsg[i5]=='F' && myMsg[i6]=='F') //If L2OFF message is received
			{
				cmd(0x01);
				lcdPrint("LIGHT2 OFF");
			//	light1 = 0;
			  light2 = 0;
			//  fan = 0;
				
				m=1;
				d=1;
				msgRead=0;
				myMsg[i5]='#';
			}
			
			if(myMsg[i1]=='*' && myMsg[i2]=='F' && myMsg[i3]=='O' && myMsg[i4]=='N') //If FON message is received   
			{
				cmd(0x01);
				lcdPrint("FAN ON");
			//	light1 = 0;
			//  light2 = 0;
			  fan = 1;
				
				m=1;
				d=1;
				msgRead=0;
				myMsg[i4]='#';   //this is for non detection of the same message again
			}
			
			if(myMsg[i1]=='*' && myMsg[i2]=='F' && myMsg[i3]=='O' && myMsg[i4]=='F' && myMsg[i5]=='F') //If FOFF message is received   
			{
				cmd(0x01);
				lcdPrint("FAN OFF");
				//light1 = 0;
			//  light2 = 0;
			  fan = 0;
				
				m=1;
				d=1;
				msgRead=0;
				myMsg[i4]='#';   //this is for non detection of the same message again
			}
			
			RI=0;
			d++;
			
		}
						
		if(pirPin == 1 && pirFlag == 0)	   //wait till pir sensor o/p becomes high i.e. when it senses any human
		{
			cmd(0x01);
			cmd(0x80);
		    lcdPrint("Motion Detected!");
			light1 = 1;
			light2 = 1;
			fan = 1;
			pirFlag=1;
		}  		
	}	
}

void uc_init()
{
	pirPin = 1;	//making as i/p port 

	//making as o/p port
	lcd=0x00;
	rs=0;
	en=0;  

	light1 = 0;
	light2 = 0;
	fan = 0; 
}

void lcd_init()
{
	cmd(0x38); //8-bit mode with 5*7 matrix
	cmd(0x0c); //display on with cursor =>(0ch=curson off; 0fh=cursor blinking; 0eh=cursor on
	cmd(0x01); //clear the screen
	cmd(0x06); //shift cursor =>(06h right--> ; 04h <---left; default=right--->)
	cmd(0x80); //position =>(80=1st row 1st position; c0=2nd row 1st position)
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

void lcdPrint(unsigned char *p)
{
 	while(*p)
	character(*p++);
}

void number(int n)
{
	unsigned char i=1,arr[5];
	if(n<0)
	{
		character('-'); //signed representation for negative number
		n=-n; //taking magnitude
	}
	if(n==0)
		character('0');
	while(n>0)
	{
		arr[i]=(n%10)+48;
		n=n/10;
		i++;
	}
	for(--i;i>0;i--) //problem with >=; remedy=> initialize i=1 and stop at i>0
		character(arr[i]); 	
}

void delay(int d)
{
	unsigned char i;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--); //500usec
		for(i=250;i>0;i--); //500usec
	}
}
//----------------------End of LCD Code-------------------------------------------------//

//------------------------GSM Code------------------------------------------------//
void uart_init()
{
	TMOD=0x20;
	TH1=0xFD;
	SCON=0x50;
	TR1=1;
//	IE=0x90;
}

void Tx(unsigned char ch)
{
	SBUF=ch;
	while(TI==0);
	TI=0;
}

void serialPrint(unsigned char *ptr)
{
	while(*ptr)
	Tx(*ptr++);
}