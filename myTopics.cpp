/*
 * myTopics.cpp
 *
 *  Created on: 23.03.2015
 *      Author: Gageik
 */

#include "rodos.h"
#include "myStructs.h"

Topic<char>			i2c_init(-1, "i2c_error");

CommBuffer<Winkel> 	rpy_winkel;
CommBuffer<IMU_Data> imu;

char filter_count = 0;							// Zählt Anzahl Filter
TelecommandData tmData;
bool engine_on = 0;




