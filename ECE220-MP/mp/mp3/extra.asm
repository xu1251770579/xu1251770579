	.ORIG x4800

	.FILL x15	; M W F
	.FILL NAME1	; "A"
	.FILL x0290	; 10, 13, 15

	.FILL x01	; F
	.FILL NAME2	; "B"
	.FILL x8114	; 8, 10, 14

	.FILL x1F	; M Tu W Th F
	.FILL NAME3	; "C"
	.FILL x0424	; 8, 11, 16

	.FILL x0B	; Tu Th F
	.FILL NAME4	; "D"
	.FILL x0128	; 9, 11, 14

	.FILL x1C	; M Tu W
	.FILL NAME5	; "E"
	.FILL x8A08	; 9, 15, 17

	.FILL x12	; M Th
	.FILL NAME6	; "F"
	.FILL x8C80	; 13, 16, 17

	.FILL #-1	; end of list

NAME1	.STRINGZ "A"
NAME2	.STRINGZ "B"
NAME3	.STRINGZ "C"
NAME4	.STRINGZ "D"
NAME5	.STRINGZ "E"
NAME6	.STRINGZ "F"

	.END

