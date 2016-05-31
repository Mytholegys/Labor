/*
 * myStructs.h
 *
 *  Created on: 31.03.2015
 *      Author: Gageik
 */

#include "matlib.h"
#include "rodos.h"

#ifndef MYSTRUCTS_H_
#define MYSTRUCTS_H_

typedef struct{
	unsigned char M1;
	unsigned char M2;
	unsigned char M3;
	unsigned char M4;
}Stellwerte;


typedef struct{
	//char	dummy;
}Winkel;

typedef struct{						// Rohwerte
	//char	dummy;
	uint8_t sensorBuffer[12];
	Vector3D gyro_raw;
	Vector3D accel_raw;
}IMU_RAW_Data;

typedef struct{
	Vector3D gyro_offsets;
	Vector3D accel_offsets;
}IMU_Offset;

typedef struct{
	char dummy;
	Vector3D gyro;
	Vector3D accel;
}IMU_Data;


typedef struct{
	char	dummy;
} TelecommandData;


typedef struct{
	HAL_I2C * i2c;
	bool i2c_init;
	int	error_counter;
	uint8_t i2c_buffer[12];
	int32_t i2c_return;
}I2C_Data;


#endif /* MYSTRUCTS_H_ */
