/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */

/* TODO: Make define statements for each pin used in the LCD
 */
#include <xc.h>
#include "lcd.h"
#include "timer.h"

#define LCD_DATA LATE
//#define LCD_RS  
//#define LCD_E   

#define DB7 LATEbits.LATE6
#define DB6 LATEbits.LATE4
#define DB5 LATEbits.LATE2
#define DB4 LATEbits.LATE0
#define RS LATBbits.LATB11
#define E  LATBbits.LATB13

#define INPUT 1
#define OUTPUT 0

#define LCD_WRITE 1
#define LCD_CONTROL 0

#define LOWER 1
#define UPPER 0

/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATB. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int bound)
{
    //TODO:
    if(bound==0)
    {
        DB7 = (word & 0x80)>>7;
        DB6 = (word & 0x40)>>6;
        DB5 = (word & 0x20)>>5;
        DB4 = (word & 0x10)>>4;
    }
    else
    {
        DB7 = (word & 0x08)>>3;
        DB6 = (word & 0x04)>>2;
        DB5 = (word & 0x02)>>1;
        DB4 = (word & 0x01);
    }
    RS = commandType;
    delayUs(1);
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1);
    delayUs(delayAfter);
    RS = LCD_CONTROL;           //Turn off while not writing character
    delayUs(1);
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter)
{
    //TODO:
    writeFourBits(word, commandType, 46, UPPER);
    writeFourBits(word, commandType, 46, LOWER);
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) 
{
    //TODO:
    writeLCD(c, LCD_WRITE, 46);
}
/*Initialize the LCD
 */
void initLCD(void) {
    // Setup D, RS, and E to be outputs (0). Change TRIS_ to pins that will be in use
    TRISEbits.TRISE6 = OUTPUT;
    TRISEbits.TRISE4 = OUTPUT;
    TRISEbits.TRISE2 = OUTPUT;
    TRISEbits.TRISE0 = OUTPUT;
    TRISBbits.TRISB11 = OUTPUT;
    TRISBbits.TRISB13 = OUTPUT;
    
    // Initilization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initilition commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.
    delayUs(15000);
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 1;DB4 = 1;
    E = 1;
    delayUs(1);
    E = 0;
    
    delayUs(4100);
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 1;DB4 = 1;
    E = 1;
    delayUs(1);
    E = 0;

    delayUs(110);
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 1;DB4 = 1;
    E = 1;
    delayUs(1);
    E = 0;  
    delayUs(1);
    

    // Enable 4-bit interface
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 1;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;  
    delayUs(40);
    // Function Set (specifies data width, lines, and font.
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 1;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1);
    RS = 0;DB7 = 1; DB6 = 0;DB5 = 0;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;  
    delayUs(40);
    // 4-bit mode initialization is complete. We can now configure the various LCD
    // options to control how the LCD will function.

    // TODO: Display On/Off Control
        // Turn Display (D) Off
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 0;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1);
    RS = 0;DB7 = 1; DB6 = 0;DB5 = 0;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;  
    delayUs(40);
    // TODO: Clear Display (The delay is not specified in the data sheet at this point. You really need to have the clear display delay here.
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 0;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1);
    RS = 0;DB7 = 0; DB6 = 0;DB5 = 0;DB4 = 1;
    E = 1;
    delayUs(1);
    E = 0;  
    delayUs(1000);
    // TODO: Entry Mode Set
        // Set Increment Display, No Shift (i.e. cursor move)
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 0;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1);
    RS = 0;DB7 = 0; DB6 = 1;DB5 = 1;DB4 = 1;
    E = 1;
    delayUs(1);
    E = 0;  
    delayUs(40);
    // TODO: Display On/Off Control
        // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
    
}   

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) 
{
    //TODO:
    int x = 0;
    while(s[x]!=NULL)
    {
        printCharLCD(s[x]);
    }
}

/*
 * Clear the display.
 */
void clearLCD()
{
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 0;DB4 = 0;
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1);
    RS = 0;DB7 = 0;DB6 = 0;DB5 = 0;DB4 = 1;
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1000);
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y)
{
    DB7 = 1;                            //set DD RAM ADDRESS
    if(x==0)                            //Handles most sig bits
    {
        DB6 = 0; DB5 = 0; DB4 = 0;
    }
    else if(x==1)
    {
        DB6 = 1; DB5 = 0; DB4 = 0;
    }
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(1);
    
    DB7 = 0;                            //least sig figs
    if(y==0 | y==2 | y==4 | y==6)
        DB4 = 0;  
    else
        DB4 = 1;
    
    if(y==0 | y==1 | y==4 | y==5)
        DB5 = 0;
    else
        DB5 = 1;
    if(y==0 | y==1 | y==2 | y==3)
        DB6 = 0;
    else
        DB6 = 1;
    E = 1;
    delayUs(1);
    E = 0;
    delayUs(40);    
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}
