/*
 * pid_controller.c
 *
 *  Created on: Apr 3, 2018
 *     Authors: lopez
 *      		zuniga anzaldo
 */
#include <ti/sysbios/BIOS.h>
#include <xdc/cfg/global.h>

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

int lineCounter = 0;
bool passed1st = false;
bool collect = true;

int blackLine = 0;
int smallStrip = 0; 		//Collects data on every odd small strip 1, 3, 5
int iCounter = 0; 			//Counts on every interrupt
int tCounter = 0;			//Counts on toggle, when collect data
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
	iCounter++;
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

	if (iCounter == 3){
		UARTPutString(UART_BASE, "Check\n\r");
		iCounter = 0;
		if (blackLine == 1 ) {
			smallStrip += 1;
		}
		if (blackLine == 2) {


			SysCtlReset();
		}
		blackLine = findLine();
	}

	if ((toggle) && (smallStrip % 2 == 1)) {
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
		if (tCounter < 20) {
			bufferOne[tCounter] = error;
		} else if (tCounter >= 20)  {
			bufferTwo[tCounter - 20] = error;
		}
		tCounter += 1;

		//Post a swi to print the buffers
		if ((tCounter == 20) || (tCounter == 40)){
			Swi_post(SWI2);
		}

	} else if (smallStrip % 2 == 0) {
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
	}

	//End of Data Collection


	//U-Turn
	if(frontSensor > 2200) {
		while(frontSensor > 900) {
			motorsLEFT();
			frontSensor = getVALF_ADC();
		}
	}
	motorsFWD();

	//Follow the wall using PID
	if(rightSensor > 1600 && rightSensor <= 2100) {
		motorsFWD();
	} else if(error < 0) {
		if (rightSensor < 900) {
			schange += 15;
		}
		motorsADDR(schange);
	} else if (error > 0) {
		motorsADDL(schange);
	}
}

void printPingPong(void) {
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
	//Write bufferOne
	if (tCounter == 20) {
		UARTPutString(UART_BASE, "Buffer 1:\n\r");
		for (i = 0; i < 20; i++) {
			UARTPutInt(UART_BASE, bufferOne[i]);
			UARTPutString(UART_BASE, "\n\r");
		}

		tCounter = 20;
		for (i = 0; i < 20; i++) {
			bufferTwo[i] = 0;
		}
		UARTPutString(UART_BASE, "\n\n\r");
	}
	//Write bufferTwo
	if (tCounter == 40) {
		UARTPutString(UART_BASE, "Buffer 2:\n\r");
		for (i = 0; i < 20; i++) {
			UARTPutInt(UART_BASE, bufferTwo[i]);
			UARTPutString(UART_BASE, "\n\r");
		}
		tCounter = 0;
		for (i = 0; i < 20; i++) {
			bufferOne[i] = 0;
		}
		UARTPutString(UART_BASE, "\n\n\r");
	}
}



