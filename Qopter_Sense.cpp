/*
 * Qopter.cpp
 *
 * Created on: 25.06.2014
 * Edited:		  03.2015
 * Author: Atheel Redah (Original STM + Rodos Framework)
 *		   Nils Gageik (Quadcopter Implementation)
 */

#include "rodos.h"
#include "hal.h"
#include "matlib.h"
#include "KalmanFilter.h"
#include "KFI.h"
#include "myMath.h"
#include "Actuator.h"
#include "myTopics.h"

static Application module01("Qopter", 2015);

//{{{{{{{{{{{{{{{{{{{{{{{			IMU			******************************************************************************
// ADXL
#define ADXL_TWI_ADDRESS      		0x53    // ADXL TWI address

#define ADXL_SENSING_ADDR_START 	0x32    // Start->Address of ADXL Sensor Values

#define ADXL_POWER_CTRL_ADR	 		0x2D    // Configuration Address for ADXL (POWER_CTRL)
#define ADXL_DATA_FORMAT_ADR 		0x31    // Configuration Address for ADXL (FORMAT_ADR)
#define ADXL_BW_RATE_ADR			0x2C	// Set Frequency to 800 Hz

#define ADXL_POWER_CTRL_DATA 		0x08
#define ADXL_DATA_FORMAT_DATA		0x0B
#define ADXL_BW_RATE_DATA			0x0D

#define ADXL_SCALE_FACTOR			3.9f

uint8_t adxl_power_reg[2]={ADXL_POWER_CTRL_ADR,ADXL_POWER_CTRL_DATA};
uint8_t adxl_data_format_reg[2]={ADXL_POWER_CTRL_ADR,ADXL_POWER_CTRL_DATA};
uint8_t adxl_bw_rate_reg[2]={ADXL_BW_RATE_ADR,ADXL_BW_RATE_DATA}

uint8_t adxl_sens_addr = ADXL_SENSING_ADDR_START;

// ITG
#define ITG_TWI_ADDRESS      		0x68    // ITG TWI address

#define ITG_SENSING_ADDR_START 		0x1D    // Start->Address of ITG Sensor Values

#define ITG_INIT_DLPF_ADDR		 	0x16    // Configuration Address for ITG
#define ITG_INIT_PM_ADDR	 		0x3E    // PowerManagement Setup: X Gyro as reference clock (recommended)
#define ITG_SMPLRT_DIV_ADR			0x15	// Sample Rate Divider: 8 -> 8ms per Sample bei 1kHz

#define ITG_INIT_DLPF_DATA		 	0x11
#define ITG_INIT_PM_DATA	 		0x01
#define ITG_SMPLRT_DIV_DATA			0x07

uint8_t itg_sens_addr = ITG_SENSING_ADDR_START; // GyroX High

uint8_t itg_twi_addr = ITG_TWI_ADDRESS;

uint8_t itg_dlpf_reg[2]={ITG_INIT_DLPF_ADDR,ITG_INIT_DLPF_DATA};
uint8_t itg_pwr_reg[2] = {ITG_INIT_PM_ADDR,ITG_INIT_PM_DATA};
uint8_t itg_smplrt_div_reg[2]={ITG_SMPLRT_DIV_ADR,ITG_SMPLRT_DIV_DATA};


#define ITG_SCALE_FACTOR			32.8f

#define BIAS_SAMPLES				2000
#define SENSE_SAMPLE_TIME			10

#define GPIO_BUTTON_BLUE GPIO_000
//}}}}}}}}}}}}}}}}}}}}}}}			IMU			******************************************************************************

//#define IMU_I2C I2C_IDX2
//HAL_UART usart3(USB_UART);

HAL_I2C i2c1(I2C_IDX1);
I2C_Data i2c_1;
#define I2C_ERROR_GRENZE		3

HAL_GPIO pin(GPIO_BUTTON_BLUE);

/* Example: CommBuffer, Topic, Subscriber
CommBuffer<TelecommandData> TelecommandFifo;
Topic<TelecommandData> TelecommandTopic(-1, "Telecommands");
Subscriber Sub1(TelecommandTopic, TelecommandFifo);
*/


void reset_i2c(){
	i2c1.reset();
	AT(NOW() + 0.5*MILLISECONDS);
	i2c1.init();
}

class Qopter: public Thread {

public:

	Qopter(const char* name, const long prio) :
			Thread(name, prio) {

	}

