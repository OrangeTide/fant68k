ACIASTAT	equ	$FF1000
ACIADATA	equ	$FF1002
SIM_EXIT	equ	$FF7FFC

START   ORG     $60000

*-------Code for output------
	LEA	ACIADATA,A0	Loads serial tx into address register A0
        LEA     MESSAGE,A1      Loads MESSAGE into address register A1
OUT	MOVE.B  (A1)+,D0
	CMP.B	#0,D0
	BEQ	DONE		Bail out on NUL terminator
*-------TODO: we should poll to see that serial port is ready
	MOVE.B	D0,(A0)		Output character
	JMP	OUT
*-------Exit the simulator---
DONE	LEA	SIM_EXIT,A0
	MOVE.L	D0,(A0)		Halt Simulator
	JMP	DONE

MESSAGE DC.B    'HELLO WORLD',13,10,0

        END     START
