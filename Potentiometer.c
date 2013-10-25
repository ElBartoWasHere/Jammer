#include <msp430f6736.h>

unsigned char MST_Data;

int main(void)
{

    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    // Setup P1.0 output, P1.2 UCA0SOMI, P1.3 UCA0SIMO, P1.6 UCA0CLK
    P1OUT &= ~BIT7;                               // Clear P1.0
    P1DIR |= BIT7;                                // Set P1.0 to output direction
    P1SEL |= BIT3 | BIT6;                         // Set P1.0, P1.3,P1.6 to non-IO

     // Setup additional button
    //P1IN &= ~BIT5;                                // Clear P1.5
    P2DIR &= ~BIT0;                               // Set P1.5 as input
    
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
