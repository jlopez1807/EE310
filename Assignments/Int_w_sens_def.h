/* This program contains the program constants for program "Int_w_sens.c" */
/*-------------------------------
 Definitions
---------------------------------*/
#define _XTAL_FREQ 4000000            // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#define EM_SW PORTBbits.RB0
#define Num_1 PORTBbits.RB1
#define Num_2 PORTBbits.RB2
#define SYS_LED PORTBbits.RB3
#define EM_LED PORTBbits.RB4
#define enter_button PORTBbits.RB5
#define buzzer PORTBbits.RB6
#define relay_mod PORTBbits.RB7
#define Secret_Code 0x30
#define FCY    _XTAL_FREQ/4
/*---------------------------------------
 Program Inputs / Function Initialization
------------------------------------------*/
    unsigned char a;            // Placeholder variable
    unsigned char count;        
    unsigned char digit_1 __attribute ((address (0x30)));
    unsigned char digit_2 __attribute ((address (0x34)));
    unsigned char num_ent __attribute ((address (0x38)));
    void seven_seg (unsigned char count1);
    void store_1 (void);
    void store_2 (void);

