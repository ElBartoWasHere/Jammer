#include <msp430f6736.h>
#include "FuelGauge.h"

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

unsigned char RxBuffer;                               // Allocate 1 byte for data

/*
*  Description: Initialize all ports for I2C interface.
*/
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

/*
*  Description: This function will write the read SOC command, then it will
*               start reading incoming byte that represents the State Of Charge
*               in a percentage.
*/
unsigned char readSOC()
{
  UCB0CTLW0 |= UCTR + UCTXSTT;                        // Start TX transmission
  
  while (!(UCB0IFG & UCTXIFG0) | (UCB0CTLW0 & UCTXSTT));  // Wait for previous TX to complete
  UCB0TXBUF = 0x04;                                   // Memory Address desired to reading
  
  while (!(UCB0IFG & UCTXIFG0) | (UCB0CTLW0 & UCTXSTT));  // Wait for previous TX to complete
  UCB0CTLW0 &= ~UCTR;                                 // Set Master Read bit
  UCB0CTLW0 |= UCTXSTT;                               // Send Repeated Start bit
  
  while (!(UCB0IFG & UCRXIFG0));                      // Wait for an RX character to be received
  
  UCB0CTLW0 |= UCTXSTP;                               // Send stop bit before first data byte
  RxBuffer = UCB0RXBUF;                               // Save received byte
  
  while (UCB0CTLW0 & UCTXSTP);                        // Wait for full stop
  
  return RxBuffer;                                    // Return received SOC value
}
