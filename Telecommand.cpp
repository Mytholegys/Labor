/*
 * Telecommand.cpp
 *
 *  Created on: 23.03.2015
 *      Author: Gageik
 */

#include "rodos.h"

#include "myTopics.h"

extern bool engine_on;
extern I2C_Data i2c_1;

#define LED_GREEN GPIO_060
#define LED_ORANGE GPIO_061
#define LED_RED GPIO_062
#define LED_BLUE GPIO_063

HAL_GPIO led_green(LED_GREEN);
HAL_GPIO led_organe(LED_ORANGE);
HAL_GPIO led_red(LED_RED);
HAL_GPIO led_blue(LED_BLUE);

HAL_UART uart_stdout(UART_DEBUG);

#define USART_BUFFER_SIZE				100
char usart_buffer[USART_BUFFER_SIZE];

char dummy = 0;


class Telecommand: public Thread {

public:

	Telecommand(const char* name, const long priority) :
			Thread(name, priority) {
	}

	void init() {
		led_green.init(true, 1, 0);
		led_organe.init(true, 1, 0);
		led_red.init(true, 1, 0);
		led_blue.init(true);
	}


	void run() {

		long long jetzt_zeit;

		PRINTF("\n\n*** TM/TC initialized ***\n");
		this->setPeriodicBeat(0, 100*MILLISECONDS);

		while (1) {

			jetzt_zeit = (long long)NOW();


			// TM

			// ****		add TM here		****

			// TC

			// ****		add TC here		****

			// Toggle LED, to be removed
			if (led_red.readPins() == 0)
					led_red.setPins(1);
				else
					led_red.setPins(0);



			suspendUntilNextBeat();
		}
	}
};


Telecommand 	Telecommand("TM/TC", 101);					// TM/TC

