 #include<reg51.h>

 //functions' declaration

 void delay(int);

 sbit s0=P3^0;
 sbit s1=P3^1;
 sbit s2=P3^2;
 sbit s3=P3^3;

 sbit in1=P2^0;
 sbit in2=P2^1;
 sbit in3=P2^2;
 sbit in4=P2^3;	 

 unsigned char m=3;
													   
void main()
{

	in1=in2=in3=in4=0;
	s0=s1=s2=s3=1;
	delay(1000);

	for(;;)
	{
		while(s3==0 && s2==0 && s1==1 && s0==0)
		{
			in1=1;
			in2=0;
			in3=1;
			in4=0;			
		}

		while(s3==0 && s2==1 && s1==0 && s0==0)
		{
			in1=0;
			in2=0;
			in3=1;
			in4=0;			
		}

		while(s3==0 && s2==0 && s1==1 && s0==0)
		{
			in1=1;
			in2=0;
			in3=1;
			in4=0;			
		}

		while(s3==0 && s2==1 && s1==1 && s0==0)
		{
			in1=1;
			in2=0;
			in3=1;
			in4=0;			
		}
		
		while(s3==1 && s2==0 && s1==0 && s0==0)
		{
			in1=0;
			in2=1;
			in3=0;
			in4=1;			
		}
		
		while(s3==0 && s2==1 && s1==0 && s0==1)
		{
			in1=0;
			in2=0;
			in3=0;
			in4=0;			
		}	
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


