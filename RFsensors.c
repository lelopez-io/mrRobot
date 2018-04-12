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

#define UART_BASE UART0_BASE
#define LOW 0


void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

void reflectanceSensors() {

	//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	int sVal = 100;

	while(1) {

		// Configure PC5 for RF sensor
		GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);

		//Charge
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5);
		delayMS(25);

		int reflectance = 1;
		GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5);       //set pin as input

		//Discharge
		// read the pin state, increment counter until state = LOW
		while((reflectance < 10000) && ((sVal = sensorsRead()) != LOW)){   //timeout at 10000

			reflectance++;                                           // increment value to be displayed via serial port
			}

//*------------------[Debug]-----------------------------------------//
		if (reflectance < 100){

			UARTPutString(UART_BASE, "Less than 100");
		}
		else if ((reflectance >= 100) && (reflectance < 200)){

					UARTPutString(UART_BASE, "Less than 200");
		}
		else if ((reflectance >= 200) && (reflectance < 300)){

					UARTPutString(UART_BASE, "Less than 300");
		}
		else if ((reflectance >= 300) && (reflectance < 400)){

					UARTPutString(UART_BASE, "Less than 400");
		}
		else if ((reflectance >= 400) && (reflectance < 500)){

					UARTPutString(UART_BASE, "Less than 500");
		}
		else if ((reflectance >= 500) && (reflectance < 10000)) {
			UARTPutString(UART_BASE, "More than 500");       //if reflectance value is over 500
		}
		else {
			UARTPutString(UART_BASE, "Time Out");            //Time Out
		}
//---------------------------[Debug End]---------------------------//

		delayMS(100);


	}

}

int sensorsRead() {
	uint32_t s2 = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_5);

	return s2;
}





