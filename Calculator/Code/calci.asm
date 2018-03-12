 N1 EQU 30H ;#!Ram byte location holds first oeprand
 N2 EQU 31H ;#!Ram byte locations holds second operand 
 OP EQU 32H ;#!Ram byte location holds operation ascii code
 R EQU 33H ;#!Ram byte location holds result
 SIGN EQU 34H ;#!Ram byte location holds result's sign
 TEMP EQU 35H ;#!Ram byte location holds temporary data
 DIF BIT 0AH ;#!Digit input flag bit(0,1,2,3,4,5,6,7,8,9)
 OIF BIT 0BH ;#!Operator input flag bit(+,-,*,/)
 AIF BIT 0CH ;#!Assignment operator flag bit(=)
 ORG 0H ;#!Orginate at rom address 0000H
 AJMP MAIN ;#!Absoulte jump to main routine
 ;--------------------------------------------------------------------------------------------------#!
 ;--------------------------------------------------------------------------------------------------#!
 ORG 30H ;#!Orginate at rom adress 0030H
 MAIN: ACALL memoryInit ;#!CALL memory initialization subroutine
 ACALL lcdInit ;#!CALL lcd initialization subroutine
 ACALL readInput ;#!CALL read input subroutine
 ACALL calculateResult ;#!CALL calculate result subroutine
 ACAll printOutput ;#!CALL print output subroutine
 AJMP MAIN ;#!return and start the program once more
 ;--------------------------------------------------------------------------------------------------#!
 ;--------------------------------------------------------------------------------------------------#!
 memoryInit:
 CLR A ;#!Clear ACC
 MOV N1,A ;#!Clear first operand location in ram
 MOV N2,A ;#!Clear second operand location in ram
 MOV OP,A ;#!Clear operation location in ram
 MOV R,A ;#!Clear result location in ram
 MOV SIGN,A ;#!Clear sign location in ram
 MOV TEMP,A ;#!Clear result location in ram
 CLR C ;#!Clear carry bit
 MOV DIF,C ;#!Clear digit flag bit
 MOV OIF,C ;#!Clear operator flag bit
 MOV AIF,C ;#!Clear assignment operator flag bit
 RET ;#!return to MAIN routine
 ;--------------------------------------------------------------------------------------------------#!
 lcdInit:
 MOV DPTR,#MYCOM ;#!Set DPTR to look-up table's address
 C1: CLR A ;#!Clear ACC
 MOVC A,@A+DPTR ;#!Get code from look-up table and placing it into ACC
 ACALL COMMWRT ;#!CALL LCD command write subroutine
 INC DPTR ;#!Increment DPTR <DPTR = DPTR + 1>
 JZ C2 ;#!Jump if ACC = ZERO to end of loop
 SJMP C1 ;#!If not start the loop again
 C2: RET ;#!return to MAIN routine
 ;--------------------------------------------------------------------------------------------------#!
 readInput:
 ACALL readKey ;#!CALL keypad subroutine to read one number
 ACALL validateInput ;#!CALL subroutine to confirm of valid number
 JB OIF,ERROR ;#!Jump to ERROR routine if OIF = 1
 JB AIF,ERROR ;#!Jump to ERROR routine if AIF = 1
 ACALL DATAWRT ;#!CALL subroutine so pass data to LCD
 ANL A,#0FH ;#!Masking to convert number form ASCII to BCD
 MOV N1,A ;#!Save number at RAM
 ACALL readKey ;#!CALL keypad subroutine to read operation(+,-,*,/)
 ACALL validateInput ;#!CALL subroutine to confirm of valid operation
 JB DIF,ERROR ;#!Jump to ERROR routine if DIF = 1
 JB AIF,ERROR ;#!Jump to ERROR routine if AIF = 1
 ACALL DATAWRT ;#!CALL subroutine so pass data to LCD
 MOV OP,A ;#!Save operation at RAM
 ACALL readKey ;#!CALL keypad subroutine to read one number
 ACALL validateInput ;#!CALL subroutine to confirm of valid number
 JB OIF,ERROR ;#!Jump to ERROR routine if OIF = 1
 JB AIF,ERROR ;#!Jump to ERROR routine if AIF = 1
 ACALL DATAWRT ;#!CALL subroutine so pass data to LCD
 ANL A,#0FH ;#!Masking to convert number form ASCII to BCD

 MOV N2,A ;#!Save number at RAM
 ACALL readKey ;#!CALL keypad subroutine to read assignemt operation (=)
 ACALL validateInput ;#!CALL subroutine to confirm of valid input
 JB DIF,ERROR ;#!Jump to ERROR routine if OIF = 1
 JB OIF,ERROR ;#!Jump to ERROR routine if OIF = 1
 ACALL DATAWRT ;#!CALL subroutine so pass data to LCD
 AJMP NOERROR ;#!Jump to END of subroutine if NO ERROR
 ERROR: ACALL ERRFUN ;#!CALL ERROR subroutine if input was not valid
 LJMP MAIN ;#!return if ERROR to start of program again
 NOERROR:RET ;#!return if NO ERROR normally to MAIN
 ;--------------------------------------------------------------------------------------------------#!
 calculateResult:
 MOV A,N1 ;#!Copy first number to ACC
 MOV B,N2 ;#!Copy second number to B
 MOV R7,OP ;#!Copy operation to R7
 CJNE R7,#"+",NEXT11 ;#!Check firstly if the operation was "+", if not jump
 ADD A,B ;#!If so, Add the two operands
 MOV R,A ;#!Save result at RAM
 MOV SIGN,#"+" ;#!Set result sign to positive
 RET ;#!return to MAIN
 NEXT11: CJNE R7,#"-",NEXT22 ;#!Check secondly if the operation was "-", if not jump
 SUBB A,B ;#!If so, Sub the two operands
 JC NIGATIV ;#!Check on carry for nigative result
 MOV R,A ;#!Save result at RAM
 MOV SIGN,#"+" ;#!Set result sign to positive
 RET ;#!return to MAIN
 NIGATIV:CPL A ;#!Convert to 1's complement if negativ
 INC A ;#!Incremet to get 2's complement
 MOV R,A ;#!Save result at RAM
 MOV SIGN,#"-" ;#!Set result sign to nigative
 RET ;#!return to MAIN
 NEXT22: CJNE R7,#"*",NEXT33 ;#!Check thirdly if the operation was "*", if not jump
 MUL AB ;#!If so, Multibly the oeprands
 MOV R,A ;#!Save result in RAM
 MOV SIGN,#"+" ;#!Set result sign to positive
 RET ;#!return to MAIN
 NEXT33: CJNE R7,#"/",NEXT44 ;#!Check finally if the operation was "/", if not jump
 MOV TEMP,B ;#!Copy to TEMP location
 DIV AB ;#!Carry out division to the two operands
 MOV R,A ;#!Save the quotient in RAM
 MOV A,#0AH ;#!Fill ACC with 10
 MUL AB ;#!Multibly reminder by 10
 MOV B,TEMP ;#!Get the second number again
 DIV AB ;#!Divide again
 MOV TEMP,A ;#!Save the result into RAM
 MOV SIGN,#"+" ;#!Set result sign to positive
 NEXT44: RET ;#!return to MAIN
 ;--------------------------------------------------------------------------------------------------#!
 printOutput:
 MOV R7,TEMP ;#!Copy TEMP contents to R7
 CJNE R7,#0,POINTED ;#!Check if the result was decimal point number
 MOV R6,SIGN ;#!Copy SIGN contents to R6
 CJNE R6,#"+",SIGNED ;#!Check on sign if positive or negative
 RETURN: MOV A,R ;#!If positive, Copy R contents to ACC
 MOV B,#0AH ;#!Fill B register with 10
 DIV AB ;#!Divide result by 100
 JZ LESSTEN ;#!jump if reminder equals 0 (result<10)
 ORL A,#30H ;#!If not convert first digit from BIN to ASCII
 ACALL DATAWRT ;#!CALL subroutine, so pass data to LCD
 MOV A,B ;#!Get the second digit
 ORL A,#30H ;#!Convert it from BIN to ASCII
 ACALL DATAWRT ;#!CALL subroutine, so pass data to LCD
 AJMP DONE ;#!Done!!
 LESSTEN:MOV A,B ;#!If the result was one digit
 ORL A,#30H ;#!Convert from Bin to ASCII
 ACALL DATAWRT ;#!CALL subroutine, so print data on LCD
 AJMP DONE ;#!Done!!
 POINTED:MOV A,R ;#!For decimal point numbers
 ORL A,#30H ;#!Convert from Bin to ASCII
 ACALL DATAWRT ;#!CALL subroutine, so print data on LCD
 MOV A,#"." ;#!MOV dot ascii code to ACC
 ACALL DATAWRT ;#!CALL subroutine, so print data on LCD
 MOV A,TEMP ;#!Get the digit that after point
 ORL A,#30H ;#!Convert it from BIN to ASCII


 ACALL DATAWRT ;#!CALL subroutine, so print data on LCD
 AJMP DONE ;#!Done!!
 SIGNED: MOV A,#"-" ;#!IF signed mov "-" ASCII to ACC
 ACALL DATAWRT ;#!CALL subroutine, so print data on LCD
 AJMP RETURN ;#!Return to carryout the remain process
 DONE: ACALL LDELAY ;#!Give data some time on LCD
 ACALL LDELAY ;#!More time again
 RET ;#!return to MIAN routine
