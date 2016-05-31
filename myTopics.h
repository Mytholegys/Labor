/*
 * topic.h
 *
 *  Created on: 23.03.2015
 *      Author: Gageik
 */
#include "myStructs.h"

#ifndef TOPIC_H_
#define TOPIC_H_

extern Topic<char>				i2c_init;

extern CommBuffer<Winkel>		rpy_winkel;			// Wird von Attitude Control verwendet
extern CommBuffer<IMU_Data> 	imu;

extern char 				filter_count;
extern TelecommandData 		tmData;
extern bool engine_on;



#endif /* TOPIC_H_ */
