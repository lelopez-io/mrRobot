//---------------------------------------------------------------------------------
// Project: UART Library
// Authors: Hunter Hedges, Roger Canales, Matthew Hunter
// Date: Feb 2018
//
//
//----------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "uart_library.h"

/*************************************************
 * Configure UART 0
 * BaudRate = 115200
 * PA0 = U0RX
 * PA1 = U0TX
 ************************************************/
void Configure_UART0() {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

}

/*************************************************
 * Configure UART 3
 * BaudRate = 9600
 * PC6 = U3RX
 * PC7 = U3TX
 ************************************************/
void Configure_UART3() {

	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	GPIOPinConfigure(GPIO_PC6_U3RX);
	GPIOPinConfigure(GPIO_PC7_U3TX);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	UARTConfigSetExpClk(UART3_BASE, SysCtlClockGet(), 9600,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}

/*************************************************
 * Send string to UART
 *
 ************************************************/
void UARTPutString(uint32_t UART_BASE, unsigned char *stringToSend)
{
    while(*stringToSend != 0)
    {
    	UARTCharPut(UART_BASE, *stringToSend++);
    }
}

/*************************************************
 * Get string from UART
 *
 ************************************************/
void UARTGetString(uint32_t UART_BASE, unsigned char *stringFromUART, unsigned long ulCount){

	unsigned long charcount = 0;

	while (charcount <= ulCount) {

		stringFromUART[charcount] = UARTCharGet(UART_BASE);

		if ((stringFromUART[charcount] == '\r') || (stringFromUART[charcount] == '\n'))
			break;

		charcount++;
	}

	stringFromUART[charcount] = 0;
}
