/*
 * rtc.c
 *
 *  Created on: Nov 27, 2019
 *      Author: Adam Chehadi
 */

#include "msp.h"
#include "rtc.h"
#include "i2c1.h"
#include "imu.h"
#include <stdio.h>
#include <stdlib.h>


void config_rtc(void) {
    P1->DIR |= BIT0;
    P1->OUT |= BIT0;

    RTC_C -> CTL0 = RTC_C_KEY |             /*!< RTC_C Key Value for RTC_C write access */
                   RTC_C_CTL0_TEVIE |       /*!< Real-time clock time event interrupt enable */
                   RTC_C_CTL0_TEVIFG;       /*!< Real-time clock time event interrupt flag */

    RTC_C -> CTL13 = RTC_C_CTL13_HOLD |     /*!< Real-time clock hold */
                    RTC_C_CTL13_MODE |
                    RTC_C_CTL13_BCD |       /*!< Real-time clock BCD select */
                    RTC_C_CTL13_TEV_0;      /*!< Minute changed */

    RTC_C -> YEAR = 0x2019;                          // Year = 0x2019
    RTC_C -> DATE = (0x12 << RTC_C_DATE_MON_OFS) |    // Month = 0x12 = December
           (0x04 | RTC_C_DATE_DAY_OFS);             // Day = 0x02 = 2nd
    RTC_C -> TIM1 = (0x03 << RTC_C_TIM1_DOW_OFS) |   // Day of week = 0x01 = Monday
           (0x12 << RTC_C_TIM1_HOUR_OFS);           // Hour = 0x11
    RTC_C -> TIM0 = (0x26 << RTC_C_TIM0_MIN_OFS) |   // Minute = 0x33
           (0x45 << RTC_C_TIM0_SEC_OFS);            // Seconds = 0x45

    // Start RTC calendar mode
    RTC_C -> CTL13 = RTC_C -> CTL13 & ~(RTC_C_CTL13_HOLD);

    // Lock the RTC registers
    RTC_C -> CTL0 = RTC_C -> CTL0 & ~(RTC_C_CTL0_KEY_MASK);

    // Enable global interrupt
    __enable_irq();

    NVIC->ISER[0] = 1 << ((RTC_C_IRQn) & 31);

        // Sleep on exit from ISR
        SCB -> SCR |= SCB_SCR_SLEEPONEXIT_Msk;

        __DSB();

        while (1)
        {
            // Setting the sleep deep bit
            SCB -> SCR |= (SCB_SCR_SLEEPDEEP_Msk);

            // Ensures SLEEPDEEP takes effect immediately
            __DSB();

            __sleep();

            // Clearing the sleep deep bit
            SCB -> SCR &= ~(SCB_SCR_SLEEPDEEP_Msk);
        }
}

void init_toggle(void){
    hourt=48;
    houro=52;
    mint=53;
    mino=53;
    togset=1;
    start=2;
    yeart=49;
    yearo=57;
    year_tog=0;
    montht=49;
    montho=50;
    month_tog=0;
    dayt=48;
    dayo=57;
    day_tog=0;
}

void counter(void){
    if(start==0){
            if(mino<57){
                mino++;
            }
            else if(mino==57 && mint<53){
                mino=48;
                mint++;
            }
            else if(mino==57 && mint==53 && houro<57 && hourt==48){
                mino=48;
                mint=48;
                houro++;
            }
            else if(mino==57 && mint==53 && houro==57 && hourt==48){
                mino=48;
                mint=48;
                houro=48;
                hourt=49;
            }
            else if(mino==57 && mint==53 && houro<50 && hourt==49){
                mino=48;
                mint=48;
                houro++;
            }
            else if(mino==57 && mint==53 && houro==50 && hourt==49){
                mino=48;
                mint=48;
                houro=49;
                hourt=48;
                day_tog++;
            }
            if(day_tog==2 && dayo<57){
                dayo++;
                day_tog=0;
            }
            else if(day_tog==2 && dayo==57 && dayt<50){
                dayo=49;
                dayt++;
                day_tog=0;
            }
            else if(day_tog==2 && dayo==57 && dayt==50){
                dayo=49;
                dayt=48;
                month_tog=1;
                day_tog=0;
            }
            if(month_tog==1 && montht==48 && montho<57){
                montho++;
                month_tog=0;
            }
            else if(month_tog==1 && montht==48 && montho==57){
                montho=48;
                montht=49;
                month_tog=0;
            }
            else if(month_tog==1 && montht==49 && montho<50){
                montho++;
                month_tog=0;
            }
            else if(month_tog==1 && montht==49 && montho==50){
                year_tog=1;
                month_tog=0;
            }
            if(year_tog==1 && yearo<57){
                yearo++;
                year_tog=0;
            }
            if(year_tog==1 && yearo==57 && yeart<57){
                yearo=48;
                year_tog=0;
            }
            if(year_tog==1 && yearo==57 && yeart==57){
                yearo=48;
                yearo=48;
                year_tog=0;
            }
    }
    if(start>0){
        UART_transmitData(EUSCI_A2_BASE, '|');
        UART_transmitData(EUSCI_A2_BASE, '-');
            start--;
        }
display();
}

void display(void){
            UART_transmitData(EUSCI_A2_BASE, '|');
            UART_transmitData(EUSCI_A2_BASE, '-');
            /*UART_transmitData(EUSCI_A2_BASE, 't');
            UART_transmitData(EUSCI_A2_BASE, 'i');
            UART_transmitData(EUSCI_A2_BASE, 'm');
            UART_transmitData(EUSCI_A2_BASE, 'e');
            UART_transmitData(EUSCI_A2_BASE, ':');
            UART_transmitData(EUSCI_A2_BASE, ' ');*/
            UART_transmitAddress(EUSCI_A2_BASE, hourt);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, houro);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, mint);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, mino);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            /*UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, 'd');
            UART_transmitData(EUSCI_A2_BASE, 'a');
            UART_transmitData(EUSCI_A2_BASE, 't');
            UART_transmitData(EUSCI_A2_BASE, 'e');
            UART_transmitData(EUSCI_A2_BASE, ':');
            UART_transmitData(EUSCI_A2_BASE, ' ');*/
            UART_transmitAddress(EUSCI_A2_BASE, montht);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, montho);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, dayt);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, dayo);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, yeart);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, yearo);
            UART_transmitAddress(EUSCI_A2_BASE, yeart);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, yearo);
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitData(EUSCI_A2_BASE, 's');
            UART_transmitData(EUSCI_A2_BASE, 't');
            UART_transmitData(EUSCI_A2_BASE, 'e');
            UART_transmitData(EUSCI_A2_BASE, 'p');
            UART_transmitData(EUSCI_A2_BASE, 's');
            UART_transmitData(EUSCI_A2_BASE, ':');
            UART_transmitData(EUSCI_A2_BASE, ' ');
            UART_transmitAddress(EUSCI_A2_BASE, data);
            UART_transmitData(EUSCI_A2_BASE, ' ');
}


void RTC_C_IRQHandler(void)
{
    if (RTC_C -> CTL0 & RTC_C_CTL0_TEVIFG)
    {
        P1 -> OUT ^= BIT0;
        // Unlock the RTC module and clear time event interrupt flag
        RTC_C -> CTL0 = (RTC_C->CTL0 & ~(RTC_C_CTL0_KEY_MASK |  RTC_C_CTL0_TEVIFG)) | RTC_C_KEY;

        // Re-lock the RTC
        RTC_C -> CTL0 = RTC_C->CTL0 & ~(RTC_C_CTL0_KEY_MASK);
        counter();
    }
}
