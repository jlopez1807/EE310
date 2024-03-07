;---------------------
; Program Details:
; The purpose of this program is to design a cooling and heating system. If 
; the measured temperature is greater than the reference temperature, the air
; will turn on. If the measured temperature is less than the reference
; temperature, the heat will come on. If the measured temperature is equal to
; the reference temperature, the system will turn off.

; Inputs: refTempInput, measuredTempInput
; Outputs: PORTD
; Date: March 6, 2024
; File Dependencies / Libraries: None 
; Compiler: xc8, 2.46
; Author: Jessie Lopez
; Versions:
;       V1: Initial Code
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 

;---------------------
; Initialization
;---------------------
#include "Assignment_4.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
#define measuredTempInput 15	; this is the input value
#define refTempInput 15		; this is the input value
  
;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld       ;Do not change
    ORG          0                ;Reset vector
   GOTO        _start1
    ORG          0020H            ;Begin assembly at 0020H
; Declaring variables used in program
refTemp		EQU 0X20
measuredTemp	EQU 0X21
contReg		EQU 0X22
ref_neg		EQU 0X80
minuend		EQU 0X10
val_save	EQU 0X32
refTemp_RMND_H	EQU 0X60
refTemp_RMND_M	EQU 0X61
refTemp_RMND_L	EQU 0X62
measuredTemp_RMND_H   EQU   0X70
measuredTemp_RMND_M   EQU   0X71
measuredTemp_RMND_L   EQU   0X72
dec_10	EQU 10
;Start of program
_start1:
   CLRF	    refTemp,0
   CLRF	    measuredTemp,0
   CLRF	    contReg,0
   MOVLW    0X00
   MOVWF    STATUS,1
   BCF	    STATUS,2,0
   MOVLW    0XF9
   MOVWF    TRISD
   MOVLW    measuredTempInput
   MOVWF    measuredTemp,0    
   MOVLW    ref_neg
   CPFSGT   measuredTemp   ;if measuredTemp is negative, skip and go to heat
   GOTO	PROCESS_1	   ;if measuredTemp is not negative, determine op. mode
   GOTO	LED_HEAT		
;Determining mode of operation
PROCESS_1:  MOVLW    refTempInput
	    MOVWF    refTemp,1	      
	    MOVFF   measuredTemp,WREG
	    SUBWF    refTemp,0
	    BZ	LED_OFF 
	    BN	LED_COOL
	    BNN	LED_HEAT
; setting contReg value 
LED_OFF:    MOVLW 0X00
	    MOVWF   contReg,0
	    GOTO PORTSET
LED_COOL:   MOVLW   0x02
	    MOVWF   contReg,0
	    GOTO PORTSET
LED_HEAT:   MOVLW   0X01
	    MOVWF   contReg,0
	    GOTO PORTSET
; this portion is used to assign the PORT from contReg value per requirements
PORTSET:    MOVLW 0X00	    
	    CPFSEQ  contReg	;If contReg is zero turn off portD
	    GOTO PORT_OR    ; otherwise check to see if contReg is 1.
	    GOTO PORT_OFF
PORT_OR:    MOVLW 0X01
	    CPFSEQ contReg	;If contReg is 1,  pin 1 of portD goes high.
	    GOTO PORT_COOL  ;otherwise, contReg is 2, pin 2 of portD goes high.
	    GOTO PORT_HEAT
; this is to set the port 
PORT_OFF:   MOVLW 0X00
	    MOVWF PORTD
	    GOTO NEXT
PORT_HEAT:  MOVLW 0X02
	    MOVWF PORTD
	    GOTO NEXT
PORT_COOL:  MOVLW 0X04
	    MOVWF PORTD
	    GOTO NEXT
; performing division & storing value of refTemp as a decimal in reg 0x60-0x62
NEXT:	    MOVLW    refTempInput
	    MOVWF    refTemp,1
	    MOVLW 0x00
	    MOVWF   STATUS
	    BCF	STATUS,2
	    MOVFF refTemp,WREG
	    MOVWF minuend
	    MOVLW dec_10
	    CLRF val_save
ref_high:  INCF val_save,1 ;loop to subtract 10d until carry bit in STATUS is 0.
	    SUBWF minuend
	    BC ref_high
	    ADDWF minuend
	    DECF val_save,1
	    MOVFF minuend,refTemp_RMND_H    ;storing LSB in register 60
	    MOVFF val_save,minuend
	    CLRF val_save
ref_low:   INCF val_save,1 ;loop to subtract 10d until carry bit in STATUS is 0.
	    SUBWF minuend
	    BC ref_low
	    ADDWF minuend
	    DECF val_save,1
	    MOVFF minuend,refTemp_RMND_M    ;storing 2nd bit in register 61
	    MOVFF val_save,refTemp_RMND_L   ;storing MSB in register 62
	    GOTO NEXT_2
;performing division & storing value of measuredTemp as decimal in reg 0x70-0x72	
NEXT_2:	    MOVLW 0x00
	    MOVWF   STATUS
	    BCF	STATUS,2
	    MOVLW   measuredTempInput
	    MOVWF   measuredTemp
	    CALL NEG_CHECK
	    MOVFF measuredTemp, WREG
	    MOVWF minuend
	    MOVLW dec_10
	    CLRF val_save
meas_high: INCF val_save,1 ;loop to subtract 10d until carry bit in STATUS is 0.
	    SUBWF minuend
	    BC meas_high
	    ADDWF minuend
	    DECF val_save,1
	    MOVFF minuend,measuredTemp_RMND_H	;storing LSB in register 70
	    MOVFF val_save,minuend
	    CLRF val_save
meas_Low:  INCF val_save,1 ;loop to subtract 10d until carry bit in STATUS is 0.
	    SUBWF minuend
	    BC meas_Low
	    ADDWF minuend
	    DECF val_save,1
	    MOVFF minuend,measuredTemp_RMND_M	;storing 2nd bit in register 71
	    MOVFF val_save,measuredTemp_RMND_L	;storing MSB in register 72
HERE:	GOTO HERE
; function to perform two's complement for negative values of measuredTemp
NEG_CHECK:
    MOVLW    ref_neg
    CPFSGT   measuredTemp
    RETURN
    BN TWOS_COMP
TWOS_COMP:
    NEGF measuredTemp,1
    RETURN
END