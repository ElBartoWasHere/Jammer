#include <msp430f6736.h>

//******************************************************************************
//
//
//  MSP430 digital potentiometer interface
//
//             |     PD     |  
//  DP Pin #   |  Function  |   MSP Pin #
//  ---------------------------------------
//  PIN 1      |     ~CS    |     P1.7
//  PIN 2      |     SCK    |     P1.6
//  PIN 3      |   SDI/SDO  |     P1.3
//  PIN 4      |     VSS    |     ---
//  PIN 5      |     P0A    |     ---
//  PIN 6      |     W      |     ---
//  PIN 7      |     P0B    |     ---
//  PIN 8      |     VDD    |     ---
//
//
//  VSS -> GND
//  P0A -> voltage divisor
//  W   -> Mixer
//  P0B -> GND
//  VDD -> +5V Regulator
//
//
//******************************************************************************


unsigned char MST_Data;

int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    // Setup P1.7 output, P1.3 UCA0SIMO, P1.6 UCA0CLK
    P1OUT &= ~BIT7;                               // Clear P1.7
    P1DIR |= BIT7;                                // Set P1.7 to output direction
    P1SEL |= BIT3 | BIT6;                         // Set P1.3,P1.6 to non-IO

    // Setup INTERRUPT here
    
    
    // Setup eUSCI_A0
    UCA0CTLW0 |= UCSWRST;                         // **Put state machine in reset**
    UCA0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB; // 3-pin, 8-bit SPI master
                                                  // Clock polarity high, MSB
    UCA0CTLW0 |= UCSSEL_2;                        // SMCLK
    UCA0BRW_L = 0x02;                             // /2
    UCA0BRW_H = 0;                                //
    UCA0MCTLW = 0;                                // No modulation
    UCA0CTLW0 &= ~UCSWRST;                        // **Initialize USCI state machine**
    UCA0IE = UCRXIE;                              // Enable USCI_A0 RX,TX interrupt

    __delay_cycles(100);                          // Wait for slave to initialize

    MST_Data = 0x08;                              // Initialize data values

    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment

    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment

    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment

    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment

    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment

    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment
    while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
    UCA0TXBUF = MST_Data;                     // Transmit increment    
    
/*
    while (1)
    {
      if(P2IN == BIT0)
      {
        while (!(UCA0IFG & UCTXIFG)) ;            // USCI_A0 TX buffer ready?
        UCA0TXBUF = MST_Data;                     // Transmit increment
      }
    };       
*/
    
    
    __bis_SR_register(LPM0_bits);                 // CPU off, enable interrupts
    
}
