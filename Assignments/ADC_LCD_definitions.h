/* This program contains the definitions for ADC_LCD_Project.c program.*/
/*-------------------------------
 Definitions
---------------------------------*/
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
#define Vref 5.0 // voltage reference 
#define RS LATD0                   /* PORTD 0 pin is used for Register Select */
#define EN LATD1                   /* PORTD 1 pin is used for Enable */
#define ldata LATB                 /* PORTB is used for transmitting data to LCD */
#define LCD_Port TRISB              
#define LCD_Control TRISD

void LCD_Init();
void LCD_Command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int );

int digital; // holds the digital value 
float voltage; // hold the analog value (volt))
float Lux;
char data[10];
char Lux_1[5] = "Lux: ";
void ADC_Init(void);