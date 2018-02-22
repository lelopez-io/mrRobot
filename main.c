//---------------------------------------------------------------------------------
// Project: UART Echo using Bluetooth Demo
// Authors: Luis E. Lopez, Oscar Zuniga
// Date: Feb 2018
//
// Description: In order to use the BlueSmirf, initialize UART3 and setup according to
// pin layout. 'r' turns on RED LED, 'g', turns on GREEN LED, 'b' turns on BLUE LED.
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
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "uart_library.h"

int main(void) {

	uint32_t UART_BASE = UART3_BASE; // UART3_BASE can be switched to UART0_BASE for USB connection

	unsigned char charFromUART;

	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	Configure_UART0();
	Configure_UART3();

	// Enables port, sets pins 1-3 (RGB) pins for output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	UARTPutString(UART_BASE, "Connecting Established...\nPress r: to turn on Red LED\nPress g: to turn on Green LED\nPress b: to turn on Blue LED\nPress q: to turn off the LED\n\r");

	while (1)
	{
		if(UARTCharsAvail(UART_BASE)) {

			charFromUART = UARTCharGet(UART_BASE);

			switch(charFromUART) {
				case 'r': // Turn on Red LED
					UARTPutString(UART_BASE, "Red LED selected\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
					break;
				case 'g': // Turn on Green LED
					UARTPutString(UART_BASE, "Green LED selected\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
					break;
				case 'b': // Turn on Blue LED
					UARTPutString(UART_BASE, "Blue LED selected\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
					break;
				case 'q': // Turn off LED
					UARTPutString(UART_BASE, "Turning LED off\n\r");
					GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
				default:
					UARTPutString(UART_BASE, "\nCommand Unknown\n\r");
					break;
			}
		}

	}
}
