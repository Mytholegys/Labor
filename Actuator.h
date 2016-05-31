/*
 * Actuator.h
 *
 *  Created on: 20.03.2015
 *      Author: Gageik
 */

#ifndef ACTUATOR_H_
#define ACTUATOR_H_

#include "rodos.h"
#include <stdio.h>
#include "hal.h"
#include "Control.h"
#include "myTopics.h"
#include "myStructs.h"

#define BLCTRL_ENGINE1_TWI_ADDRESS		0x29	// Engine TWI address
#define BLCTRL_ENGINE2_TWI_ADDRESS		0x2A
#define BLCTRL_ENGINE3_TWI_ADDRESS		0x2B
#define BLCTRL_ENGINE4_TWI_ADDRESS		0x2C


class Actuator : public Thread, SubscriberReceiver<char>{

public:
	virtual ~Actuator();

	Actuator(const char* name, const long priority) :
			Thread(name, priority),
			SubscriberReceiver<char>(i2c_init, "SB2"){	}

	void run();
	void init();

	Stellwerte	Motoren;

	unsigned char stellwert_blctrl_engine1;
	unsigned char stellwert_blctrl_engine2;
	unsigned char stellwert_blctrl_engine3;
	unsigned char stellwert_blctrl_engine4;

	int blctrl_init;

	void put(char &init) {
		blctrl_init = 1;
	    }

	Control 	Ctrl;
	void set_engine(Stellwerte stell);
};

#endif /* ACTUATOR_H_ */
