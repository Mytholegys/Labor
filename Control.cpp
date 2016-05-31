/*
 * Control.cpp
 *
 *  Created on: 20.03.2015
 *      Author: Gageik
 */

#include "Control.h"
#include "myMath.h"
#include "rodos.h"
#include "matlib.h"
#include "myTopics.h"


Control::Control() {

	// Add Code here


}

Control::~Control() {}

Stellwerte Control::run(){

	Stellwerte out;

	if(engine_on){

			Winkel neueWinkel;
			rpy_winkel.get(neueWinkel);


			// Add Code here


		}

	return out;
}
