/*
 * Actuator.cpp
 *
 *  Created on: 20.03.2015
 *      Author: Gageik
 */

#include "Actuator.h"
#include "myTopics.h"

void reset_i2c();
extern I2C_Data i2c_1;
extern TelecommandData tmData;

//Actuator	 	motoren("BluusMotoren", 50);			// Actuatoren

Actuator::~Actuator() {}

void Actuator::run(){

	this->setPeriodicBeat(2*SECONDS, 10*MILLISECONDS);

	Motoren.M1 = 20;
	Motoren.M2 = 20;
	Motoren.M3 = 20;
	Motoren.M4 = 20;

	PRINTF("\n\n*** Motors up! ***\n");

	while (1){
			if (engine_on){

				//Motoren = Ctrl.run();

				set_engine(Motoren);
			}
			else {
				init();
			}

			suspendUntilNextBeat();
	}
}


void Actuator::init(){
	blctrl_init = 1;
}



void Actuator::set_engine(Stellwerte stell){

		if (!i2c_1.i2c_init)
			return;

		int blctrl_i;

		if (blctrl_init){

			blctrl_init = 0;

			stellwert_blctrl_engine1 = 0;
			stellwert_blctrl_engine2 = 0;
			stellwert_blctrl_engine3 = 0;
			stellwert_blctrl_engine4 = 0;

			for (blctrl_i = 0; blctrl_i < 20; blctrl_i++){

			i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE1_TWI_ADDRESS, &stellwert_blctrl_engine1, 1);
			i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE2_TWI_ADDRESS, &stellwert_blctrl_engine2, 1);
			i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE3_TWI_ADDRESS, &stellwert_blctrl_engine3, 1);
			i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE4_TWI_ADDRESS, &stellwert_blctrl_engine4, 1);
			}

			PRINTF("Motors initalized!!\n");

		}

		// For old BLCTRL
		 stellwert_blctrl_engine1 = stell.M1;
		 stellwert_blctrl_engine2 = stell.M2;
		 stellwert_blctrl_engine3 = stell.M3;
		 stellwert_blctrl_engine4 = stell.M4;

		 // NONFLYING setzt die Stellwerte immer auf 0 damit der Copter nie starten kann
		 #ifdef NONFLYING
			 stellwert_blctrl_engine1 = 0;
			 stellwert_blctrl_engine2 = 0;
			 stellwert_blctrl_engine3 = 0;
			 stellwert_blctrl_engine4 = 0;
		 #endif

		//PRINTF("\n\n*** Run Motors! ***\n");


		i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE1_TWI_ADDRESS, &stellwert_blctrl_engine1, 1);
			if (i2c_1.i2c_return < 0){
				reset_i2c();
				//PRINTF("\n\n*** Motor 1 Error ***\n");
				i2c_1.i2c->write(BLCTRL_ENGINE1_TWI_ADDRESS, &stellwert_blctrl_engine1, 1);
			}

		i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE2_TWI_ADDRESS, &stellwert_blctrl_engine2, 1);
		if (i2c_1.i2c_return < 0){
				reset_i2c();
				//PRINTF("\n\n*** Motor 2 Error ***\n");
				i2c_1.i2c->write(BLCTRL_ENGINE2_TWI_ADDRESS, &stellwert_blctrl_engine2, 1);
			}

		i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE3_TWI_ADDRESS, &stellwert_blctrl_engine3, 1);
		if (i2c_1.i2c_return < 0){
					reset_i2c();
					//PRINTF("\n\n*** Motor 3 Error ***\n");
					i2c_1.i2c->write(BLCTRL_ENGINE3_TWI_ADDRESS, &stellwert_blctrl_engine3, 1);
			}

		i2c_1.i2c_return = i2c_1.i2c->write(BLCTRL_ENGINE4_TWI_ADDRESS, &stellwert_blctrl_engine4, 1);
		if (i2c_1.i2c_return < 0){
					reset_i2c();
					//PRINTF("\n\n*** Motor 4 Error ***\n");
					i2c_1.i2c->write(BLCTRL_ENGINE4_TWI_ADDRESS, &stellwert_blctrl_engine4, 1);
			}
}
