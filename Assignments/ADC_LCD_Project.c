/*-----------------------------------------------------------------------------
 Title: ADC_LCD Project
-----------------------------------------------------------------------------
 Program Details:
 * NOTE: This program contains portions of code shared by Dr. Farid Farahmand 
 * in previous labs; I have simply tied them together and added to them.

 * The purpose of this program is to use the ADC chip on our board to convert 
 * an analog signal into a digital signal. We use a voltage divider circuit
 * to input different voltages into an analog port where the value is then
 * quantified into a digital value. The resulting value is then sent to PORTB,
 * our output port, to be displayed on an LCD. 
 * The second portion of this project utilizes a photo-resistor to measure the
 * light intensity in the room. Again, the analog value is converted via the 
 * ADC and the resulting Lux value is then displayed on the LCD. 
 
 Inputs: PORTAbits.RA0
 Outputs: PORTB
 Setup: PIC18F47K42 Curiosity Nano Board

 Date: April 29, 2024
 File Dependencies / Libraries: It is required to include the
 ADC_LCD_config.h, the ADC_LCD_definitions.h, as well as the ADC_LCD_func_def.h 
 header files.
 Software: MPLAB X IDE v6.20; XC-8 compiler
 Author: Dr. Farid Farahmand; Jessie Lopez
 Versions:
       V1.0: Original
 Useful links:
 Datasheet for PIC: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 ----------------------------------------------------------------------------*/ 
/*-------------------------------
  Initialization
---------------------------------*/
#include <xc.h> // must have this
#include "ADC_LCD_config.h"
#include "ADC_LCD_func_def.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*-------------------------------
 Main Program
---------------------------------*/  
void main(void)
{     
    while (1){
    PORTD = 0; ANSELD = 0; TRISD = 0; //used for RS and E of LCD
    PORTB = 0; ANSELB = 0; TRISB = 0; //used for LCD
    ADC_Init();               /* Initializing the ADC*/
    LCD_Init();               /* Initializing the 16x2 LCD */
    //OSCCON=0x72;            /* Use Internal Oscillator with Frequency 8MHZ */   
        ADCON0bits.GO=1;
        while (ADCON0bits.GO); //stay here until conversion is done (when ADCON0bits.Go goes to 0)
                               // once its done, it converts input to decimal 
/* PART A: Use this portion of code for potentiometer and comment out PART B: */
      digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
      voltage = digital*((float)Vref/(float)(4096));
      PORTB = ADRESL;          
/*This is used to convert integer value to ASCII string*/
      sprintf(data,"%.2f",voltage);
      strcat(data," V");             /*Concatenate result and unit to print*/
      LCD_String_xy(1,0,"Input Voltage"); 
      LCD_String_xy(2,0,data);   /*Display string at location(row,location).*/ 
        
/*  PART B: Use this for Lux part and comment out PART A */
//        digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
//        voltage = digital*((float)Vref/(float)(4096));
//        if (voltage < 0.075)
//            Lux = 220*voltage;
//        else if(voltage> 0.075 && voltage <= 0.850){
//            Lux = 508.71*voltage - 32;
//                if (Lux < 0)
//                Lux = 0;}
//        else if (voltage > 0.850 && voltage < 1.15)
//            Lux = 1971*voltage - 1500;
//        else if (voltage > 1.2)
//            Lux = 1971*voltage - 1600;
//    /*This is used to convert integer value to ASCII string*/
//        sprintf(data,"%.2f",Lux);
//        LCD_String_xy(1,0,"Input Light"); 
////  LCD_String_xy(1,0,"Embedded System!");    /* Display string at location(row,location). */
//                                              /*   This function passes string to display */  
//        LCD_String_xy(2,0,Lux_1);   /*Display string at location(row,location). */
//                                    /* This function passes string to display */
//        LCD_String_xy(2,5,data);    /*Display string at location(row,location). */  
    }
}
