/*
 * i2c.c
 *
 *  Created on: Dec 8, 2019
 *      Author: Adam Chehadi
 */

#include "msp.h"
#include "i2c1.h"
#include <stdio.h>
#include <stdlib.h>


void config_i2c(void) {
    // Initialize eUSCI_B regs with setting the UCSWRST bit
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_SWRST;   /*!< Software reset enable */

    // Configure ports
    P1->SEL0 |= (BIT6 | BIT7);
    P1->SEL1 &= ~(BIT6 | BIT7);

    EUSCI_B0 -> CTLW0 |= (EUSCI_B_CTLW0_MODE_3 |  /*!< I2C mode */
                        EUSCI_B_CTLW0_MST    |  /*!< Master mode select */
                        EUSCI_B_CTLW0_UCSSEL_3);    /*!< SMCLK */

    EUSCI_B0 -> BRW = 8;   // Setting clock prescalar for 375 kHz

    // Clear UCSWRST in software
    EUSCI_B0 -> CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);

    // Enable interrupts
    EUSCI_B0 -> IFG = 0;    /*!< eUSCI_Bx Interrupt Flag reg */
}




uint8_t read_register(uint8_t address, uint8_t reg) {
    // MASTER TRANSMITTER MODE
    // Slave transmitter mode is entered when the slave address transmitted by the master is identical to its own
    // Master needs to act as transmitter to send the address so it can enter Slave Transmitter Mode / Master Receive Mode
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TR;

    // Write desired slave address to the UCBxI2CSA reg
    EUSCI_B0 -> I2CSA = address;

    // UCBxTBCNT must be set to the number of bytes that are to be transmitted or received
    EUSCI_B0 -> TBCNT = 1;

    // Set UCTXSTT to generate a START condition
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    // Wait for START condition
    while((EUSCI_B0 -> CTLW0) & EUSCI_B_CTLW0_TXSTT);

    // Transfer reg from Transfer Buffer
    EUSCI_B0 -> TXBUF = reg;

    // Wait for buffer to transfer reg
    while(!((EUSCI_B0 -> IFG) & EUSCI_B_IFG_TXIFG0));


    // MASTER RECEIVE MODE
    // Clear the UCTR for receiver mode
    EUSCI_B0 -> CTLW0 &= ~(EUSCI_B_CTLW0_TR);

    // Write desired slave address to the UCBxI2CSA reg
    EUSCI_B0 -> I2CSA = address;

    // UCBxTBCNT must be set to the number of bytes that are to be transmitted or received
    EUSCI_B0 -> TBCNT = 1;

    // Set UCTXSTT to generate a START condition
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    // Wait for START condition
    while((EUSCI_B0 -> CTLW0) & EUSCI_B_CTLW0_TXSTT);

    // Wait for buffer to receive data
    while(!((EUSCI_B0 -> IFG) & EUSCI_B_IFG_RXIFG));

    // Read UCBxRXBUF
    uint8_t rx_reg_buffer_data = 0xFF;
    rx_reg_buffer_data = EUSCI_B0 -> RXBUF;

    // Generate STOP condition
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TXSTP;

    // Wait for the STOP condition
    while((EUSCI_B0 -> CTLW0) & EUSCI_B_CTLW0_TXSTT);

    // Return Slave Receive Data
    return rx_reg_buffer_data;
}




void write_register(uint8_t address, uint8_t reg, uint8_t value) {
    // MASTER TRANSMITTER MODE
    // Slave transmitter mode is entered when the slave address transmitted by the master is identical to its own
    // Master needs to act as transmitter to send the address so it can enter Slave Transmitter Mode / Master Receive Mode
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TR;

    // Write desired slave address to the UCBxI2CSA reg
    EUSCI_B0 -> I2CSA = address;

    // UCBxTBCNT must be set to the number of bytes that are to be transmitted or received
    EUSCI_B0 -> TBCNT = 2;

    // Set UCTXSTT to generate a START condition
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    // Wait for START condition
    while((EUSCI_B0 -> CTLW0) & EUSCI_B_CTLW0_TXSTT);

    // Transfer reg from Transfer Buffer
    EUSCI_B0->TXBUF = reg;

    // Wait for buffer to transfer reg
    while(!((EUSCI_B0 -> IFG) & EUSCI_B_IFG_TXIFG0));

    // Transfer value from Transfer Buffer
    EUSCI_B0->TXBUF = value;

    // Wait for buffer to transfer value
    while(!((EUSCI_B0 -> IFG) & EUSCI_B_IFG_TXIFG0));

    // Generate STOP condition
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_TXSTP;

    // Wait for the STOP condition
    while((EUSCI_B0 -> CTLW0) & EUSCI_B_CTLW0_TXSTT);
}
