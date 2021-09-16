;
;
;
;The register will use in PRINT_SLOT in following rules
;R0 is have the output value
;R1 contain the give number 
;R2 have the Zero ASCLL 
;
; as always we start at x3000
	.ORIG	x3000
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
	
	
;
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
