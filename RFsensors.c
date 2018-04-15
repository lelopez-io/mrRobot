/*
 * RFsensors.c
 *
 *  Created on: Apr 7, 2018
 *      Author: z-bot
 */
//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "uart_library.h"
#include "RFsensors.h"

#define UART_BASE UART3_BASE
#define LOW 0


void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

void reflectanceSensors() {

	//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	uint32_t sVal = 100;

	// Configure PC5 for RF sensor
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);

	//Charge
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
	delayMS(25);

	int reflectance = 1;
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5);       //set pin as input

	//Discharge
	// read the pin state, increment counter until state = LOW
	while((reflectance < 10000) && (sVal != LOW)){   //timeout at 10000
		reflectance++;                                           // increment value to be displayed via serial port
		sVal = sensorsRead();
	}

//*------------------[Debug]-----------------------------------------//
	if (reflectance < 500){

		UARTPutString(UART_BASE, "White\n\r");
	}
	else if ((reflectance >= 500) && (reflectance < 10000)) {
		UARTPutString(UART_BASE, "Black\n\r");       //if reflectance value is over 500
	}
	else {
		UARTPutString(UART_BASE, "Time Out\n\r");            //Time Out
	}
//---------------------------[Debug End]---------------------------//

	delayMS(100);




}

int findLine() {
	uint32_t rfSR = 100;

	// Configure PC5 for RF sensor
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);

	//Charge
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
	delayMS(25);

	int reflectance = 1;
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5);       //set pin as input

	//Discharge
	// read the pin state, increment counter until state = LOW
	while((reflectance < 10000) && (rfSR != LOW)){   //timeout at 10000
		reflectance++;                                           // increment value to be displayed via serial port
		rfSR = sensorsRead();
	}

	if ((reflectance >= 500) && (reflectance < 10000)) {
		//Black
		return 1;
	}
	else {
		//White or TimeOut
		return 0;
	}
}

int sensorsRead() {
	uint32_t s2 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5);

	return s2;
}





