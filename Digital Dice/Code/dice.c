#include<reg51.h>
#define seven_seg P1
void delay(unsigned int d);

sbit reset=P3^2;
sbit stop=P3^3;
sbit start=P3^4;


//unsigned char n,arr[]={0x06,0x5b,0x4f,0x66,0x6d,0x7d};        // Hex values corresponding to 1 to 6 for Common Cathode
unsigned char n,arr[]={0xf9,0xa4,0xb0,0x99,0x92,0x82};		   //
void ex0()interrupt 0
{
	seven_seg=0xc0;											//to reset in any condition

	while(start==1);								   //wait till start button is pressed
}

void ex1() interrupt 2
{
	while(start==1 && reset==1)						 
	seven_seg=arr[n]; 										 //stop and show till start or reset button is pressed
}

void main()
{
	seven_seg=0xc0;									   //initially zero
	start=1;
	EA=1;										 //interrupt enable bit
	EX0=1;										 //INTO is enabled
	EX1=1;										 //INT1 is enabled
												 //wait till the start button is pressed
	while(start==1);

    while(1)
    {
       	for(n=0;n<6;n++)					  
		{
			seven_seg=arr[n];					  //showing 1 to 6 continuously
    		delay(100);
  		}
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
