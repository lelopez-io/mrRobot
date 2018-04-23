/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

/* Board Header file */
#include "Board.h"

//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdio.h>
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
#include "navigate.h"
#include "RFsensors.h"

#define UART_BASE UART3_BASE // UART3_BASE can be switched to UART0_BASE for USB connection

/*************[ Main ]************************************/

int main(void) {

	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//uint32_t UART_BASE = UART3_BASE; // UART3_BASE can be switched to UART0_BASE for USB connection
	Configure_UART0();
	Configure_UART3();

	Configure_PWM();
	Configure_ADC();


	//Speed of motors
	unsigned long pwmNow = 150;

	int i;
	char data[10]; //Will store command from UART
	unsigned char charFromUART;

	UARTPutString(UART_BASE, "Connection Established...\n\n\r");
	UARTPutString(UART_BASE, "Select a status for the Motor:\n\r");
	UARTPutString(UART_BASE, "    fw = FORWARD\n\r");
	UARTPutString(UART_BASE, "    bk = BACK\n\r");
	UARTPutString(UART_BASE, "    tr = RIGHT TURN\n\r");
	UARTPutString(UART_BASE, "    tl = LEFT TURN\n\r");
	UARTPutString(UART_BASE, "    su = SPEED UP\n\r");
	UARTPutString(UART_BASE, "    sd = SLOW DOWN\n\r");
	UARTPutString(UART_BASE, "    off = STOP\n\n\r");
	UARTPutString(UART_BASE, "Select a program to run:\n\r");
	UARTPutString(UART_BASE, "    ra = Run ADC\n\r");
    UARTPutString(UART_BASE, "    rp = Run PID\n\r");
    UARTPutString(UART_BASE, "    rs = Reset\n\r");
	UARTPutString(UART_BASE, "\nPress ENTER after typing your selection\n\n\r");

	while(1) {
		//Create an array for commands
		for (i = 0; i < 10; i++) {
			data[i] = '\0';
		}

		if(UARTCharsAvail(UART_BASE)) {
			charFromUART = UARTCharGet(UART_BASE);

			//Get command
			i = 0;
			while(charFromUART != '\r') {
				data[i] = charFromUART;
				charFromUART = UARTCharGet(UART_BASE);
				i++;

				if (i > 10) {
					UARTPutString(UART_BASE, "\rExceeded command length\n\r\r");
				}
			}

			//Execute command
			if(strcmp(data, "fw") == 0) {
				UARTPutString(UART_BASE, "Selection: go FORWARD\n\r");
				motorsFWD();
			}
			else if(strcmp(data, "bk") == 0) {
				UARTPutString(UART_BASE, "Selection: turn BACK\n\r");
				motorsBACK();
			}
			else if(strcmp(data, "tr") == 0) {
				UARTPutString(UART_BASE, "Selection: turn RIGHT\n\r");
				motorsRIGHT();
			}
			else if(strcmp(data, "tl") == 0) {
				UARTPutString(UART_BASE, "Selection: turn LEFT\n\r");
				motorsLEFT();
			}
			else if(strcmp(data, "su") == 0) {
				UARTPutString(UART_BASE, "Selection: speed UP\n\r");
				pwmNow += 20;
				motorsSPEED(pwmNow);
			}
			else if(strcmp(data, "sd") == 0) {
				UARTPutString(UART_BASE, "Selection: speed DOWN\n\r");
				pwmNow -= 20;
				motorsSPEED(pwmNow);
			}
			else if(strcmp(data, "off") == 0) {
				UARTPutString(UART_BASE, "Selection: motors OFF\n\r");
				motorsOFF();
			}
			else if(strcmp(data, "ra") == 0) {
				runADC();
			}
			else if(strcmp(data, "rp") == 0) {
				runPID();
			}
			else if(strcmp(data, "rs") == 0) {
				SysCtlReset();
			}
			else {
				UARTPutString(UART_BASE, "Unknown Command!!!\n\n\r");
			}
		}
	}
}


