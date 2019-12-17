/*
 * imu.h
 *
 *  Created on: Dec 9, 2019
 *      Author: Adam Chehadi
 */

#ifndef IMU_H_
#define IMU_H_

uint8_t data;

void config_imu(void);

uint16_t accelerometer_x_axis(void);

#endif /* IMU_H_ */
