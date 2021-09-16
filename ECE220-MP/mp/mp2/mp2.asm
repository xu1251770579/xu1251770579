
; lets start
;this is use to translate the unkown line of schedule into 
;a know array which has two dimension
;first we should test the day, if the day is -1 the code will terminate 
;or store the day then 
;R0-> the bit to test 
;R1-> pointer of the array
;R2-> temporary(75 OR the vector)
;R3-> pointer of the event
;R4-> a number for each days (or a counter for five days)
;R5->tem
;R6-> NEXT EVENT VECTOR ADDRESS
;R7->tem
;********************************************************************
;my code is here
    .ORIG x3000

    LD R2,SEVENTYFIVE           ;
    LD R1,ARRAY_ADDRESS         ;
    AND R0,R0,#0                ;
INI_AGAINE                      ;    this is to initial the 75 array positions
    STR R0,R1,#0                ;
    ADD R1,R1,#1                ;
    ADD R2,R2,#-1               ;
    BRp INI_AGAINE              ;

    LD R3,EVENT_ADDRESS ;initial the first event address into R3
    LD R1,ARRAY_ADDRESS ;NEW 2.0


IF_TERMINATE
    LDR R2,R3,#0     ; test if the vector is -1 end the event
    BRn PRINT_START

    ADD R6,R3,#0
AGAINE_INCREMENT
    ADD R6,R6,#1            ; because the days vector can be zero 
    LDR R5,R6,#0            ; we need find the next days vector out the COUNT_IN 
    BRnp AGAINE_INCREMENT   ; we load the next vector address in R6
    ADD R6,R6,#2

	ADD R5,R6,#-1
    LDR R5,R5,#0
    BRn INVALID_SLOT  ;because the days vector could be zero
    ADD R5,R5,#-15	  ;so we need to test before count_in  
    BRzp INVALID_SLOT ;test the slot_invalid

    AND R0,R0,#0
    ADD R0,R0,#1 ;first bit to test is 1
    AND R4,R4,#0
    ADD R4,R4,#4 ;the first day Monday is four 

NEXT_DAY
    AND R5,R0,R2   ; 
    BRp COUNT_IN   ;we put the event in array    
NEXT_INCREMENT    
    ADD R0,R0,R0    
    ADD R4,R4,#-1
    BRzp NEXT_DAY
    ADD R3,R6,#0
    BR  IF_TERMINATE

COUNT_IN
    ADD R5,R6,#-1
    LDR R5,R5,#0
    ADD R7,R5,R5    ;
    ADD R7,R7,R7    ;just mult 5 hah 
    ADD R5,R7,R5    ; maybe the easyest way

    ADD R5,R5,R4
    ADD R5,R5,R1    ;calculate the array position to put in
    LDR R7,R5,#0    ;test if there already a event in the position
    BRnp CONFLICT
    ADD R7,R3,#1    
    STR R7,R5,#0    ;put the pointer to string in
    BR  NEXT_INCREMENT

INVALID_SLOT
ADD R0,R3,#1
PUTS	
LEA R0,SLOT_ERROR
PUTS
HALT

CONFLICT
ADD R0,R3,#1
PUTS
LEA R0,CONFLICT_ERROR
PUTS
HALT

PRINT_START
	JSR PRINT_SCHEDULE
	HALT

;this is the code for print
;there is a big loop for each line 
;and there is a small loop in big loop for each event
;
;R0-> the output
;R1-> the link to the subruotine
;R2-> counter to the line
;R3-> tem
;R4-> counter of the day
;R5-> the pinter to the initial array
;R6-> tem
;R7-> don't use
;
;

PRINT_SCHEDULE
    ;to make it a subroutine
	ST	R0,RZERO3  ;for not change the register 
	ST	R2,RTWO3   ;so we use some memory to store 
	ST	R3,RTHREE3 ;the orig-register's value
	ST	R4,RFOUR3
	ST	R5,RFIVE3
	ST	R6,RSIX3
	ST	R7,RSEVEN3

    LEA R1,NULL             ;
    JSR PRINT_CENTERED      ;
    LD R0,VERTICAL          ;   
    OUT                     ;PRINT the first line of days
    LEA R1,MONDAY           ;
    JSR PRINT_CENTERED      ;
    LD R0,VERTICAL          ;
    OUT                     
    LEA R1,TUESDAY
    JSR PRINT_CENTERED
    LD R0,VERTICAL
    OUT
    LEA R1,WEDNESDAY
    JSR PRINT_CENTERED
    LD R0,VERTICAL
    OUT
    LEA R1,THURSDAY
    JSR PRINT_CENTERED
    LD R0,VERTICAL
    OUT
    LEA R1,FRIDAY
    JSR PRINT_CENTERED
    LD R0,LINE_FEED
    OUT
    AND R2,R2,#0
    LD  R5,ARRAY_ADDRESS

