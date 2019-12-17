/*
 * rtc.h
 *
 *  Created on: Nov 27, 2019
 *      Author: Adam Chehadi
 */

#ifndef RTC_H_
#define RTC_H_

int hourt;
int houro;
int mint;
int mino;
int toggle;
int togset;
int start;
int yeart;
int yearo;
int year_tog;
int montht;
int montho;
int month_tog;
int dayt;
int dayo;
int day_tog;


#include "driverlib.h"

void config_rtc(void);
void init_toggle(void);

void RTC_C_IRQHandler(void);
void counter(void);
void display(void);


#endif /* RTC_H_ */
