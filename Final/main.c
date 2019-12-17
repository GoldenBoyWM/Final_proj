#include "msp432.h"
#include "msp.h"
#include "driverlib.h"
#include "rtc.h"
#include "imu.h"
#include "i2c1.h"
#include <stdio.h>
#include <stdlib.h>

const eUSCI_UART_Config uartConfig =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
    78,                                     // BRDIV = 78
    2,                                       // UCxBRF = 2
    0,                                       // UCxBRS = 0
    EUSCI_A_UART_NO_PARITY,                  // No Parity
    EUSCI_A_UART_LSB_FIRST,                  // LSB First
    EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
    EUSCI_A_UART_MODE,                       // UART mode
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  // Oversampling
};


int main(void)
{
    WDT_A_holdTimer();
    config_i2c();
    config_imu();
    // Config P3.3 to UART
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
    //Set DCO (clock) to 12MHz
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);
    // Config UART
    UART_initModule(EUSCI_A2_BASE, &uartConfig);
    // Enable UART
    UART_enableModule(EUSCI_A2_BASE);
    //Config LCD
    UART_transmitData(EUSCI_A2_BASE, '|'); //settings
    UART_transmitData(EUSCI_A2_BASE, 157); //backlight
    UART_transmitData(EUSCI_A2_BASE, 0x18); //contrast (doesn't work?)
    UART_transmitData(EUSCI_A2_BASE, 0); //contrast
    UART_transmitData(EUSCI_A2_BASE, '-'); //clear
    UART_transmitData(EUSCI_A2_BASE, '|');
    UART_transmitData(EUSCI_A2_BASE, '-');
    //config rtc
    init_toggle();
    config_rtc();
}