;--------------------------------------------------------------------------------------------------#!
 validateInput: ;#!Detemine and set flags for each kind of input
 CJNE A,#"+", next1 ;#!Check if input is add operator,
 AJMP found ;#!Jump to found if input match with (+,-,*,/)
 next1: CJNE A,#"-", next2 ;#!Check if input is sub operator,
 AJMP found ;#!Jump to found if input match with (+,-,*,/)
 next2: CJNE A,#"*", next3 ;#!Check if input is multiply operator
 AJMP found ;#!Jump to found if input match with (+,-,*,/)
 next3: CJNE A,#"/", next4 ;#!Check if input is add operator,
 AJMP found ;#!Jump to found if input match with (+,-,*,/)
 next4: CJNE A,#"=", next5 ;#!Check if input is assignment operator
 CLR DIF ;#!CLR digit input flag for non numbers
 CLR OIF ;#!CLR operation input flag for non operation
 SETB AIF ;#!SET assignment input flag for (=)
 RET ;#!return to MAIN
 next5: SETB DIF ;#!SET digit input flag for numbers
 CLR OIF ;#!CLR operation input flag for non operatio
 CLR AIF ;#!CLR assignment input flag if not (=)
 RET ;#!return to MAIN
 found: CLR DIF ;#!CLR digit input flag for non numbers
 SETB OIF ;#!SET operation input flag for any of operations
 CLR AIF ;#!CLR assignment input flag if not (=)
 RET ;#!return
 ;--------------------------------------------------------------------------------------------------#!
 readKey:
 MOV P1,#0FFH ;#!Make P1 an input port
 K1: MOV P3,#0 ;#!Ground all rows at once
 MOV A,P1 ;#!Read all colums
 ANL A,#00001111B ;#!Masked unused bits
 CJNE A,#00001111B,K1 ;#!Till all keys release
 K2: LCALL SDELAY ;#!Call 20 msec delay
 MOV A,P1 ;#!See if any key is pressed
 ANL A,#00001111B ;#!Mask unused bits
 CJNE A,#00001111B,OVER ;#!Key pressed, find row
 SJMP K2 ;#!Check till key pressed
 OVER: LCALL SDELAY ;#!Wait 20 msec debounce time
 MOV A,P1 ;#!Check key closure
 ANL A,#00001111B ;#!Mask unused bits
 CJNE A,#00001111B,OVER1 ;#!Key pressed, find row
 SJMP K2 ;#!If none, keep polling
 OVER1: MOV P3,#11111110B ;#!Ground row 0
 MOV A,P1 ;#!Read all columns
 ANL A,#00001111B ;#!Mask unused bits
 CJNE A,#00001111B,ROW_0 ;#!Key row 0, find col.
 MOV P3,#11111101B ;#!Ground row 1
 MOV A,P1 ;#!Read all columns
 ANL A,#00001111B ;#!Mask unused bits
 CJNE A,#00001111B,ROW_1 ;#!Key row 1, find col.
 MOV P3,#11111011B ;#!Ground row 2
 MOV A,P1 ;#!Read all columns
 ANL A,#00001111B ;#!Mask unused bits
 CJNE A,#00001111B,ROW_2 ;#!Key row 2, find col.
 MOV P3,#11110111B ;#!Ground row 3
 MOV A,P1 ;#!Read all columns
 ANL A,#00001111B ;#!Mask unused bits
 CJNE A,#00001111B,ROW_3 ;#!Key row 3, find col.
 LJMP K2 ;#!If none, false input, repeat
 ROW_0: MOV DPTR,#KCODE0 ;#!Set DPTR=start of row 0
 SJMP FIND ;#!Find col. Key belongs to
 ROW_1: MOV DPTR,#KCODE1 ;#!Set DPTR=start of row
 SJMP FIND ;#!Find col. Key belongs to
 ROW_2: MOV DPTR,#KCODE2 ;#!Set DPTR=start of row 2
 SJMP FIND ;#!Find col. Key belongs to
 ROW_3: MOV DPTR,#KCODE3 ;#!Set DPTR=start of row 3
 FIND: RRC A ;#!See if any CY bit low
 JNC MATCH ;#!If zero, get ASCII code


 INC DPTR ;#!Point to next col. addr
 SJMP FIND ;#!Keep searching
 MATCH: CLR A ;#!Set A=0 (match is found)
 MOVC A,@A+DPTR ;#!Get ASCII from table
 RET ;#!return
 ;--------------------------------------------------------------------------------------------------#!
 ERRFUN: ACALL CLS ;#!CALL subroutine to clear screen
 MOV DPTR,#ERRMSG ;#!Set DPTR to look-up table's address
 E1: CLR A ;#!Clear ACC
 MOVC A,@A+DPTR ;#!Get code from look-up table and placing it into ACC
 ACALL DATAWRT ;#!CALL LCD command write subroutine
 ACALL SDELAY ;#!Give LCD some time
 INC DPTR ;#!Increment DPTR <DPTR = DPTR + 1>
 JZ E2 ;#!Jump if ACC = ZERO to end of loop
 SJMP E1 ;#!If not start the loop again
 E2: ACALL LDELAY ;#!Give some time to msg on screen
 ACALL CLS ;#!CALL subroutine to clear screen again
 RET ;#!return
 ;--------------------------------------------------------------------------------------------------#!
 COMMWRT: ;#!Send command to LCD
 MOV P2,A ;#!Copy reg A to P1
 CLR P0.0 ;#!RS=0 for command
 CLR P0.1 ;#!R/W=0 for write
 SETB P0.2 ;#!E=1 for high pulse
 ACALL SDELAY ;#!Give LCD some time
 CLR P0.2 ;#!E=0 for H-to-L pulse
 RET ;#!return to caller
 ;--------------------------------------------------------------------------------------------------#!
 DATAWRT: ;#!Write data to LCD
 MOV P2,A ;#!Copy reg A to port 1
 SETB P0.0 ;#!RS=1 for data
 CLR P0.1 ;#!R/W=0 for write
 SETB P0.2 ;#!E=1 for high pulse
 ACALL SDELAY ;#!Give LCD some time
 CLR P0.2 ;#!E=0 for H-to-L pulse
 RET ;#!return to caller
 ;--------------------------------------------------------------------------------------------------#!
 CLS: ;#!CLear Screen subroutine
 MOV A,#01H ;#!Fill ACC with the command code
 ACALL COMMWRT ;#!CALL subroutine to send code to lCD
 RET ;#!return to caller
 ;--------------------------------------------------------------------------------------------------#!
 SDELAY: ;#!Short Delay subroutine
 MOV R0,#50 ;#!R0 = 50
 S11: MOV R1,#255 ;#!R1 = 255
 S21: DJNZ R1,S21 ;#!stay here until R1 becomes ZERO
 DJNZ R0,S11 ;#!
 RET ;#!return to caller
 ;--------------------------------------------------------------------------------------------------#!
 LDELAY: ;#!Long Delay Subroutine
 MOV R0,#7 ;#!R0 = 7
 L33: MOV R1,#190 ;#!R1 = 190
 L22: MOV R2,#255 ;#!R2 = 255
 L11: DJNZ R2,L11 ;#!stay here until R2 becomes ZERO
 DJNZ R1,L22 ;#!
 DJNZ R0,L33 ;#!
 RET ;#!return to caller
 ;--------------------------------------------------------------------------------------------------#!
 ;--------------------------------------------------------------------------------------------------#!
 ;-------COMMANDS LOOK-UP TABLE FOR LCD INITIALIZATION
 ORG 300H
 MYCOM: DB 38H,0FH,01H,06H,80H,0
 ;-------ASCII LOOK-UP TABLE FOR EACH ROW
 KCODE0: DB "7","8","9","/" ;#!ROW 0
 KCODE1: DB "4","5","6","*" ;#!ROW 1
 KCODE2: DB "1","2","3","-" ;#!ROW 2
 KCODE3: DB 0,"0","=","+" ;#!ROW 3
 ;-------ERROR MASSAGE FOR ANY INVALID INPUT
 ERRMSG: DB "ERROR!!",0
 END