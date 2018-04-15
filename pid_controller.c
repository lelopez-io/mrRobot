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
#include "RFsensors.h"

#define DEBUG 0 //1 enables print statements
#define UART_BASE UART3_BASE
#define IDEAL_READING 1600

int blackLine = 0;
int tCounter = 0;
int toggle = 0;
int bufferOne[20] = { 0 };	//should be initialized
int bufferTwo[20] = { 0 };	//should be initialized
int i;

uint32_t kp = 25;
uint32_t ki = 0.34;
uint32_t kd = 0.34;

uint32_t proportion;
uint32_t integral;
uint32_t derivative;
int error;
int prev_error;
int schange;

uint32_t rightSensor = 0;
uint32_t frontSensor = 0;

//Runs with timer every 50ms
void funcBIOS() {
	rightSensor = getVALS_ADC();
	frontSensor = getVALF_ADC();

	//PID Calculations
	error = IDEAL_READING - rightSensor;

	proportion = kp * abs(error);
	integral = ki * (error + prev_error);
	derivative = kd * (error - prev_error);
	schange = proportion + integral + derivative;

	prev_error = error;

	//Data Collection
	blackLine += findLine();
	toggle = !toggle;
	if ((0 < blackLine) && (toggle)) {
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
		if (tCounter < 20) {
			bufferOne[tCounter] = error;
		} else if (tCounter >= 20)  {
			bufferTwo[tCounter - 20] = error;
		}
		tCounter += 1;

		//Write bufferOne
		if (tCounter == 20) {
			UARTPutString(UART_BASE, "bufferOne: \n\r");
			for (i = 0; i < 20; i++) {
				UARTPutInt(UART_BASE, bufferOne[i]);
				//Toggle LED Green
			}
			UARTPutString(UART_BASE, "\n\n\r");
			tCounter = 20;
			for (i = 0; i < 20; i++) {
				bufferTwo[i] = 0;
			}
		}
		//Write bufferTwo
		if (tCounter == 40) {
			UARTPutString(UART_BASE, "bufferTwo: \n\r");
			for (i = 0; i < 20; i++) {
				UARTPutInt(UART_BASE, bufferTwo[i]);
				//Toggle LED Green
			}
			UARTPutString(UART_BASE, "\n\n\r");
			tCounter = 0;
			for (i = 0; i < 20; i++) {
				bufferOne[i] = 0;
			}
		}
	} //End of Data Collection

/*========================[DEBUG]==============================*/
	if(DEBUG) {
		UARTPutString(UART_BASE, "*");

		if(frontSensor > 900 && frontSensor <= 1500) {
			UARTPutString(UART_BASE, "\t900 < V < 1500\t\t");
		}
		else if(frontSensor > 1500 && frontSensor <= 2200) {
			UARTPutString(UART_BASE, "\t1500 < V < 2200\t\t");
		}
		else if(frontSensor > 2200) {
			UARTPutString(UART_BASE, "\t2200 < V \t\t");
		}
		else {
			UARTPutString(UART_BASE, "\tFRONT\t\t\t");
		}

		if(rightSensor > 900 && rightSensor <= 1500) {
			UARTPutString(UART_BASE, "900 < V < 1500\t\t");
		}
		else if(rightSensor > 1500 && rightSensor <= 2200) {
			UARTPutString(UART_BASE, "1500 < V < 2200\t\t");
		}
		else if(rightSensor > 2200) {
			UARTPutString(UART_BASE, "2200 < V \t\t");
		}
		else {
			UARTPutString(UART_BASE, "SIDE\t\t\t");
		}
	}
/*======================[DEBUG END]===========================*/

	//U-Turn
	if(frontSensor > 2200) {
		while(frontSensor > 900) {
			motorsLEFT();
			frontSensor = getVALF_ADC();
		}
		//motorsOFF();
	}
	motorsFWD();

	//Follow the wall using PID
	if(rightSensor > 1500 && rightSensor <= 2200) {
		//UARTPutString(UART_BASE, "OK\t\t");
		motorsFWD();
	} else if(error < 0) {
		//UARTPutString(UART_BASE, "NEGATIVE\t");
		motorsADDR(schange);
	} else if (error > 0) {
		//UARTPutString(UART_BASE, "POSITIVE\t");
		motorsADDL(schange);
	}
	//UARTPutString(UART_BASE, "*\n\n\r");
}




