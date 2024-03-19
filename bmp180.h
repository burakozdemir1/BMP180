/*
 * bmp180.h
 *
 *  Created on: Mar 13, 2024
 *      Author: Burak Ozdemir
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stm32f4xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

/* BMP180 Device Address*/

#define BMP180_DEVICE_WRITE_REGISTER_ADDRESS 0xEE
#define BMP180_DEVICE_READ_REGISTER_ADDRESS 0xEF




/* BMP180 Calibration Adress*/
#define BMP180_CALIBRATION_START_ADDRESS 0xAA //address of AC1


/* BMP180 Calibration Value Length --> Since we will read 11 pieces and do an 8-bit job
We cannot read 16 bits at the same time, therefore 11*2=22 */

#define BMP180_CALIBRATION_VALUE_LENGHT 22

/* Calibration Value   */
extern short 	 AC1;
extern short 	 AC2;
extern short 	 AC3;
extern unsigned short AC4;
extern unsigned short AC5;
extern unsigned short AC6;

extern short 	 B1;
extern short 	 B2;

extern short 	 MB;
extern short 	 MC;
extern short 	 MD;

extern uint8_t calibBuff[BMP180_CALIBRATION_VALUE_LENGHT] ;

/* Temperature Values */
extern int16_t unCompTemperature;
extern float temperature;
extern int32_t tX1,tX2,tB5;
extern uint8_t rData[2];
extern uint8_t wData[1];
void BMP180_Init(void);
void BMP180_GetCalibration(void);
void BMP180_Get_Calibration_Value(void);
void BMP180_Get_Uncompansated_Temperature(void);
void BMP180_Get_Temperature(void);
void BMP180_Get_Uncompansated_Presure(void);
void BMP180_Get_Presure(void);
void fillData(uint8_t* calibDatas) ;

#endif /* INC_BMP180_H_ */
