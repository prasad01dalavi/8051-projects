#include<reg51.h>

//sensor outputs(input to the controller)
sbit s0=P1^0;
sbit s1=P1^1;
sbit s2=P1^2;
sbit s3=P1^3;
sbit s4=P1^4;

//motor ic control (input) bits
sbit m1=P3^0;
sbit m2=P3^1;
sbit m3=P3^2;
sbit m4=P3^3;

void main()
{
	P1=0xff;			//i/p port
	P3=0x00;			//o/p port

	for(;;)			   //forever loop
	{
	
		while((s0==1)&&(s1==1)&&(s2==0)&&(s3==1)&&(s4==1))	   //straight
		{
			m1=1;
			m2=0;
			m3=1;
			m4=0;
		}	
		
		while((s0==1)&&(s1==1)&&(s2==0)&&(s3==0)&&(s4==1))	   //slight right
		{
			m1=1;
			m2=0;
			m3=0;
			m4=0;
		}
		
		while((s0==1)&&(s1==1)&&(s2==0)&&(s3==0)&&(s4==0))	  //immediate and 90 degree right
		{
			m1=1;
			m2=0;
			m3=0;
			m4=1;
		}
		
		while((s0==1)&&(s1==0)&&(s2==0)&&(s3==1)&&(s4==1))	//slight let
		{
			m1=0;
			m2=0;
			m3=1;
			m4=0;
		}

		while((s0==0)&&(s1==0)&&(s2==0)&&(s3==1)&&(s4==1))	   //immediate and 90 degree left
		{
			m1=0;
			m2=1;
			m3=1;
			m4=0;
		}

		while((s0==0)&&(s1==0)&&(s2==0)&&(s3==0)&&(s4==1))	  //rare case of very immediate left
		{
			m1=0;
			m2=1;
			m3=1;
			m4=0;
		}

		while((s0==1)&&(s1==0)&&(s2==0)&&(s3==0)&&(s4==0))	  //rare case of very immediate right
		{
			m1=1;
			m2=0;
			m3=0;
			m4=1;
		}

		while((s0==0)&&(s1==0)&&(s2==0)&&(s3==0)&&(s4==0))	 //take left or right (all sensor on line)
		{
			m1=1;
			m2=0;
			m3=1;
			m4=0;
		}
		
	}
}