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

int findLine() {
	uint32_t rfFront = 100;
	uint32_t rfBack = 100;

	// Configure PC5 for RF sensor
	GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_4);

	//Charge
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4);
	delayMS(15);

	int reflectance = 1;
	GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5 | GPIO_PIN_4);       //set pin as input

	//Discharge
	// read the pin state, increment counter until state = LOW
	while((reflectance < 10000) && ((rfFront + rfBack) != LOW)){   //timeout at 10000
	                                           // increment value to be displayed via serial port
		rfFront = sensorsRead1();
		rfBack = sensorsRead2();
		reflectance++;
	}

	if ((reflectance > 500) && (reflectance < 10000)) {
		//Black
		//UARTPutString(UART_BASE, "Black line\r\n\n");
		return 1;
	}
	else {
		//White or TimeOut
		//UARTPutString(UART_BASE, "White\r\n\n");
		return 0;
	}
}

int sensorsRead1() {
	uint32_t s1 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5);

	return s1;
}

int sensorsRead2() {
	uint32_t s2 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);

	return s2;
}




