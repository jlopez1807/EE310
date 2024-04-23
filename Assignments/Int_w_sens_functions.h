/* This program contains the functions used in Int_w_sens.c program. */
/*-------------------------------
 Definitions
---------------------------------*/
#define _XTAL_FREQ 4000000             // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#define EM_LED PORTBbits.RB4
#define buzzer PORTBbits.RB6
/*-------------------------------
 Program Inputs
---------------------------------*/
    unsigned char a;
    unsigned char count;
    unsigned char digit_1;
    unsigned char digit_2;
    unsigned char num_ent;
// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT0), base(0x4008)) INT0_ISR (void) {

    if (PIR1bits.INT0IF == 1) {       // Has an interrupt occurred?
            EM_LED = 1;        // Turns ON Emergency LED
            __delay_ms(500);         // Delays for 0.25 second    
            buzzer = 1;        // Play melody on buzzer
            __delay_ms(250);
            buzzer = 0;
            __delay_ms(80);
            buzzer = 1;
            __delay_ms(80);
            buzzer = 0;
            __delay_ms(80);
            buzzer = 1;
            __delay_ms(100);
            buzzer = 0;
            __delay_ms(100);
            buzzer = 1;
            __delay_ms(400);
            buzzer = 0;
            __delay_ms(450);
            buzzer = 1;
            __delay_ms(100);
            buzzer = 0;
            __delay_ms(80);
            buzzer = 1;             // Turns OFF Emergency LED    
            __delay_ms(250);        // Delays for 0.25 second
            buzzer = 0;
            EM_LED = 0;
            __delay_ms(250);
            PIR1bits.INT0IF = 0;        // Clear interrupt flag
    }
}
// Interrupt initialization
void INTERRUPT_Initialize (void)
{
    INTCON0bits.IPEN = 1;  // Interrupt priority enable
    INTCON0bits.GIEH = 1;  // Global int. enable &/or enable high priority int.
    INTCON0bits.GIEL = 1;       // Enable low priority interrupts
    INTCON0bits.INT0EDG = 1;    // Interrupt on rising edge
    IPR1bits.INT0IP = 1;        // Interrupt priority reg; priority set high
    PIE1bits.INT0IE = 1;        // Periph. int. enable; external int. 0 enabled
    PIR1bits.INT0IF = 0;   // Peripheral int. request reg.; set int. flag = 0
    IVTBASEU = 0X00;       // These were given:
    IVTBASEH = 0X40;       //
    IVTBASEL = 0X08;       //
}
// Storing first digit
void store_1 (void){ 
    if ((count - 2) < 0)
        digit_1 = 0;
    else {
    digit_1 = (count-2) * 0x10;
    a = 1;
    count = 1;
    PORTD = 0;
    }
    __delay_ms(500);
}
// Storing second digit
void store_2 (void){
    if ((count - 2) < 0)
        digit_2 = 0;
    else {digit_2 = count - 2;
        num_ent = digit_1 + digit_2;
        a = 0;
        count = 1;
        PORTD = 0;
    }
   __delay_ms(500);
}