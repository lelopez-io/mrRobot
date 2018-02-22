//---------------------------------------------------------------------------------
// Project: UART Echo using Bluetooth Demo
// Authors: Hunter Hedges, Roger Canales, Matthew Hunter
// Date: Feb 2018
//
// Description: In order to use the BlueSmirf, initialize UART3 and setup according to
// pin layout. 'a' turns on blue LED, 'b', turns on red LED, 'c' turns on green LED.
// By setting var UART_BASE = UART0_BASE, the code will work through USB connection
//----------------------------------------------------------------------------------


//----------------------------------------
// BIOS header files
//----------------------------------------
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Log.h>
#include <xdc/cfg/global.h>

//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
//#include <uart_library.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "uart_library.h"

int main(void) {

	uint32_t UART_BASE = UART3_BASE; // UART3_BASE Can be switched to UART0_BASE for usb connection

	unsigned char charFromUART;

	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	Configure_UART0();
	Configure_UART3();

	// Enables port, sets pins 1-3 (RGB) pins for output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	UARTPutString(UART_BASE, "Beginning Program...\n\r");

	while (1)
	{
		if(UARTCharsAvail(UART_BASE)) {

			charFromUART = UARTCharGet(UART_BASE);

			switch(charFromUART) {
				case 'a': // Turn on Blue LED
					UARTPutString(UART_BASE, "\nCommand Received\n\r");
					UARTPutString(UART_BASE, "Turning on Blue LED...\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
					break;
				case 'b': // Turn on Red LED
					UARTPutString(UART_BASE, "\nCommand Received\n\r");
					UARTPutString(UART_BASE, "Turning on Red LED...\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
					break;
				case 'c': // Turn on Green LED
					UARTPutString(UART_BASE, "\nCommand Received\n\r");
					UARTPutString(UART_BASE, "Turning on Green LED...\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
					break;
				case 'q': // Turn off LED
					UARTPutString(UART_BASE, "\nCommand Received\n\r");
					UARTPutString(UART_BASE, "Turning off LED...\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
				default:
					UARTPutString(UART_BASE, "\nCommand Unknown\n\r");
					break;
			}
		}

	}
}
