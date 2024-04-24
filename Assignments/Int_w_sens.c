/*-----------------------------------------------------------------------------
 Title: Interfacing w/ Sensors Project
-----------------------------------------------------------------------------
 Program Details:
 * The purpose of this program is to design a security box that uses two
 * photo-resistors to enter a two digit security code in HEX. If the code 
 * entered is correct, a motor will be activated to open the box. If the code 
 * entered is incorrect, a buzzer will sound. A 7-segment display is used to 
 * display each number after it is entered.
 
 Inputs: EM_SW, enter_button, Num_1, Num_2
 Outputs: SYS_LED, EM_LED, buzzer, relay_mod, PORTD for 7-Segment Display
 Setup: PIC18F47K42 Curiosity Nano Board

 Date: April 19, 2024
 File Dependencies / Libraries: It is required to include the
 Int_w_sens_functions.h header file, as well as the Int_w_sens_config.h header 
 file.
 Software: MPLAB X IDE v6.20; XC-8 compiler
 Author: Jessie Lopez
 Versions:
       V1.0: Original
 Useful links:
 Datasheet for PIC: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 ----------------------------------------------------------------------------*/ 
/*-------------------------------
  Initialization
---------------------------------*/
#include <xc.h>
#include <stdio.h>
#include "Int_w_sens_config.h"
#include "Int_w_sens_functions.h"
#include "Int_w_sens_def.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
/*-------------------------------
 Main Program
---------------------------------*/    
__at(0x100)int main () { 
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0;    // PORTD all outputs
    PORTB = 0; LATB = 0; ANSELB = 0; TRISB = 0x27; //PORTB,0 used for interrupt;
        //PORTB5 is enter button input,PORTB6 & 7 used for input of secret #
    PORTB = 0;
    SYS_LED = 1;        // Turns on system LED 
    INTERRUPT_Initialize(); 
    count = 1;
    while(1){
start:        a = 0;
 dig_1:     if (Num_1 == 1){
                count++;
                goto seven_seg;
            }
            else if (enter_button == 1){
                store_1();
                goto dig_2;
            }
            else goto dig_1;
            
dig_2:      if (Num_2 == 1){
                count++;   
                goto seven_seg;
            }
            else if (enter_button == 1){
                store_2();
                goto next;
            }
            else goto dig_2;
// Checking if code entered matches secret code
 next:   if (num_ent == Secret_Code){
        relay_mod = 1; // turn on motor
        __delay_ms(800);
        relay_mod = 0; // turn off motor
        __delay_ms(500);
    }
    else {
        buzzer = 1;
        __delay_ms(800);
        buzzer = 0;
    }
// Displaying count number
seven_seg: {
    if (count == 2){
        PORTD = 0x3F;
       __delay_ms(500);
    }
    if (count == 3){
        PORTD = 0x06;
       __delay_ms(500);
    }
    else if (count == 4){
        PORTD = 0x5B;
       __delay_ms(500);
    }
    else if (count == 5){
        PORTD = 0x4F;
       __delay_ms(500);
    }
    else if (count == 6){
        PORTD = 0x66;
        __delay_ms(500);
    }
    if (count != 0 && a == 0 && enter_button == 0)
        goto dig_1;
    else if (count != 0 && a == 1 && enter_button == 0)
        goto dig_2;
  }
    return 0;
    }
}
