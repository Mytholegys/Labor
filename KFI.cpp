/*
 * KFI.cpp
 *
 *  Created on: 12.03.2015
 *      Author: Gageik
 */

#include "KFI.h"
#include "rodos.h"
#include "myTopics.h"


class Filter_KFI: public Thread {

	KFI kfi_roll;
	KFI kfi_pitch;

	public:
	Filter_KFI(const char* name, const long prio) :
			Thread(name, prio) {

	}

	void run(){

		filter_count = filter_count + 1;

		PRINTF("\n\n*** KFI initialized ***\n");


		this->setPeriodicBeat(1*SECONDS, 10*MILLISECONDS);

		while(1){

				IMU_Data imu_new;
				imu.get(imu_new);

				// Add Code here

				if (filter_count == 1){

				Winkel neueWinkel;

				// Add Code here


				rpy_winkel.put(neueWinkel);
				}

				suspendUntilNextBeat();
		}

	}

};

Filter_KFI 		 	kfi("kfi", 89);


KFI::KFI() {		initModell();	}

KFI::~KFI() {	}



void KFI::initModell(){

	// Add Code here


}


State KFI::step(State pre){
	State next = pre;

	// Add Code here

	return next;
}


Sensors KFI::senseState(Sensors alt, double* measurements){
	Sensors z = alt;

	// Add Code here

	return z;
}



double KFI::process(double* measurements){

	modell = step(modell);

	sensors = senseState(sensors, measurements);

	// Fusion to be adapted
	// mergeVal hier!
	//modell.x.mergeVal(sensors.x);
	//modell.y.mergeVal(sensors.y);

	//Output, to be changed
	return 0;
}
