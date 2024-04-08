/* This header file contains the functions used in the project 
   file: digi_calc.c */
/*-------------------------------
  Program Constants 
 --------------------------------*/
unsigned char a;
unsigned char c;
unsigned char answer;
unsigned char Display_Results_REG __attribute ((address (0x10)));
unsigned char oper __attribute ((address (0x03)));
unsigned char X_Input_REG __attribute ((address (0x30)));
unsigned char Y_Input_REG __attribute ((address (0x40)));
#define _XTAL_FREQ 4000000        // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
/*--------------------------------
  Program Functions
 ---------------------------------*/  
    void store_val_X (unsigned char vals){
        c = 1;
        X_Input_REG = vals;
        PORTD = 0x01;
    }
    void store_val_Y (unsigned char valy){
        Y_Input_REG = valy;
        PORTD = 0x02;
    }
    void solve (unsigned char oper){
        if (oper == 0){
            answer = X_Input_REG + Y_Input_REG;
    }
        else if (oper == 1){
            Y_Input_REG = ~Y_Input_REG + 1;
            answer = X_Input_REG + Y_Input_REG;
    }
        else if (oper == 2){
            answer = X_Input_REG * Y_Input_REG;
    }
        else if (oper == 3){
            if (X_Input_REG < Y_Input_REG)
                answer = 0;
            else if (X_Input_REG == Y_Input_REG)
                answer = 1;
            else if (X_Input_REG % 2 == 0){ // Checking if X is Even
                if (Y_Input_REG < 2)
                    answer = X_Input_REG;
                else if(Y_Input_REG >= 2 && Y_Input_REG < 4)
                    answer = X_Input_REG >> 1;
                else answer = X_Input_REG >> 2;               
            }
            else {                          // Checking if X is Odd
                if (Y_Input_REG < 2)
                    answer = X_Input_REG;
                else if (Y_Input_REG >= 2 && Y_Input_REG < 3)
                    answer = X_Input_REG >> 1;
                else if ((X_Input_REG == 7 || X_Input_REG == 9) && Y_Input_REG == 3){ 
                    answer = (~X_Input_REG & 0x0F) >> 1 ;
                }
                else answer = X_Input_REG >> 2;          
            }
          }
        else PORTD = 0;
    }
    void rst_result(unsigned char rst){
        X_Input_REG = 0;
        Y_Input_REG = 0;
        Display_Results_REG = 0;
        a = 0;
        c = 0;
        PORTB = 0;
        PORTD = 0;
    }

