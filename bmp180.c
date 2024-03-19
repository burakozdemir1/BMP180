/*
 * bmp180.c
 *
 *  Created on: Mar 13, 2024
 *      Author: Burak Ozdemir
 */

#include "bmp180.h"

 short 	 AC1;
 short 	 AC2;
 short 	 AC3;
 unsigned short AC4;
 unsigned short AC5;
 unsigned short AC6;

 short 	 B1;
 short 	 B2;

 short 	 MB;
 short 	 MC;
 short 	 MD;

 float temperature;

int16_t unCompTemperature;
uint8_t rData[2];
uint8_t wData[1];
int32_t tX1,tX2,tB5;
uint8_t calibBuff[BMP180_CALIBRATION_VALUE_LENGHT] ;

void BMP180_Init() 									//check if there is a device or not
{
	if(HAL_I2C_IsDeviceReady(&hi2c2, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 1, 100000)!=HAL_OK)
	{




		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1) ;

	}

	BMP180_Get_Calibration_Value(); //There are 11 16 bit calibration data in the Eeprom , take them.

}


void BMP180_Get_Calibration_Value()
{
	uint8_t a = 0 ;


	HAL_I2C_Mem_Read(&hi2c2, 0xEE, BMP180_CALIBRATION_START_ADDRESS, 1, calibBuff, BMP180_CALIBRATION_VALUE_LENGHT, 100000) ;


	 AC1 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 AC1 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 AC3 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 AC4 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 AC5 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 AC6 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;


	 B1 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 B2 = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;

	 MB = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 MC = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;
	 MD = (int16_t)((calibBuff[a] << 8) | calibBuff[a+1]); a+=2 ;



//	 if(AC1 == 0x0000 || AC1 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(AC2 == 0x0000 || AC2 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(AC3 == 0x0000 || AC3 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(AC4 == 0x0000 || AC4 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//
//	 if(AC5 == 0x0000 || AC5 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(AC6 == 0x0000 || AC6 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//
//	 if(B1 == 0x0000 || B1 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(B2 == 0x0000 || B2 == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(MB == 0x0000 || MB == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(MC == 0x0000 || MC == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//	 if(MD == 0x0000 || MD == 0xFFFF)
//	 {
//			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
//			while(1);
//	 }
//
//



}




void BMP180_Get_Temperature(void)
{
	BMP180_Get_Uncompansated_Temperature(); // UT değerini bu fonksiyonda çektik
	tX1 = (((int32_t)unCompTemperature - (int32_t)AC6) * (int32_t)AC5) / 32768;
//	tX2 = ((int32_t)MC * 2048) / (tX1 + (int32_t)MD) ;

	tB5 = tX1 + tX2 ;
	temperature = (float) (((tB5 + 8) / 16) * 0.1);

	if(tX1 == 0 && MD == 0)
	{
		/* it means there's a mistake, if there's a mistake , let it wait.*/
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1) ;
	//	while(1);
	}


}

void BMP180_Get_Uncompansated_Temperature(void)
{

	wData[0]=0x2E;
	HAL_I2C_Mem_Write(&hi2c2, BMP180_DEVICE_WRITE_REGISTER_ADDRESS, 0xF4, 1, wData, 1, 100000) ;
	HAL_Delay(50);

	HAL_I2C_Mem_Read(&hi2c2, BMP180_DEVICE_READ_REGISTER_ADDRESS, 0xF6, 1, rData, 2, 100000) ;
	unCompTemperature = (uint16_t)((rData[0]<<8) | rData[1]);
}

void BMP180_Get_Presure(void)
{
	BMP180_Get_Uncompansated_Presure() ;

}


void BMP180_Get_Uncompansated_Presure(void)
{



}



















