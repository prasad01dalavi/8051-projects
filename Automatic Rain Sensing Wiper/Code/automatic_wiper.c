#include<reg51.h>
#define smotor P2
void wiper();
void delay(unsigned int d);

sbit rain_sensor=P1^0;



void main()
{
	rain_sensor=1;		//making as input port
	smotor=0x00;

	for(;;)
	{
		while(rain_sensor==1);	 //wait till the rain is not present
		wiper(); 				//run the motor
	}
}

void wiper()
{
	for(;;)
	{
		smotor=0x09;
		delay(1000);
		smotor=0x0c;
		delay(1000);
		smotor=0x06;
		delay(1000);
		smotor=0x03;
	//	delay(1000);
	//	smotor=0x09;
	//	delay(1000);
	
	//	smotor=0x03;
	//	delay(1000);
		smotor=0x06;
		delay(1000);
		smotor=0x0c;
		delay(1000);
		smotor=0x09;
		delay(1000);
	}		   

}


void delay(unsigned int d)
{
 	unsigned char i;
	for(;d>0;d--)
	{
	 for(i=255;i>0;i--);
	 for(i=255;i>0;i--);
	}

}


 

