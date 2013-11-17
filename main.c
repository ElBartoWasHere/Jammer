#include <msp430f6736.h>
#include "LCD.h"
#include "FuelGauge.h"

//******************************************************************************
//  Description: This module controls three components using an msp430f6736 as
//		 the MCU. It comunicates with components through SPI, I2C, and 
//		 GPIO. 
//
//******************************************************************************
//  MSP430 LCD interface
//
//             |     LCD    |  
//  LCD Pin #  |  Function  |   MSP Pin #
//  ---------------------------------------
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
//******************************************************************************
//
//  MSP430 Digital Potentiometer interface
//
//             |     PD     |  
//  DP Pin #   |  Function  |   MSP Pin #
//  ---------------------------------------
//  PIN 1      |     ~CS    |     P2.6
//  PIN 2      |     SCK    |     P2.5
//  PIN 3      |   SDI/SDO  |     P2.3
//  PIN 4      |     VSS    |     ---
//  PIN 5      |     P0A    |     ---
//  PIN 6      |     W      |     ---
//  PIN 7      |     P0B    |     ---
//  PIN 8      |     VDD    |     ---
//
//
//  VSS -> GND
//  P0A -> Voltage Divisor
//  W   -> Mixer
//  P0B -> GND
//  VDD -> +5V Regulator
//
//******************************************************************************
//
//  MSP430 Battery Fuel Gauge interface
//
//             |     BFG    |  
//  DP Pin #   |  Function  |   MSP Pin #
//  ---------------------------------------
//  PIN 1      |     SDA    |     P2.1
//  PIN 2      |     SCK    |     P2.0
//  PIN 3      |     QST    |     GND
//  PIN 4      |     ALT    |     ---
//  PIN 5      |     GND    |     ---
//  PIN 6      |     VDD    |     ---
//
//
//	QST -> GND
//	GND -> Battery
//	VDD -> Battery
//
//******************************************************************************
//
//  BUTTON LCD -> P1.0
//  SWITCH GSM850 -> P1.1
//  SWITCH GSM900 -> P1.2
//
//
//
//******************************************************************************

// Define Pins
#define         BUTTON1         BIT0
#define         SWITCH1         BIT1
#define         SWITCH2         BIT2
#define         INTERRUPT_MASK  (BUTTON1 | SWITCH1 | SWITCH2)

// Create variables
unsigned char batterySOC;
unsigned char percentage1;
unsigned char percentage2;

// Create functions here
void test()
{
  batterySOC = readSOC();
  percentage1 = (batterySOC & 0xF0);
  percentage2 = (batterySOC & 0x0F);
  
  char mssg[33] = "CF: 850  1900   Charge:   %";
  mssg[25] = percentage1;
  mssg[26] = percentage2;
  
  char *mssgPtr = mssg;
  
  initLCD(mssgPtr);
}

/*
*  Description: 
*
*/
int main (void)
{
  WDTCTL = WDTPW | WDTHOLD;
	
  // Init Interrupts
  P1DIR &= ~INTERRUPT_MASK;
  P1IFG &= ~INTERRUPT_MASK;
  P1IE |= INTERRUPT_MASK;
  P1IES &= ~INTERRUPT_MASK;
  
  // Init I2C
  initI2C();
  
  __delay_cycles(100);
  
  // Init SPI
  
  test();
  
  __bis_SR_register(LPM0 | GIE);
}

// Interrupt Vectors here
// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
  batterySOC = readSOC();
  percentage1 = (batterySOC & 0xF0);
  percentage2 = (batterySOC & 0x0F);
  
  char mssg[33] = "CF: 850  1900   Charge:   %";
  mssg[25] = percentage1;
  mssg[26] = percentage2;
  
  char *mssgPtr = mssg;
  
  initLCD(mssgPtr);
  
  P1IFG &= ~INTERRUPT_MASK;
}
