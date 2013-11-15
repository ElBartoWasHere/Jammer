#include <msp430f6736.h>

//******************************************************************************
//
//                                /|\  /|\
//                Fuel Gauge      10k  10k     MSP430F673x
//                   slave         |    |         master
//             -----------------   |    |   -----------------
//            |              SDA|<-|----+->|P2.1/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//            |                 |  |       |             XOUT|-
//            |              SCL|<-+------>|P2.0/UCB0SCL     |
//            |                 |          |                 |
//
//
//
//
//******************************************************************************

unsigned char RxBuffer;                               // Allocate 2 byte of RAM
unsigned char TxCounter;

void initI2C()
{
  // Setup eUSCI_B0
  UCB0CTLW0 |= UCSWRST;                               // Enable SW reset
  UCB0CTLW0 |= UCMST | UCMODE_3 | UCSSEL_2;           // I2C Master, use SMCLK

  UCB0BRW_L = 12;                                     // fSCL = SMCLK/12 = ~100kHz
  UCB0BRW_H = 0;
  UCB0I2CSA = 0x36;                                   // Slave Address is 036h
  UCB0CTLW0 &= ~UCSWRST;                              // Clear SW reset, resume operation
  UCB0IE |= UCTXIE0 | UCRXIE0 | UCNACKIE;             // Enable TX interrupt
    
}

void Transmit()
{
  while (UCB0CTLW0 & UCTXSTP) ;                       // Ensure stop condition got sent
  UCB0CTLW0 |= UCTR | UCTXSTT;                        // I2C TX, start condition
}


void Receive()
{
  while (UCB0CTLW0 & UCTXSTP) ;
  UCB0CTLW0 &= ~UCTR ;                                // Clear UCTR
//  UCB0CTLW0 |= UCTXSTT;                               // I2C start condition
//  while (UCB0CTLW0 & UCTXSTT);                        // Start condition sent?
//  UCB0CTLW0 |= UCTXSTP;                               // I2C stop condition
  __bis_SR_register(LPM0_bits | GIE);                 // Enter LPM0 w/ interrupts
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                         // Stop WDT

    // Setup P2.0 UCB0SCL, P2.1 UCB0SDA
    P2SEL |= BIT0 | BIT1;                             // Set P2.0,P2.1 to UCB0SCL, UCB0SDA

    initI2C();

    while (1)
    {
        TxCounter = 1;
        Transmit();
        
        __bis_SR_register(LPM0_bits | GIE);                 // Enter LPM0 w/ interrupts
        __no_operation();
    }
}

//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
// any 2+ number of bytes by pre-loading RXByteCtr with the byte count.
//-------------------------------------------------------------------------------
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
    switch (__even_in_range(UCB0IV, 30))
    {
        case USCI_NONE: break;                        // No interrupts
        case USCI_I2C_UCALIFG: break;                 // ALIFG
        case USCI_I2C_UCNACKIFG: break;               // NACKIFG
        case USCI_I2C_UCSTTIFG: break;                // STTIFG
        case USCI_I2C_UCSTPIFG: break;                // STPIFG
        case USCI_I2C_UCRXIFG3: break;                // RXIFG3
        case USCI_I2C_UCTXIFG3: break;                // TXIFG3
        case USCI_I2C_UCRXIFG2: break;                // RXIFG2
        case USCI_I2C_UCTXIFG2: break;                // TXIFG2
        case USCI_I2C_UCRXIFG1: break;                // RXIFG1
        case USCI_I2C_UCTXIFG1: break;                // TXIFG1
        
        case USCI_I2C_UCRXIFG0:                       // RXIFG0
            RxBuffer  = UCB0RXBUF;
            UCB0CTLW0 |= UCTXSTP;
            break;                
        
        case USCI_I2C_UCTXIFG0:                       // TXIFG0
            if (TxCounter)
            {
              UCB0TXBUF = 0x04;
              TxCounter--;
            }
            else
            {
              UCB0IFG &= ~UCTXIFG;                      // Clear USCI_B0 TX int flag
              UCB0CTLW0 |= UCTXSTP;
            }
                        

//            Receive();
            break;

        case USCI_I2C_UCBCNTIFG: break;               // CNTIFG
        case USCI_I2C_UCCLTOIFG: break;               // LTOIFG
        case USCI_I2C_UCBIT9IFG: break;               // BIT9IFG
        default: break;
    }
}