NEW_LINE 
    ADD R1,R2,#0
    ADD R2,R2,#1

    ADD R3,R2,#-15 ;end the print when R2+1 = 16
    BRp END_PRINT

    JSR PRINT_SLOT
    AND R4,R4,#0  ; to be the counter of the day
    ADD R3,R1,R1  ;
    ADD R3,R3,R3  ; MULT 5  
    ADD R3,R3,R1  ;

ANOTHER_EVENT
    LD R0,VERTICAL  ;every event begin with a vertical line
    OUT
    ADD R6,R3,R4
    ADD R6,R6,R5  ;calculate the array position
    LDR R1,R6,#0  ;load the pinter in R1
    BRz PRINT_NULL
    JSR PRINT_CENTERED
    BR  PASS_NULL
PRINT_NULL
    LEA R1,NULL
    JSR PRINT_CENTERED
PASS_NULL 
    ADD R4,R4,#1  ;increment the day counter 
    ADD R6,R4,#-5 ;test if the line is end
    BRn ANOTHER_EVENT
    LD R0,LINE_FEED     ;finish this line
    OUT
    BR NEW_LINE
END_PRINT	
LD	R0,RZERO3   ;retrive the the bits 
	LD	R2,RTWO3    ;in the register
	LD	R3,RTHREE3
	LD	R4,RFOUR3
	LD	R5,RFIVE3
	LD	R6,RSIX3
	LD	R7,RSEVEN3
	JMP	R7 			;jump back to the main code



RZERO3	.FILL	x0000  ;_2 is make the label different
RTWO3	.FILL	x0000  ;with subrutine one
RTHREE3	.FILL	x0000
RFOUR3	.FILL	x0000
RFIVE3	.FILL	x0000
RSIX3	.FILL	x0000
RSEVEN3	.FILL	x0000
    

    
;

    
;********************************************************************
    VERTICAL      .FILL x007C
    LINE_FEED     .FILL x000A
    NULL          .FILL x0000
    ARRAY_ADDRESS .FILL x3800
    EVENT_ADDRESS .FILL x4000
    SEVENTYFIVE   .FILL x004B
    SLOT_ERROR    .STRINGZ " has an invalid slot number.\n"
    CONFLICT_ERROR .STRINGZ " conflicts with an earlier event.\n"
    MONDAY        .STRINGZ "MONDAY"
    TUESDAY       .STRINGZ "TUESDAY"
    WEDNESDAY     .STRINGZ "WEDNESDAY"
    THURSDAY      .STRINGZ "THURSDAY"
    FRIDAY        .STRINGZ "FRIDAY"

;***********************************************************************
;subruotine of print_slot and print_centered
PRINT_SLOT	
		ST	R0,RZERO ;Load the orig-value of R0,R2, R7,R1  
		ST	R2,RTWO	 ; in memory RZERO,RTWO,RSEVEN,RONE
		ST	R7,RSEVEN ;make sure the register don't change 
		ST	R1,RONE   ;after the sub routine finished
				;
				;
				;
		LD	R2,ZERO	 ;put the ascll of 0 in R2
		LD	R0,SPACE ;put the ascll of space in R0
		TRAP	x21	 ;R0 and display space three times
		TRAP	x21
		TRAP	x21
		ADD	R1,R1,#-4 ;test for if the R1+6 more 
		BRn	LESSTEN	 ; than ten = R1-4 is negative
;
;
;
	MORETEN	ADD	R1,R1,#-10	;test for if the R1+6  
		BRz	TWENTY	  ; is equal to twenty
		BR	TEN
;
;
	TWENTY	ADD	R0,R2,#2  ;output the 2 and 0
		TRAP	x21	  ;for the twenty
		LD	R0,ZERO	
		TRAP	x21
		ADD	R1,R1,#14 ;get back to the orig-R1
		BR	FINISH
;
;	
	TEN	ADD	R0,R2,#1  ;output the 1
		TRAP	x21
		ADD	R1,R1,#10 ;put the number into 
		ADD	R0,R1,R2  ;correct ASCLL number 
		TRAP	x21	  ;output 
		ADD	R1,R1,#4  ;get back to the orig-R1
		BR	FINISH
