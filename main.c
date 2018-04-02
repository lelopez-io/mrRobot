//---------------------------------------------------------------------------------
// Project: PWM using Bluetooth
// Authors: Luis E. Lopez, Oscar Zuniga
// Date: March 2018
//
// Description: In order to use the BlueSmirf, initialize UART3 and setup according to
// pin layout. This program uses command to direct the motors (forward, backwards, left,
// right, speed up, and speed down) with the help of the PWM functions of the tiva c.
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

#include "adc.h"
#include "pwm_motors.h"
#include "uart_library.h"


//#include "inc/hw_types.h"
//#include "driverlib/pwm.h"
//#include "driverlib/pin_map.h"

/* PINS
 * Bluetooth:
 * 	Orange PC6
 * 	Yellow PC7
 *
 * Motors:
 * 	Blue PD0 & PD1
 *
 * */




int main(void) {

	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	unsigned char charFromUART;
	uint32_t UART_BASE = UART0_BASE; // UART3_BASE can be switched to UART0_BASE for USB connection
	Configure_UART0();
	Configure_UART3();
	UARTPutString(UART_BASE, "UART Configured...\n\n\r");


	Configure_PWM();

	Configure_ADC();

	// Enables port, sets pins 1-3 (RGB) pins for output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2);

	// Enable Port E
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);



	UARTPutString(UART_BASE, "Connection Established...\n\n\r");
	UARTPutString(UART_BASE, "Select a status for the Motor:\n\r");
	UARTPutString(UART_BASE, "    fw = FORWARD\n\r");
	UARTPutString(UART_BASE, "    bk = BACK\n\r");
	UARTPutString(UART_BASE, "    tr = RIGHT TURN\n\r");
	UARTPutString(UART_BASE, "    tl = LEFT TURN\n\r");
	UARTPutString(UART_BASE, "    su = SPEED UP\n\r");
	UARTPutString(UART_BASE, "    sd = SLOW DOWN\n\r");
	UARTPutString(UART_BASE, "    off = STOP\n\r");
	UARTPutString(UART_BASE, "Press ENTER after typing your selection\n\n\r");



	int i;
	char data[10];

	//Speed of motors
	unsigned long pwmNow = 150;

	while (1)
	{
		//Create an array for commands
		for (i = 0; i < 10; i++) {
			data[i] = '\0';
		}



		if (UARTCharsAvail(UART_BASE)) {
			charFromUART = UARTCharGet(UART_BASE);

			//Get command
			i = 0;
			while ( charFromUART != '\r') {
				data[i] = charFromUART;
				charFromUART = UARTCharGet(UART_BASE);
				i++;

				if (i > 10)
					UARTPutString(UART_BASE, "\rExceeded command length\n\r\r");
			}

			//Execute command
			if (strcmp(data, "off") == 0) {
				UARTPutString(UART_BASE, "Selection: motor OFF\n\r");
				motorsOFF();

			}
			else if (strcmp(data, "fw") == 0) {
				UARTPutString(UART_BASE, "Selection: go FORWARD\n\r");
				motorsFWD();
			}
			else if (strcmp(data, "bk") == 0) {
				UARTPutString(UART_BASE, "Selection: turn BACK\n\r");
				motorsBACK();
			}
			else if (strcmp(data, "tr") == 0) {
				UARTPutString(UART_BASE, "Selection: turn RIGHT\n\r");
				motorsRIGHT();
			}
			else if (strcmp(data, "tl") == 0) {
				UARTPutString(UART_BASE, "Selection: turn LEFT\n\r");
				motorsLEFT();
			}
			else if (strcmp(data, "su") == 0) {
				UARTPutString(UART_BASE, "Selection: speed UP\n\r");
				//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
				pwmNow += 20;
				motorsSPEED(pwmNow);
			}
			else if (strcmp(data, "sd") == 0) {
				UARTPutString(UART_BASE, "Selection: speed DOWN\n\r");
				//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
				pwmNow -= 20;
				motorsSPEED(pwmNow);
			}
			else if (strcmp(data, "off") == 0) {
				UARTPutString(UART_BASE, "Selection: OFF\n\r");
				motorsOFF();
			}
			else {
				UARTPutString(UART_BASE, "Unknown Command!!!\n\n\r");
			}

		}
	}
}
