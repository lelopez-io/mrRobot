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
#include <string.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "uart_library.h"

int main(void) {

	uint32_t UART_BASE = UART0_BASE; // UART3_BASE can be switched to UART0_BASE for USB connection

	unsigned char charFromUART;

	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	Configure_UART0();
	Configure_UART3();

	// Enables port, sets pins 1-3 (RGB) pins for output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	UARTPutString(UART_BASE, "Connection Established...\n\n\r");
	UARTPutString(UART_BASE, "Select a status for the LED:\n\r");
	UARTPutString(UART_BASE, "    red\n\r");
	UARTPutString(UART_BASE, "    green\n\r");
	UARTPutString(UART_BASE, "    blue\n\r");
	UARTPutString(UART_BASE, "    off\n\r");
	UARTPutString(UART_BASE, "Press ENTER after typing your selection\n\n\r");



	int i;
	char data[10];
	while (1)
	{
		for (i = 0; i < 10; i++) {
			data[i] = '\0';
		}
		i = 0;


		if (UARTCharsAvail(UART_BASE)) {
		charFromUART = UARTCharGet(UART_BASE);



			while ( charFromUART != '\r') {
				data[i] = charFromUART;
				charFromUART = UARTCharGet(UART_BASE);
				i++;

				if (i > 10)
					UARTPutString(UART_BASE, "\rExceeded command length\n\r\r");
			}


		if (strcmp(data, "red") == 0) {
			UARTPutString(UART_BASE, "Selection: turn LED RED\n\r");
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
		}
		else if (strcmp(data, "green") == 0) {
			UARTPutString(UART_BASE, "Selection: turn LED GREEN\n\r");
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
		}
		else if (strcmp(data, "blue") == 0) {
			UARTPutString(UART_BASE, "Selection: turn LED BLUE\n\r");
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
		}
		else if (strcmp(data, "off") == 0) {
			UARTPutString(UART_BASE, "Selection: turn LED OFF\n\r");
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
		}
		else {
			UARTPutString(UART_BASE, "Unknown Command!!!\n\n\r");
		}

	}
	}
}
