;-----------------------------------------------------------------------------
; Title: Seven Segment Display Project
;-----------------------------------------------------------------------------
; Program Details:
; The purpose of this program is to use two switches to either count up or 
; down in the Hexadecimal number range (i.e. 0-F) depending on which switch is
; pressed. The corresponding number is also displayed on a seven-segment 
; display. Switch A is assumed to be the switch that counts up, switch B is 
; assumed to be the switch that counts down. 
    
; Inputs: RB3,4
; Outputs: RD0-7
; Setup: PIC18F47K42 Curiosity Nano Board

; Date: March 21, 2024
; File Dependencies / Libraries: It is required to include the
; seven_segment_functions.inc as well as the seven_segment_source.inc
; files in the same file location as Header file.
; Software: MPLAB X IDE v6.20
; Author: Jessie Lopez
; Versions:
;       V1.0: Original
; Useful links:
;  Datasheet for PIC: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;  Datasheet for 7-segment: https://www.inolux-corp.com/datasheet/Display/Through-Hole-Display/SingleDigit/INND-TS56%20Series_V1.0.pdf 
;  PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;----------------------------------------------------------------------------- 
;---------------------
; Initialization
;---------------------
#include "./seven_segment_source.inc"
#include "./seven_segment_functions.inc"
#include <xc.inc>
;---------------------
; Program Constants
;---------------------
REG4	EQU 0X4
REG7	EQU 0X7
;---------------------
; Program Organization
;---------------------
    PSECT absdata,abs,ovrld       ; Do not change
    ORG          0                ;Reset vector
    GOTO        _setup
    ORG	0X200
MYDATA1:    DB	0X3F, 0X06, 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X67, 0X77, 0X7C, 0X39, 0X5E, 0X79, 0X71	
;---------------------
; Main Program
;---------------------
_setup:	CALL _setupPortD    ; Setting portD as output
        CALL _setupPortB    ; Setting portB,3 and portB,4 as inputs
; Loading a register with value equal to vslue of both switches on
	MOVLW	0X18
	MOVWF	REG7
	GOTO RSET_CNT	    ; Loading the pointer and displaying 0 as default

; Determining which case circuit is in
CASE:	MOVFF	PORTB,WREG
	CPFSEQ	REG7
	GOTO	CK_OFF
	GOTO	RSET_CNT	; both switches are on
CK_OFF:	MOVLW	0X00
	CPFSEQ	PORTB
	GOTO	CK_SW
	GOTO	RSET_CNT	; both switches are off
CK_SW:	BTFSS	PORTB,3		; only switch A is on
	GOTO	CNT_DOWN	; only switch B is on
	GOTO	INC_CNT
	
; Holds last value for switch A
HOLD_A:	BTFSC	PORTB,3	    
	GOTO	INC_CNT
	BTFSC	PORTB,4
	GOTO	RSET_CNT
	GOTO	HOLD_A
; Holds F when switching from F to 0 on count up without additional delay
HOLD_AF0:   BTFSC   PORTB,3	    
	    GOTO    CNT_UP
	    BTFSC   PORTB,4
	    GOTO    RSET_CNT
	    GOTO    HOLD_AF0
; Holds last value for switch B
HOLD_B:	BTFSC	PORTB,4	    
	GOTO	DEC_CNT
	BTFSC	PORTB,3
	GOTO	RSET_CNT
	GOTO	HOLD_B
; This part of code sets up the pointer for displaying 0 by default or for RESET
RSET_CNT:	CALL	Zero
		CALL	loop_Delay
		GOTO	CASE
; Function to count up (switch A is on)
CNT_UP:		BTFSC	PORTB,4
		GOTO	CASE
		BTFSS	PORTB,3
		GOTO	HOLD_AF0
		CALL	Zero
		CALL	loop_Delay		
INC_CNT:	BTFSS	PORTB,3	  ;loop for counting up by incrementing pointer
		GOTO	HOLD_A
		BTFSC	PORTB,4
		GOTO	RSET_CNT
		TBLRD+* 
		MOVFF	TABLAT,PORTD
		CALL	loop_Delay
		DECF	REG4
		BNZ	INC_CNT
		BRA	CNT_UP    ;reloading the pointer to start again @ 0
		
; Function to count down (switch B is on)	
CNT_DOWN:	BTFSS	PORTB,4
		GOTO	CASE
		BTFSC	PORTB,3
		GOTO	RSET_CNT
		MOVLW	0XF
		MOVWF	REG4
		MOVLW	0X00
		MOVWF   TBLPTRU
		MOVLW   0X02
		MOVWF   TBLPTRH
		MOVLW   0X0F
		MOVWF   TBLPTRL
		TBLRD* 
		MOVFF   TABLAT,PORTD
		CALL	loop_Delay
		GOTO	DEC_CNT
DEC_CNT:	BTFSS	PORTB,4	 ;loop for counting down by decrementing pointer
		GOTO	HOLD_B
		BTFSC	PORTB,3
		GOTO	RSET_CNT
		DECF	TBLPTR
		TBLRD* 
		MOVFF	TABLAT,PORTD
		CALL	loop_Delay
		DECF	REG4
		BNZ	DEC_CNT
		BRA	CNT_DOWN   ;reloading the pointer to start again @ F	
END	
