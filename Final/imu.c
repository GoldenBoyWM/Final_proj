/*
 * imu.c
 *
 *  Created on: Dec 9, 2019
 *      Author: Adam Chehadi
 */

#include "msp.h"
#include "i2c1.h"
#include "imu.h"
#include <stdio.h>
#include <stdlib.h>

#define IMU                 0x69        // 7-bit address

#define PWR_MGMT_1          0x06
#define LP_CONFIG           0x05
#define INT_PIN_CFG         0x0F
#define ACCEL_CONFIG        0x14
#define GYRO_CONFIG_1       0x01

#define VAL_1               0x01
#define VAL_2               0x40
#define VAL_3               0x02
#define VAL_4               0x38
#define VAL_5               0x38


void config_imu(void) {
   int i;
   for (i=0; i<10000;i++); // Start-up time
   printf("Establishing connection with IMU...\n");
   for(i=0; i<100500; i++);

   uint8_t who_am_i = read_register(IMU ,0x00);
   printf("Device ID 0x%x.\n",who_am_i);
   // Determines ID of device from Who Am I register.

    write_register(IMU, PWR_MGMT_1, VAL_1);

    write_register(IMU, LP_CONFIG, VAL_2);

    write_register(IMU, INT_PIN_CFG, VAL_3);

    write_register(IMU, ACCEL_CONFIG, VAL_4);

    while(1) {
        accelerometer_x_axis();
    }
}


uint16_t accelerometer_x_axis(void){
    uint8_t data;
    uint16_t accel_x_axis = 0;
    data = read_register(IMU, 0x2E);
    accel_x_axis = data;
    printf("x = %u\n", accel_x_axis);
    return accel_x_axis;
}
