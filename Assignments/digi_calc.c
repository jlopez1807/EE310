/*-----------------------------------------------------------------------------
 Title: Digital Calculator Project
-----------------------------------------------------------------------------
 Program Details:
 The purpose of this program is to act as a digital calculator solving 4 math
 operations (+,-,*,/) in DECIMAL. The result shown on LED's is the binary 
 equivalent of the decimal answer. The range of digits is from 1-9 where each 
 digit must be preceeded by a zero. Negative numbers are displayed in 
 2's complement. 
 
 Inputs: X_Input_REG, Y_Input_REG
 Outputs: Display_Results_REG
 Setup: PIC18F47K42 Curiosity Nano Board

 Date: April 6, 2024
 File Dependencies / Libraries: It is required to include the
 digi_calc_functions.h header file, as well as the digi_calc.h header file.
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
#include "digi_calc.h"
#include "digi_calc_functions.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
/*---------------------------------
  Program Constants
 ----------------------------------*/
unsigned char a;    // place holder variable
unsigned char c;    // place holder variable
unsigned char input; 
unsigned char digit;
unsigned char answer;
unsigned char oper __attribute ((address (0x03)));
unsigned char X_Input_REG __attribute ((address (0x30)));
unsigned char Y_Input_REG __attribute ((address (0x40)));
unsigned char Display_Results_REG __attribute ((address (0x10)));
/*--------------------------------
  Program Functions Declaration 
 ---------------------------------*/  
void rst_result (unsigned char);
void store_val_X(unsigned char);
void store_val_Y(unsigned char);
void solve (unsigned char);
/*--------------------------------
 Main Program
 ---------------------------------*/
__at(0x20)int main () {     // Program built at 0x20 in program memory
    PORTD = 0; LATD = 0; ANSELD = 0; TRISD = 0;
    PORTB = 0; LATB = 0; ANSELB = 0; TRISB = 0x0F;
    a = 0;
    c = 0;
    while(1){
 start:
        PORTB = 0x10;       // Checking first column
        input = PORTB & 0x0F;   // Masking unused bits
             if (input == 1){
                digit = 1;
                goto eqn_builder;
            }
             else if (input == 2){
                digit = 4;
                goto eqn_builder;
            }
             else if (input == 4){
                digit = 7;
                goto eqn_builder;
            }
             else if (input == 8){   // '*' sign resets calculator
              rst_result(input);
              goto start;
            }
             else goto next_col_2;
next_col_2:                     // Checking second column
            PORTB = 0x20;
            input = PORTB & 0x0F;   // Masking unused bits
             if (input == 1){
                digit = 2;
                goto eqn_builder;
            }
             else if (input == 2){
                digit = 5;
                goto eqn_builder;
            }
             else if (input == 4){
                digit = 8;
                goto eqn_builder;
            }
             else if (input == 8){   // Numbers must be preceeded by a zero
                    if (a == 5 && c == 1){
                        digit = 0;
                        goto eqn_builder;
                    }
                    else{   
                        a = 5;
                        __delay_ms(500);
                        goto start;}
            }
             else goto next_col_3;
next_col_3:                       // Checking third column
            PORTB = 0x40;
            input = PORTB & 0x0F;
             if (input == 1){
                digit = 3;
                goto eqn_builder;
            }
             else if (input == 2){
                digit = 6;
                goto eqn_builder;
            }
             else if (input == 4){
                digit = 9;
                goto eqn_builder;
            }
             else goto start;
// Code for the calls
wait_rst:   PORTB = 0x10;           // waiting for reset to be pressed
            input = PORTB & 0x0F;
             if (input == 0x08){
                rst_result(input);
                goto start;
            }
             else goto wait_rst;
            
eqn_builder: if (a == 5 && c == 0){ // zeros has been pressed, second # stored
                store_val_X(digit);
                a = 0; 
                goto next_col_4;           
            }
             else if (a == 5 && c == 1){
                store_val_Y(digit);
                goto pound_sign;
            }
             else goto start;
next_col_4:  PORTB = 0x80;         // determining mathematical operation
             input = PORTB & 0x0F;
              if (input == 1){
                oper = 0;
                goto start;
            }
              else if (input == 2){
                oper = 1;
                goto start;
            }
              else if (input == 4){
                oper = 2;
                goto start;
            }
              else if (input == 8){
                oper = 3;
                goto start;
            }
              else goto next_col_4;
pound_sign:                         // waiting for # sign & displaying results
            PORTB = 0x40;
            input = PORTB & 0x08;
             if (input == 8){
                solve(oper);
                Display_Results_REG = answer;
                PORTD = answer;
                goto wait_rst;
            }
             else if (input != 8){      // if '*' is pressed reset program
                PORTB = 0x10;
                input = PORTB & 0x0F;
                    if (input == 0x08){
                        rst_result(input);
                        goto start;
                    }
             else goto pound_sign;
            }   
    }
    return 0;
}