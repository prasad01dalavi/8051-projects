//-------------------Automatic Railway Crossing Gate assembly code--------------------//
					
					//variable declartion for signal connections to port pins
					SMOTOR EQU P1  			
					REDLED1 BIT P2.0
					REDLED2 BIT P2.1
					GREENLED1 BIT P2.2
					GREENLED2 BIT P2.3

					ORG 0H
					LJMP MAIN	  //very important step while using interrupts otherwise interrupts are executed automatically
								  //this is because program execution stars normally from org 0h
		
			   		ORG 200H		//origin of the code should be upto 500H because 4KB ROM allows upto 7FFF H
					//making them as output port
			MAIN:	CLR REDLED1
					CLR REDLED2
					CLR GREENLED1
					CLR GREENLED2
					MOV SMOTOR,#00H	 //making stepper motor port i.e. P1 as output
					
					//lets do interrupt setting because sensors are connected to INT0 and INT1 (external interrupts)
					SETB EA		 //Enable access to use the interrupts
					SETB EX0	 //enable interrupt for INT0 i.e. sensor 1
					SETB EX1	 //enable interrupt for INT1 i.e. sensor 2
					SJMP $	  //waiting for the train i.e. until sensors to be detected

//------------------------Subroutiones-------------------------------//	
  STEPPER_CLOCK:    MOV SMOTOR,#09H		   //write your code to rotate the motor 90 clockwise
					ACALL DELAY
					ACALL DELAY
					MOV SMOTOR,#0CH
					ACALL DELAY
					ACALL DELAY
					RET	
					
STEPPER_ANTICLOCK:  MOV SMOTOR,#0CH	   //write your code to rotate the motor 90 anti-clockwise
					ACALL DELAY
					ACALL DELAY
					MOV SMOTOR,#09H
					ACALL DELAY
					ACALL DELAY
					RET	
					
			DELAY:  MOV R1,#5
			AGAIN:	MOV R2,#50
					DJNZ R2,$			//2*50 = 100 usec
					DJNZ R1,AGAIN		//2*10*100usec = 1000 usec = 1 msec
					RET 
//------------------------End of Subroutiones-------------------------------//	

//------------------------------ISR-----------------------------//	

					ORG 0003H		//INT0 
					//when sensor 1 is detected red leds should be turned on and gate should be closed by rotating the motors in clockwise direction
					CLR GREENLED1
					CLR GREENLED2
					
					SETB REDLED1
					SETB REDLED2
					
					LCALL STEPPER_CLOCK
					RETI
					
					ORG 0013H	 //INT1
					//when sensor 2 is detected the green leds should be on and gate should also be opened by rotating motors in anti clockwise direction
					CLR REDLED1
					CLR REDLED2
					
					SETB GREENLED1
					SETB GREENLED2
					
					LCALL STEPPER_ANTICLOCK
					RETI
//------------------------------End of ISR-----------------------------//	
					END