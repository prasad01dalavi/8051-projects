//---------Automatic Railway Crossing Gate C code-------------//

#include<reg51.h>
#define smotor P1  //unipolar stepper motor is connected to P1
				   //as they are going to operate simultaneously  we are using same port pins to control both of them

//function declaration or function prototype
void stepper_clock();
void stepper_antiClock();	
void delay(unsigned int d);		

//variable declartion for signal connections to port pins
sbit redLed1 = P2^0;
sbit redLed2 = P2^1;

sbit greenLed1 = P2^2;
sbit greenLed2 = P2^3;

//----------------ISR----------------------//

void sensor1() interrupt 0	//INT0 
{
	//when sensor 1 is detected red leds should be turned on and gate should be closed by rotating the motors in clockwise direction
	greenLed1 = greenLed2 = 0; //first turn off the green leds
	redLed1	= redLed2 = 1; //turned on the red leds to close the gate
	stepper_clock();   //calling a function to close the gate i.e. stepper motor will be rotated clockwise for 90
}

void sensor2() interrupt 2	//INT1
{
	//when sensor 2 is detected the green leds should be on and gate should also be opened by rotating motors in anti clockwise direction
	redLed1	= redLed2 = 0;	//first turn off the red leds
	greenLed1 = greenLed2 = 1;	//turned on the green leds to open the gate
    stepper_antiClock();  //calling a function to open the gate i.e. stepper motor will be rotated anti-clockwise for 90
}
//--------------End of ISR -------------------//

void main()
{
 	redLed1 = redLed2 = greenLed1 = greenLed2 = 0;	//making them as output port
	smotor = 0x00;	//making stepper motor port i.e. P1 as output

	//lets do interrupt setting because sensors are connected to INT0 and INT1 (external interrupts)
	EA = 1;	//Enable access to use the interrupts
	EX0 = 1;  //enable interrupt for INT0 i.e. sensor 1
	EX1 = 1;	//enable interrupt for INT1 i.e. sensor 2
	//Interrupt setting is done here we can use our interruts now
	while(1);	//waiting for the train i.e. until sensors to be detected
}

void stepper_clock() //write your code to rotate the motor 90 clockwise
{
		smotor=0x09;
		delay(1000);
		smotor=0x0c;
		delay(1000);
}

void stepper_antiClock()  //write your code to rotate the motor 90 anti-clockwise
{
		smotor=0x0c;
		delay(1000);
		smotor=0x09;
		delay(1000);	
}

void delay(unsigned int d)	  //function definition
{
 	unsigned char i;
	for(;d>0;d--)
	{
	 for(i=255;i>0;i--);
	 for(i=255;i>0;i--);
	}

}
