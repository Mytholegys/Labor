/*
 * Control.h
 *
 *  Created on: 20.03.2015
 *      Author: Gageik
 */

#include "rodos.h"
#include "myStructs.h"

#ifndef CONTROL_H_
#define CONTROL_H_

class Control{
public:
	Control();

	virtual ~Control();

	Stellwerte run();
};


#endif /* CONTROL_H_ */
