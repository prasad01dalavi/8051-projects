			 #include <reg51.h>

#include <string.h>

sbit rs = P2^7;                   // rs pin of LCD

sbit en = P2^6;                   // en pin of LCD

sbit led1 = P2^0;

sbit led2 = P2^1;

sbit led3 = P2^2;

sbit rely = P2^4;

sbit led4 = P2^3;

sbit buz = P2^5;

char str1[5] = "12345";           // default password

char str2[5];

int j;

 

void writecmd(unsigned char a);   // function initializations

void writedata(unsigned char b);

void busy(void);

void writestr(unsigned char *s);

void wait_for_2sec()

  {

int y,z;

        for(y=0;y<50;y++)

          for(z=0;z<10000;z++);

  }   

void writecmd(unsigned char a)

 {

       busy();

       rs = 0;      

       P0 = a;      

       en = 1;

       en = 0;

 }

void writedata(unsigned char b)

 {

       busy();

       rs = 1;      

       P0 = b;      

       en = 1;

       en = 0;      

 }

void busy()

{

       int k;

       for(k=0;k<1500;k++);

}

void writestr(unsigned char *s)

{

       unsigned char l,z;

       l = strlen(s);

       for(z=0;z<l;z++)

       {

         writedata(*s);

         s++;       

       }

}

void keydly()

  {

    int x,y;

       for(x=0;x<100;x++)

         for(y=0;y<1000;y++);

   }

 void sound_buzzer(unsigned int t) // produce beep sound

  {

        unsigned int p;

        for(p=0;p<t;p++)                 // beep sound once or twice

          {

               buz = 1;

               keydly();

               buz=0;

               keydly();

        }

  }                 

void main()

  {

       int t=0,i,p=0,f=0,r,y,c=0,l=0,s=0,a=0;

       P3=0x00;                                 // ports P3 and P2 output ports

       P2=0x00;     

       writecmd(0x3C);                          // configure LCD

       writecmd(0x0E);

next:  r=0;i=0;                            

       writecmd(0x01);

       if(l==0) writestr("To Lock System");     // display message

       else writestr("To UnLock System");

       writecmd(0xC0);     

       writestr("Enter Password");

       wait_for_2sec();                         // after 2 second                

       writecmd(0xC6);

       writestr("________");                    // show space to enter password  

       writecmd(0xC6);     

loop:  P1=0xF0;

       while(P1==0xF0);    

       while(P1!=0xF0)

         {

              P1=0xFE;

              if(P1==0xEE)

                {

                     if(f==0)

                       {

                           writedata(0x2A);

                           str2[i]=0x31;

                       }

                     else

                       {

                           writedata(0x31);

                           str1[i]=0x31;

                        }

                     t=1;

                }

              else if(P1==0xDE)

                {

                     if(f==0)

                       {

                            writedata(0x2A);

                           str2[i]=0x34;

                       }

                     else

                       {

                           writedata(0x34);

                           str1[i]=0x34;

                       }

                     t=1;

                  }

              else if(P1==0xBE)

                {

                     if(f==0)

                       {

                           writedata(0x2A);

                           str2[i]=0x37;

                       }

                     else

                       {

                           writedata(0x37);

                           str1[i]=0x37;

                       }

                     t=1;

                  }

              else if(P1==0x7E)

                {

                     writecmd(0x01);

                     if(f==1)

                       {

                            writestr("new password is");

                            writecmd(0xC0);

                           for(y=0;y<5;y++) writedata(str1[y]);                  

                           f=0;

                       }

                     else

                      {

                       if(i==5)

                         {

                            for(j=0;j<5;j++)

                             {

                                  if(str1[j]==str2[j]) p++;                      

                                  else

                                    {

                                         led1=0;

                                         led4=1;

                                         writestr("wrong password");

                                         sound_buzzer(2);

                                         a++;

                                         break;

                                    }

                             }

                            if(p==5)

                              {

                                   c++;

                                   if((c%2)==1)

                                    {

                                         l=1;

                                         rely=1;

                                         led2=0;

                                         led3=1;

                                         writestr("System Locked");

                                    }

                                   else

                                    {

                                         l=0;

                                         rely=0;

                                         led3=0;

                                         led2=1;

                                         writestr("System Unlocked");

                                    }

                                    led1=1;

                                    led4=0;

                                    sound_buzzer(1);

                                }

                              }

                            else 

                             {

                                   led1=0;

                                  led4=1;

                                  writestr("wrong password");

                                  sound_buzzer(2);

                                  a++;

                             }

                            }     

                            if(a==3)

                             {

                                  writecmd(0x80);

                                  writestr("System HANG!!..");

                                  writecmd(0xC0);

                                  writestr(" * Press RESET *");

                                  sound_buzzer(1);

                             }

                     r=1;

                     t=1;         

                     p=0;                

                }                                                                   

              if(t==1) break;

              P1=0xFD;

              if(P1==0xED)

                {

                     if(f==0) {writedata(0x2A);str2[i]=0x32;}

                     else {writedata(0x32);str1[i]=0x32;}

                     t=1;

                }

              else if(P1==0xDD)

                {

                     if(f==0) {writedata(0x2A);str2[i]=0x35;}

                     else {writedata(0x35);str1[i]=0x35;}

                     t=1;

                }

              else if(P1==0xBD)

                {

                     if(f==0) {writedata(0x2A);str2[i]=0x38;}

                     else {writedata(0x38);str1[i]=0x38;}

                     t=1;

                }                         

              else if(P1==0x7D)

                {

                     if(f==0) {writedata(0x2A);str2[i]=0x30;}

                     else {writedata(0x30);str1[i]=0x30;}

                     t=1;

                }                 

 

              if(t==1) break;

              P1=0xFB;

              if(P1==0xEB)

                {

                     if(f==0) {writedata(0x2A);str2[i]=0x33;}

                     else {writedata(0x33);str1[i]=0x33;}

                     t=1;

                 }

              else if(P1==0xDB)

                {

                     if(f==0) {writedata(0x2A);str2[i]=0x36;}

                     else {writedata(0x36);str1[i]=0x36;}

                     t=1;

                }

              else if(P1==0xBB)

                {

                     if(f==0) {writedata(0x2A);str2[i]=0x39;}

                     else {writedata(0x39);str1[i]=0x39;}

                     t=1;

                }

              else if(P1==0x7B)

                {   

                  writecmd(0x01);

                     if(s==0)

                       {

                           writestr("set new password");

                           writecmd(0xC0);

                           writestr("(5 digit):");

                           f=1;                      

                           i=-1;

                           s++;

                       }

                     else

        {

          writestr("password can be set");

          writecmd(0xC3);

          writestr("only once");

          r=1;

        }

                     t=1;                      

                 }                

              if(t==1) break;

        }

        keydly();     

        i++;     

   t=0;

        if(a==3) while(1);

   if (r==0) goto loop;

   else goto next;

  }