	void init() {			i2c1.init();		i2c_1.i2c_init = 0;		i2c_1.error_counter = 0;	}
	void reset_i2c(){
			i2c1.reset();
			i2c_1.i2c_init=false;
			AT(NOW() + 0.5*MILLISECONDS);
			i2c1.init();
			i2c_1.i2c_init=true;
		}
	IMU_RAW_Data sense() {

		IMU_RAW_Data	imu_new_values;

		// Add your code here				---->			Read from Sensors

		if(!i2c_1.i2c_init){
			reset_i2c();
			i2c_1.i2c_init=true;
		}
		// @sensorBuffer: sensorBuffer[0-5] : gyro | sensorBuffer[6-11]: accel
		if(i2c1.writeRead(itg_sens_addr,itg_twi_addr , 1, imu_new_values.sensorBuffer, 1)<0){
			reset_i2c();
		}
		// Buffer Positions ausgetauscht weil high / low Bit vertauscht sind
		imu_new_values.gyro_raw.x = ((int16_t) (imu_new_values.sensorBuffer[1] | (imu_new_values.sensorBuffer[0] << 8)))*ITG_SCALE_FACTOR;
		imu_new_values.gyro_raw.y = ((int16_t) (imu_new_values.sensorBuffer[3] | (imu_new_values.sensorBuffer[2] << 8)))*ITG_SCALE_FACTOR;
		imu_new_values.gyro_raw.z = ((int16_t) (imu_new_values.sensorBuffer[5] | (imu_new_values.sensorBuffer[4] << 8)))*ITG_SCALE_FACTOR;

		// Buffer Positions ausgetauscht weil high - low Reihenfolge
		imu_new_values.accel_raw.x = ((int16_t) (imu_new_values.sensorBuffer[7] | (imu_new_values.sensorBuffer[6] << 8)))*ADXL_SCALE_FACTOR;
		imu_new_values.accel_raw.y = ((int16_t) (imu_new_values.sensorBuffer[9] | (imu_new_values.sensorBuffer[8] << 8)))*ADXL_SCALE_FACTOR;
		imu_new_values.accel_raw.z = ((int16_t) (imu_new_values.sensorBuffer[11] | (imu_new_values.sensorBuffer[10] << 8)))*ADXL_SCALE_FACTOR;
		/*		HILFE PRINTF
		 PRINTF("Acc : %2.2f, %2.2f, %2.2f, || , Gyro : %2.2f, %2.2f, %2.2f\n", ax, ay, az, gx, gy, gz);
		 */

		return imu_new_values;
	}



	void imu_setup() {

		// Setup ITG
		PRINTF("Setup ITG: ");

		// Add your code here				---->			Set ITG

		if(i2c1.write(ITG_TWI_ADDRESS,itg_pwr_reg,2)<0){
			reset_i2c();
		}
		if(i2c1.write(ITG_TWI_ADDRESS,itg_dlpf_reg,2)<0){
			reset_i2c();
		}
		if(i2c1.write(ITG_TWI_ADDRESS,itg_smplrt_div_reg,2)<0){
			reset_i2c();
		}
		PRINTF("Setup ITG finished\n");

		// Setup ADXL
		PRINTF("Setup ADXL: ");

		// Add your code here				---->			Set ADXL

		if(i2c1.write(ADXL_TWI_ADDRESS,adxl_bw_rate_reg)<0){
			reset_i2c();
		}
		if(i2c1.write(ADXL_TWI_ADDRESS,adxl_data_format_reg)<0){
			reset_i2c();
		}
		if(i2c1.write(ADXL_TWI_ADDRESS,adxl_power_reg)<0){
			reset_i2c();
		}
		PRINTF("Setup ADXL finished\n");
	}

	IMU_Offset calibrate() {


		PRINTF("IMU Calibration:\n");
		// Add your code here				---->			Calibrate Sensors
		IMU_Offset calibrate;
		calibrate.gyro_offsets=gyroCalibration();
		calibrate.accel_offsets=accelorationSensorCalibration();
		PRINTF("Calibration Finished\n");
		return calibrate;
	}

