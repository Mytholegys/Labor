/*
 * KFI.h
 *
 *  Created on: 12.03.2015
 *      Author: Gageik
 */

#ifndef KFI_H_
#define KFI_H_

#include "trusted.h"

#define SAMPLE_TIME_KFI		10

struct State{
	TrustedVal	dummy;
};

struct Sensors{
	TrustedVal	dummy;
};

class KFI {
public:
	KFI();
	virtual ~KFI();

	State modell;
	Sensors sensors;

	void initModell();
	State step(const State pre);
	Sensors senseState(const Sensors alt, double* measurements);

	double process(double* measurements);
};

#endif /* KFI_H_ */
