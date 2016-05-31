/*
 * KalmanFilter.cpp
 *
 *  Created on: 11.03.2015
 *      Author: Gageik
 */

#include "KalmanFilter.h"
#include "myMath.h"
#include "myTopics.h"
#include <math.h>
#include "rodos.h"
#include "myTopics.h"

class Filter_KF: public Thread {

	KalmanFilter kf_roll;
	KalmanFilter kf_pitch;

public:
	Filter_KF(const char* name, const long prio) :
			Thread(name, prio) {

	}

	void run(){

		filter_count = filter_count + 1;

		PRINTF("\n\n*** KF initialized ***\n");


		this->setPeriodicBeat(1*SECONDS, 10*MILLISECONDS);

		while(1){

				// Daten anfordern
				IMU_Data imu_new;
				imu.get(imu_new);


				// Add Code here

				suspendUntilNextBeat();
		}

	}

};


Filter_KF 		 	kf("kf", 90);



KalmanFilter::KalmanFilter() {
	// Add Code here

}

KalmanFilter::~KalmanFilter() {}