	Vector3D gyroCalibration(){ // passt
		PRINTF("Gyro running: ");
		PRINTF("Dont touch!\n");
		suspendCallerUntil(NOW()+1000*MILLISECONDS);
		Vector3D sum;
		int samplesize=1000;
		IMU_Data data;
		for(int i = 0; i < samplesize; i++){
			data = sense();
			sum.x = sum.x + data.gyro.x;
			sum.y = sum.y + data.gyro.y;
			sum.z = sum.z + data.gyro.z;
			suspendCallerUntil(NOW()+SENSE_SAMPLE_TIME*MILLISECONDS);
		}
		data.gyro_offsets.x = sum.x / samplesize;
		data.gyro_offsets.y = sum.y / samplesize;
		data.gyro_offsets.z = sum.z / samplesize;
		PRINTF("Gyro Calibration finished\n");
		return data.gyro_offsets;
	}

	Vector3D accelorationSensorCalibration(){

			int sampleSize = 1000;
			Vector3D sum;
			IMU_Offset data;
			Vector3D offset1;
			Vector3D offset2;
			PRINTF(" horizontale Position (Z = 1g) , dann pin drücken\n");

			//suspendCallerUntil(NOW() + 1000 * MILLISECONDS); //Delay, damit der Knopf richtig funktioniert

			float button_pressed = 0;
			/*	while(button_pressed < 1){
				button_pressed=pin.readPins();
				suspendCallerUntil(NOW()+50*MILLISECONDS);
			}
			*/

			for(int i = 0; i < sampleSize; i++){
				data=sense();
				sum.x = sum.x + data.accel.x ;
				sum.y = sum.y + data.accel.y ;
				suspendCallerUntil(NOW()+SENSE_SAMPLE_TIME*MILLISECONDS);
			}

			offset1.x = sum.x / sampleSize;
			offset1.y = sum.y / sampleSize;

			/*
			PRINTF("Längsseite (X = 1g), dann pin drücken \n"); // Alt kurze Seite x=1
			button_pressed = 0;
			while(button_pressed < 1){
				button_pressed=pin.readPins();
				suspendCallerUntil(NOW()+50*MILLISECONDS);
			}
			PRINTF("ok\n");
			sum(0,0,0);
			for(int i=0; i < sampleSize; i++){
				data=sense();
				sum.z = sum.z + data.accel.z ;
				sum.y = sum.y + data.accel.y ;
				suspendCallerUntil(NOW()+SENSE_SAMPLE_TIME*MILLISECONDS);
			}
			offset1.z = sum.z / sampleSize;
			offset2.y = sum.y / sampleSize;

			PRINTF(" Kurze Seite (Y = 1g), dann pin drücken\n ");
			button_pressed = 0;
			while(button_pressed < 1){
				button_pressed=pin.readPins();
				suspendCallerUntil(NOW()+50*MILLISECONDS);
			}
			PRINTF("ok\n");
			sum(0,0,0);
			for(int i=0;i<sampleSize;i++){
				data=sense();
				sum.z = sum.z + data.accel.z ;
				sum.x = sum.x + data.accel.x ;
				suspendCallerUntil(NOW()+SENSE_SAMPLE_TIME*MILLISECONDS);
			}
			offset2.z = sum.z / sampleSize;
			offset2.x = sum.x / sampleSize;

			data.accel_offsets.x=( offset1.x + offset2.x) / 2 ;
			data.accel_offsets.y=( offset1.y + offset2.y) / 2 ;
			data.accel_offsets.z=( offset1.z + offset2.z) / 2 ;
			*/
			data.accel_offsets.x=offset1.x;
			data.accel_offsets.y=offset1.y;


			PRINTF("Die Offsetfehlerbetragen:\n X: %f \n Y: %f\n Z: %f\n",
					data.accel_offsets.x,data.accel_offsets.y,data.accel_offsets.z);

			return data.accel_offsets;
		}

	void run() {

		imu_setup();

		IMU_Offset offset = calibrate();

		i2c_1.i2c = &i2c1;
		i2c_1.i2c_init = 1;

		PRINTF("\n\n**** I2C+IMU initialized! ***\n\n");

		this->setPeriodicBeat(0, SENSE_SAMPLE_TIME*MILLISECONDS);

		while (1) {


			// Read Sensors
			IMU_RAW_Data imu_new = sense();

			// Condition & Process
			IMU_Data imu_val;
			imu_val.gyro=imu_new.gyro_raw - offset.gyro_offsets;
			imu_val.accel=imu_new.accel_raw - offset.accel_offsets;


			suspendUntilNextBeat();
		}

	}

};


Qopter 		 	qopter("AQopterI8", 333);				// Main + Sense
/***********************************************************************/
