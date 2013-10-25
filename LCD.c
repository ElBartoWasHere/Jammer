#include <msp430f6736.h>

//******************************************************************************
//
//
//  MSP430 LCD interface
//
//             |     LCD    |  
//  LCD Pin #  |  Function  |   MSP Pin #
//  ----------------------------
//  PIN 1      |     GND    |     ---
//  PIN 2      |     VCC    |     ---
//  PIN 3      |     V0     |     ---
//  PIN 4      |     RS     |     P5.0
//  PIN 5      |     RW     |     ---
//  PIN 6      |     EN     |     P5.1
//  PIN 7      |     DB0    |     ---
//  PIN 8      |     DB1    |     ---
//  PIN 9      |     DB2    |     ---
//  PIN 10     |     DB3    |     ---
//  PIN 11     |     DB4    |     P5.4  
//  PIN 12     |     DB5    |     P5.5     
//  PIN 13     |     DB6    |     P5.6     
//  PIN 14     |     DB7    |     P5.7     
//  PIN 15     |     LED+   |     ---
//  PIN 16     |     LED-   |     ---
//
//
//
//
//
//
//
//
//
//******************************************************************************

// Define LCD - MSP port directions
#define         LCD_DIR                 P5DIR
#define         LCD_OUT                 P5OUT

// Define LCD - MSP pin mapping 
#define         LCD_PIN_RS              BIT0          // P1.0
#define         LCD_PIN_EN              BIT1          // P1.1
#define         LCD_PIN_D4              BIT4          // P1.4
#define         LCD_PIN_D5              BIT5          // P1.5
#define         LCD_PIN_D6              BIT6          // P1.6
#define         LCD_PIN_D7              BIT7          // P1.7

#define LCD_PIN_MASK ((LCD_PIN_RS | LCD_PIN_EN | LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4))

#define         TRUE                    1
#define         FALSE                   0

  
}
/*
* Description: Function that tells LCD to read its data bus
*/
void enable()
{
    
    // pull Enable bit low
    LCD_OUT &= ~LCD_PIN_EN;
    __delay_cycles(200);

    // pull Enable bit high
    LCD_OUT |= LCD_PIN_EN;
    __delay_cycles(200);

    // pull Enable bit low again
    LCD_OUT &= ~LCD_PIN_EN;
    __delay_cycles(200);
}

/*
* Description: Send a byte on the data bus in the 4 bit mode. This requires 
*              sending data in two chunks, the high chunk first and then the low
*              chunk.
* Parameters
*    - byteToSend: the entire byte to send
*    - isData: set TRUE if byte is character, FALSE if command
*/
void sendByte(char byteToSend, int isData)
{
    // clear out all pins
    LCD_OUT &= ~LCD_PIN_MASK;

    // set High Nibble (HN) - 
    // usefulness of the identity mapping
    // apparent here. We can set the 
    // DB7 - DB4 just by setting P1.7 - P1.4 
    // using a simple assignment
    LCD_OUT |= (byteToSend & 0xF0);
 
    if (isData == TRUE)
    {
        LCD_OUT |= LCD_PIN_RS;
    }
    else
    {
        LCD_OUT &= ~LCD_PIN_RS;
    }

    // we've set up the input voltages to the LCD.
    // Now tell it to read them.
    enable();

    // set Low Nibble (LN) -
    // usefulness of the identity mapping
    // apparent here. We can set the 
    // DB7 - DB4 just by setting P1.7 - P1.4 
    // using a simple assignment
    LCD_OUT &= ~LCD_PIN_MASK;
    LCD_OUT |= ((byteToSend & 0x0F) << 4);

    if (isData == TRUE)
    {
        LCD_OUT |= LCD_PIN_RS;
    }
    else
    {
        LCD_OUT &= ~LCD_PIN_RS;
    }

    // we've set up the input voltages to the LCD.
    // Now tell it to read them.
    enable();
}

/*
* Description: Set position of cursor on screen
* Parameters
*    - row: zero based row #
*    - col: zero based column #
*/
void lcdSetCursorPosition(char row, char col)
{
    char address;

    // construct address from (row, col) pair
    if (row == 0)
    {
        address = 0;
    }
    else
    {
        address = 0x40;
    }

    address |= col;

    sendByte(0x80 | address, FALSE);
}

/*
* Description: Clear LCD display and return cursor home
*/
void clearDisplay()
{
    // Clear display, return home
    sendByte(0x01, FALSE);
    sendByte(0x02, FALSE);
}

/*
* Description: Initialize LCD at power-up.
*
* NOTE: Do not call twice, or nuclear bomb might set off. Very common when
*       LCD power source is different from MCU power source.
*/
void initLCD(void)
{
    // set the MSP pin configurations
    // and bring them to low
    LCD_DIR |= LCD_PIN_MASK;
    LCD_OUT &= ~LCD_PIN_MASK;

    // wait for the LCD to warm up and reach
    // active regions. Remember MSPs can power
    // up much faster than the LCD.
    __delay_cycles(100000);

    // initialize the LCD module
    // 1. Set 4-bit input 
    LCD_OUT &= ~LCD_PIN_RS;
    LCD_OUT &= ~LCD_PIN_EN;

    LCD_OUT = 0x20;
    enable();

    // set 4-bit input - second time.
    // (as reqd by the spec.)
    sendByte(0x28, FALSE);

    // 2. Display on, cursor on, blink cursor
    sendByte(0x0E, FALSE);
    
    // 3. Cursor move auto-increment
    sendByte(0x06, FALSE);
}

/*
* Description: Print character array on LCD display
* Parameters
*    - text: null terminated character array (string)
*/
void mssgDisplay(char *text)
{
    char *c;

    c = text;

    // Send characters one by one until string is empty
    while ((c != 0) && (*c != 0))
    {
        sendByte(*c, TRUE);
        c++;
    }
}

/*
* Description: Main function
*/
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    initLCD();

    clearDisplay();

    mssgDisplay("Hello World!");

    while (1)
    {
        __delay_cycles(1000);
    }

    __bis_SR_register(LPM0_bits);                 // CPU off, enable interrupts
    
}
