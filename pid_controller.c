/*
 * pid_controller.c
 *
 *  Created on: Apr 3, 2018
 *      Author: lelopez
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"

#include "adc.h"
#include "pwm_motors.h"
#include "uart_library.h"

#define UART_BASE UART3_BASE

uint32_t ui32ADC0Value[1]; // storing the data read from ADC FIFO
uint32_t ui32ADC1Value[1];




uint32_t proportion;
uint32_t integral;
uint32_t derivative;
uint32_t i, j;
uint32_t kp = 15;
uint32_t ki = 0.34;
uint32_t kd = 0.34;
int error;
int prev_error;
int schange;
uint32_t Right_Sensor = 0;
int Front_Sensor = 0;
const int IDEAL_READING = 1600;
unsigned char* motor_speed;
unsigned char* difference;
unsigned char* leftspeed;
unsigned char* rightspeed;

unsigned char* myChar;


void funcBIOS() {
	Right_Sensor = getVALS_ADC();

//Print distance reading from side facing sensor
	if (Right_Sensor > 900 && Right_Sensor <= 1500) {
		UARTPutString(UART_BASE, "900 < V < 1500\n\n\r");
	}
	else if (Right_Sensor > 1500 && Right_Sensor <= 2200) {
		UARTPutString(UART_BASE, "1500 < V < 2200\n\n\r");
	}
	else if (Right_Sensor > 2200) {
		UARTPutString(UART_BASE, "2200 < V \n\n\r");
	}
	else {
		UARTPutString(UART_BASE, "SIDE\n\n\r");
	}
///////////////////////////////////////////////////
	error = IDEAL_READING - Right_Sensor;

	proportion = kp * abs(error);
	integral = ki * (error + prev_error);
	derivative = kd * (error - prev_error);
	schange = proportion + integral + derivative;

	prev_error = error;

	if (Right_Sensor > 1500 && Right_Sensor <= 2200) {
		UARTPutString(UART_BASE, "\t\tOK\n\n\r");
		motorsFWD();
	} else if(error < 0){
		UARTPutString(UART_BASE, "\t\tNEGATIVE\n\n\r");
		motorsADDR(schange);
	} else if (error > 0){
		UARTPutString(UART_BASE, "\t\tPOSITIVE\n\n\r");
		motorsADDL(schange);
	}

}



