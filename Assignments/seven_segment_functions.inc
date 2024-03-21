;-----------------------------------
; Setup
;-----------------------------------
    PSECT absdata,abs,ovrld        ; Do not change
    ORG	0X10
;------------------------------------
; Program Constants
;------------------------------------
    REG4	    EQU	0X04
    REG08	    EQU 0X08
    REG09	    EQU 0X09
    REG10	    EQU 0X10
    REG11	    EQU 0X11
    Inner_loop	    EQU 255	; Used for delay 
    Outer_loop	    EQU 255	; Used for delay
;-------------------------------------
; Call Functions
;-------------------------------------
_setupPortD:	; Programming 7-segment as output
    BANKSEL	PORTD 
    CLRF	PORTD ;Init PORTD
    BANKSEL	LATD ;Data Latch
    CLRF	LATD 
    BANKSEL	ANSELD 
    CLRF	ANSELD ;digital I/O
    BANKSEL	TRISD 
    MOVLW	0b00000000 ;Set RD[7:0] as outputs
    MOVWF	TRISD 
    RETURN

_setupPortB:	; Programming switches as inputs
    BANKSEL	PORTB 
    CLRF	PORTB ;Init PORTB
    BANKSEL	LATB ;Data Latch
    CLRF	LATB 
    BANKSEL	ANSELB 
    CLRF	ANSELB ;digital I/O
    BANKSEL	TRISB 
    MOVLW	0b00011000 ; Set RB[3,4] as inputs
    MOVWF	TRISB 
    RETURN

loop_Delay:	; Function for delay
    MOVLW       Inner_loop
    MOVWF       REG08	    ; Register 0x08 = 255d, max value for dual loop
    MOVLW       Outer_loop
    MOVWF       REG09	    ; Register 0x09 = 255d, max value for dual loop
_loop1:
    DECF        REG08,1
    BNZ         _loop1
    MOVLW	Inner_loop ; reload inner loop when outer loop decrements
    MOVWF	REG08
    DECF        REG09,1	   ; outer loop
    BNZ        _loop1
    RETURN
    
Zero:		; Function for loading the pointer & displaying 0
    MOVLW	0X0F	
    MOVWF	REG4
    MOVLW   0X00
    MOVWF   TBLPTRU
    MOVLW   0X02
    MOVWF   TBLPTRH
    MOVLW   0X00
    MOVWF   TBLPTRL
    TBLRD*  
    MOVFF   TABLAT,PORTD
    RETURN
    