 #include<reg51.h>
 #define lcd P2
 #define dtmf P3

 //functions' declaration
 void init();
 void delay(int);
 void character(unsigned char);
 void cmd(unsigned char);
 void string(unsigned char *);
 void number(int);

 sbit rs=P0^0;
 sbit en=P0^1;

 sbit s0=P3^0;
 sbit s1=P3^1;
 sbit s2=P3^2;
 sbit s3=P3^3;

 sbit BULB1=P1^0;
 sbit BULB2=P1^1;
 sbit fan=P1^2;
 

 unsigned char m=3;
													   
void main()
{
	lcd=0x00;
	rs=0;
	en=0;

	BULB1=0;
	BULB2=0;
	fan=0;

	init();

	cmd(0x01);	
	string("   Welcome In");
	cmd(0xc0);
	string("   Smart Home");
	delay(1000);
	cmd(0x01);
	string("System is Ready");
	
	
	while(1)
	{
		while(s3==0 && s2==0 && s1==0 && s0==1)
		{
			BULB1=1;

			if(BULB1==1 && BULB2==0 && fan==0)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 OFF,FAN OFF");
				delay(1000);
			}
				
			if(BULB1==1 && BULB2==0 && fan==1)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 OFF,FAN ON");
				delay(1000);
			}

			if(BULB1==1 && BULB2==1 && fan==0)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 ON,FAN OFF");
				delay(1000);
			}  

			if(BULB1==1 && BULB2==1 && fan==1)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 ON,FAN ON");
				delay(1000);
			}
		}

		while(s3==0 && s2==0 && s1==1 && s0==0)
		{
			BULB1=0;

			if(BULB1==0 && BULB2==0 && fan==0)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 OFF,FAN OFF");
				delay(1000);
			}
				
			if(BULB1==0 && BULB2==0 && fan==1)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 OFF,FAN ON");
				delay(1000);
			}

			if(BULB1==0 && BULB2==1 && fan==0)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 ON,FAN OFF");
				delay(1000);
			}  

			if(BULB1==0 && BULB2==1 && fan==1)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 ON,FAN ON");
				delay(1000);
			}
		}

		while(s3==0 && s2==0 && s1==1 && s0==1)
		{
			BULB2=1;

			if(BULB1==0 && BULB2==1 && fan==0)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 ON,FAN OFF");
				delay(1000);
			}
				
			if(BULB1==0 && BULB2==1 && fan==1)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 ON,FAN ON");
				delay(1000);
			}

			if(BULB1==1 && BULB2==1 && fan==0)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 ON,FAN OFF");
				delay(1000);
			}  

			if(BULB1==1 && BULB2==1 && fan==1)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 ON,FAN ON");
				delay(1000);
			} 				   
		}

		while(s3==0 && s2==1 && s1==0 && s0==0)
		{
			BULB2=0;

			if(BULB1==0 && BULB2==0 && fan==0)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 OFF,FAN OFF");
				delay(1000);
			}
				
			if(BULB1==0 && BULB2==0 && fan==1)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 OFF,FAN ON");
				delay(1000);
			}

			if(BULB1==1 && BULB2==0 && fan==0)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 OFF,FAN OFF");
				delay(1000);
			}  

			if(BULB1==1 && BULB2==0 && fan==1)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 OFF,FAN ON");
				delay(1000);
			}

		}

		while(s3==0 && s2==1 && s1==0 && s0==1)
		{
			fan=1;
			
			if(BULB1==0 && BULB2==0 && fan==1)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 OFF,FAN ON");
				delay(1000);
			}
				
			if(BULB1==0 && BULB2==1 && fan==1)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 ON,FAN ON");
				delay(1000);
			}

			if(BULB1==1 && BULB2==0 && fan==1)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 OFF,FAN ON");
				delay(1000);
			}  

			if(BULB1==1 && BULB2==1 && fan==1)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 ON,FAN ON");
				delay(1000);
			}

		}

		while(s3==0 && s2==1 && s1==1 && s0==0)
		{
			fan=0;

			if(BULB1==0 && BULB2==0 && fan==0)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 OFF,FAN OFF");
				delay(1000);
			}
				
			if(BULB1==0 && BULB2==1 && fan==0)
			{
				cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 OFF");
				cmd(0xc0);
				string("BULB2 ON,FAN OFF");
				delay(1000);
			}

			if(BULB1==1 && BULB2==0 && fan==0)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 OFF,FAN OFF");
				delay(1000);
			}  

			if(BULB1==1 && BULB2==1 && fan==0)
			{
			 	cmd(0x01);
				cmd(0x80);
		   	 	string("BULB1 ON");
				cmd(0xc0);
				string("BULB2 ON,FAN OFF");
				delay(1000);
			}
		}
	}	
}

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

void delay(int d)
{
	unsigned char i;
	for(;d>0;d--)
	{
		for(i=250;i>0;i--);		//500usec
		for(i=250;i>0;i--);		//500usec
	}
}


