#include <msp430f6736.h>

//******************************************************************************
//
//
//  MSP430 LCD interface
//
//             |     LCD    |  
//  LCD Pin #  |  Function  |   MSP Pin #
//  ----------------------------
//  PIN 1      |     GND    |     DVSS
//  PIN 2      |     VCC    |     P4.5
//  PIN 3      |     V0     |     Outer potentiometer
//  PIN 4      |     RS     |     P4.6
//  PIN 5      |     RW     |     DVSS
//  PIN 6      |     EN     |     P4.7
//  PIN 7      |     DB0    |     P5.0
//  PIN 8      |     DB1    |     P5.1
//  PIN 9      |     DB2    |     P5.2
//  PIN 10     |     DB3    |     P5.3
//  PIN 11     |     DB4    |     P5.4  
//  PIN 12     |     DB5    |     P5.5     
//  PIN 13     |     DB6    |     P5.6     
//  PIN 14     |     DB7    |     P5.7     
//  PIN 15     |     LED+   |     P4.5
//  PIN 16     |     LED-   |     DVSS
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
#define         LCD_COM_DIR             P4DIR
#define         LCD_COM_OUT             P4OUT
#define         LCD_DIR                 P5DIR
#define         LCD_OUT                 P5OUT

// Define LCD - MSP pin mapping 
#define         LCD_VCC                 BIT5          // P4.5
#define         LCD_PIN_RS              BIT6          // P4.6
#define         LCD_PIN_EN              BIT7          // P4.7
#define         LCD_PIN_D0              BIT0          // P5.0
#define         LCD_PIN_D1              BIT1          // P5.1
#define         LCD_PIN_D2              BIT2          // P5.2
#define         LCD_PIN_D3              BIT3          // P5.3
#define         LCD_PIN_D4              BIT4          // P5.4
#define         LCD_PIN_D5              BIT5          // P5.5
#define         LCD_PIN_D6              BIT6          // P5.6
#define         LCD_PIN_D7              BIT7          // P5.7

#define LCD_COM_MASK ((LCD_PIN_EN | LCD_PIN_RS))
#define LCD_PIN_MASK ((LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4 | LCD_PIN_D3 | LCD_PIN_D2 | LCD_PIN_D1 | LCD_PIN_D0))

#define         TRUE                    1
#define         FALSE                   0

/*
* Description: Function that tells LCD to read its data bus
*/
void enable()
{
    // pull Enable bit high
    LCD_COM_OUT |= LCD_PIN_EN;
    __delay_cycles(2000);
    
    // pull Enable bit low again
    LCD_COM_OUT &= ~LCD_PIN_EN;

}

/*
* Description: Send a byte on the data bus.
* Parameters
*    - byteToSend: the entire byte to send
*    - isData: set TRUE if byte is character, FALSE if command
*/
void sendByte(char byteToSend, int isData)
{
    // clear out all pins
    LCD_COM_OUT &= ~LCD_COM_MASK;
    LCD_OUT &= ~LCD_PIN_MASK;

    // set data for output
    LCD_OUT |= byteToSend;
    
    // determine if instruction is command or character
    if (isData == TRUE)
    {
        LCD_COM_OUT |= LCD_PIN_RS;
    }
    else
    {
        LCD_COM_OUT &= ~LCD_PIN_RS;
    }

    // we've set up the input voltages to the LCD.
    // Now tell it to read them.
    enable();
    
}

/*
* Description: Turn LCD display on or off
* Parameters
*    - isON: 1 if desire to turn on display, 0 otherwise
*/
void displayON(int isON)
{
  if(isON == TRUE)
    sendByte(0x0F, FALSE);
  else
    sendByte(0x08, FALSE);
}

/*
* Description: Clear LCD display and return cursor home
*/
void clearDisplay()
{
    // Clear display, return home
    sendByte(0x01, FALSE);
}

/*
* Description: Set the position of the cursor on the screen
* Parameters
*    - row: zero based row number
*    - col: zero based column number
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
* Description: Initialize LCD at power-up.
*
* NOTE: Do not call twice, or nuclear bomb might set off. Very common when
*       LCD power source is different from MCU power source.
*/
void initLCD(void)
{
    LCD_COM_DIR |= LCD_VCC;
    LCD_COM_OUT |= LCD_VCC;
    
    __delay_cycles(45000);
  
    // set the MSP pin configurations
    // and bring them to low
    LCD_DIR |= LCD_PIN_MASK;
    LCD_OUT &= ~LCD_PIN_MASK;
    LCD_COM_DIR |= LCD_COM_MASK;
    LCD_COM_OUT &= ~LCD_COM_MASK;

    // wait for the LCD to warm up and reach
    // active regions. Remember MSPs can power
    // up much faster than the LCD.
    __delay_cycles(10000);

    // initialize the LCD module
    // set 8-bit 2-line
    sendByte(0x38, FALSE);
    
    // Display on, cursor on, blink cursor
    displayON(TRUE);
    
    // clear display
    clearDisplay();
    
//    sendByte(0x65, TRUE);               // e
//    sendByte(0x66, TRUE);               // f
//    sendByte(0x67, TRUE);               // g
//    sendByte(0x68, TRUE);               // h
    
}

/*
* Description: Print character array on LCD display
* Parameters
*    - text: null terminated character array (string)
*/
void mssgDisplay(char *text)
{
    char *c;
    int count = 16;
    c = text;

    // Send characters one by one until string is empty
    while ((c != 0) && (*c != 0))
    {
        // after 16 characters start writing in second line
        if(count == 0)
          lcdSetCursorPosition(1,0);
      
        sendByte(*c, TRUE);
        c++;
        count--;
    }
}

/*
* Description: Main function
*/
int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    initLCD();

    mssgDisplay("Center Freq: ###Charge: %%%");
    
    __delay_cycles(3000000);
    
    LCD_COM_OUT &= ~LCD_VCC;                    // Turn Off LCD
/*
    while (1)
    {
        __delay_cycles(1000);
    }
*/
    __bis_SR_register(LPM0_bits);               // CPU Low-Power Mode 0
    
}
