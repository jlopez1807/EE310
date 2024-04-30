/* This program contains the definitions for ADC_LCD_Project.c program.*/

#include "ADC_LCD_definitions.h"
/* This program contains the functions for program: ADC_LCD_Project.c */
/****************************Functions********************************/
void ADC_Init(void)
{
    TRISAbits.TRISA0 = 1;   //setting A0 as input
    ANSELAbits.ANSELA0 = 1; //setting pin to analog input pin (if 0 its digital I/O)
    ADCON0bits.FM = 1;      //Adjust format; align right
    ADCON0bits.CS = 1;      //clock source: ADCRC dedicated oscillator
    ADCLK = 0x00;           //ADC CLOCK SELECTION REG.: 000000 = FOSC/2
    ADPCH = 0X00; //ADPCH: ADC POSITIVE CHANNEL SELECTION REG.; 000000 = ANA0
    ADCAP = 0X00; //ADC ADDITIONAL SAMPLE CAPACITOR SELECTION REG.; no add. cap.
    ADREF = 0X00;//ADC REFERENCE SELECTION REG.; 00 = VREF+ is connected to VDD
    ADPREL = 0X00;//ADC PRECHARGE TIME CONTROL REGISTER (LOW BYTE);NOT INCLUD.
    ADPREH = 0X00;//ADPREH: ADC PRECHARGE TIME CONTROL REGISTER (HIGH BYTE);NOT INC.
    ADACQL = 0X00;//ADC ACQUISITION TIME CONTROL REGISTER (LOW BYTE);NOT INC.
    ADACQH = 0X00;//ADACQH: ADC ACQUISITION TIME CONTROL REGISTER (HIGH BYTE); NOT INC.
    ADCON0bits.ON = 1;//ON: ADC Enable bit
    ADRESH = 0; //ADRESH: ADC RESULT REGISTER HIGH, FM = 0; '0' means bit is cleared

}
void LCD_Init()
{
    MSdelay(15);           /* 15ms,16x2 LCD Power on delay */
    LCD_Port = 0x00;       /* this is TRISB; Set PORTB as output PORT for LCD data(D0-D7) pins */
    LCD_Control = 0x00;    /* this is TRISD; Set PORTD as output PORT LCD Control(RS,EN) Pins */
    LCD_Command(0x01);     /* clear display screen */
    LCD_Command(0x38);     /* uses 2 line and initialize 5*7 matrix of LCD */
    LCD_Command(0x0c);     /* display on cursor off */
    LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
}

//void LCD_Clear()
//{
//        LCD_Command(0x01); /* clear display screen */
//}

void LCD_Command(char cmd)
{
    ldata= cmd;            /* this is LATB; LATB = 0; Send data to PORT as a command for LCD */   
    RS = 0;                /* Instruction Register is selected */
    EN = 1;                /* High-to-Low pulse on Enable pin to latch data */ 
    NOP();
    EN = 0;
    MSdelay(3); 
}

void LCD_Char(char dat)
{
    ldata= dat;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    EN=1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    EN=0;
    MSdelay(1);
}


void LCD_String(const char *msg)
{
    while((*msg)!=0)
    {       
      LCD_Char(*msg);
      msg++;    
        }
}

void LCD_String_xy(char row,char pos,const char *msg)
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);

}

/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}