;
	LESSTEN	LD	R0,ZERO   ;output 0
		TRAP	x21	  
		ADD	R1,R1,#10 ;output the correct 
		ADD	R0,R1,R2  ;number
		TRAP	x21
		
;
;
	FINISH	LD	R0,ZERO	;out put the two ZEROs
		TRAP	x21	;and two space
		TRAP	x21
		LD	R0,SPACE
		TRAP	x21
		TRAP	x21
		LD	R1,RONE
		LD	R0,RZERO ;get back to the orig-R0,R2,R7,R1
		LD	R2,RTWO  ;to make the register don't change 
		LD	R7,RSEVEN
		JMP	R7 ;get back to the main code
;
	SPACE	.FILL	x0020
	ZERO	.FILL	x0030
	RONE	.FILL	x0000
	RZERO	.FILL	x0000
	RTWO	.FILL	x0000
	RSEVEN	.FILL	x0000
	
	
;==================================================================================
;
;this is the subroutine that print a word
;in the center of the nine space
;
;
;my second assamble language program
;R1 is the orig address for the character
;R0 contain the output ASCLL
;R2 have the  character COUNTER
;R3 have the number of all space
;R4 have the number of the front space 
;R5 have the number of the trail space 
;R6 is the pointer to the current address of the character

	
PRINT_CENTERED
	ST	R0,RZERO_2  ;for not change the register 
	ST	R2,RTWO_2   ;so we use some memory to store 
	ST	R3,RTHREE ;the orig-register's value
	ST	R4,RFOUR
	ST	R5,RFIVE
	ST	R6,RSIX
	ST	R7,RSEVEN_2
	AND	R2,R2,#0  ;initial all the register
	AND R3,R3,#0
	AND	R4,R4,#0
	AND	R5,R5,#0
	AND	R6,R6,#0
	ADD	R6,R1,#-1 ;get the former address of the string
			  		;to adapt to follow loop(counter)
	ADD	R2,R2,#-9 ; when it go to the zero the string is 
					;more than nine characters
;
COUNTER	
	ADD	R6,R6,#1  ;pointer to the character
	LDR	R0,R6,#0  ;to test the end of the string
	BRz	PLUS_NINE ;jump to the test of nine
	ADD	R2,R2,#1  ;add the number of character number	
	BRz	OVER_NINE
	BR	COUNTER   ;again the loop
;
;
PLUS_NINE
	NOT	R3,R2	  ;to caculate the R3
	ADD	R3,R3,#1  ;the number of space
	ADD	R2,R2,#9  ;make the R2 to be the number of characters
;
;
LESS_NINE
	ADD	R4,R4,#1  ;increment the front space number
	ADD	R3,R3,#-2 ;caculate how many 2 in space number
	BRn	FRONT_SPACE ; to test the odd situation
	ADD	R5,R5,#1  ;increment the trail space number
	ADD	R3,R3,#0   ;to test the even situation
	BRz	FRONT_SPACE ;
	BR	LESS_NINE ;againe the loop
;
;
OVER_NINE
	AND	R2,R2,#0  ;only need to out nine character
	ADD	R2,R2,#9  ;set the number of character to nine
	BR	FRONT_SPACE ;begin to type	

FRONT_SPACE	
	LD	R0,SPACE_2  ;out the space front the string
	ADD	R4,R4,#-1
	BRn	NEXT
	TRAP 	x21
	BR	FRONT_SPACE
	
NEXT	AND	R6,R6,#0  ; out the string
	ADD	R6,R1,#0
AGAINE	ADD	R2,R2,#-1
	BRn	TRAIL_SPACE
	LDR	R0,R6,#0
	TRAP	x21
	ADD	R6,R6,#1
	BR	AGAINE

TRAIL_SPACE
	LD	R0,SPACE_2  ;out the space after the trial
	ADD	R5,R5,#-1
	BRn	FINISH_2
	TRAP	x21
	BR	TRAIL_SPACE
FINISH_2	
	LD	R0,RZERO_2   ;retrive the the bits 
	LD	R2,RTWO_2    ;in the register
	LD	R3,RTHREE
	LD	R4,RFOUR
	LD	R5,RFIVE
	LD	R6,RSIX
	LD	R7,RSEVEN_2
	JMP	R7 			;jump back to the main code

SPACE_2	.FILL	x0020
RZERO_2	.FILL	x0000  ;_2 is make the label different
RTWO_2	.FILL	x0000  ;with subrutine one
RTHREE	.FILL	x0000
RFOUR	.FILL	x0000
RFIVE	.FILL	x0000
RSIX	.FILL	x0000
RSEVEN_2	.FILL	x0000

    .END
