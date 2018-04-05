//----------------------------------------
// BIOS header files
//----------------------------------------
#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <xdc/runtime/Log.h>
#include <xdc/cfg/global.h>


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"


#include "adc.h"
#include "pwm_motors.h"
#include "navigate.h"
#include "uart_library.h"
#include "Board.h"

#define UART_BASE UART3_BASE // UART3_BASE can be switched to UART0_BASE for USB connection



/************[ Global Variables ]************************/
uint32_t startDistance;
uint32_t currentDistance;
int32_t prevError;
int32_t error;
int32_t pwm, P, I, D;
unsigned char* strPwm;

float Kp = 0.7;
float Ki = 0.01;
float Kd = 0.1;

//uint32_t UART_BASE = UART3_BASE;

void runADC() {
	// UART3_BASE can be switched to UART0_BASE for USB connection
	//uint32_t UART_BASE = UART3_BASE;

	uint32_t ui32ADC0Value[1]; // storing the data read from ADC FIFO (Front sensor)
	uint32_t ui32ADC1Value[1]; // storing the data read from ADC FIFO (Side sensor)

	while(1){
		//clear ADC interrupt flag
		ADCIntClear(ADC0_BASE, 3); //Front
		ADCIntClear(ADC0_BASE, 2); //Side

		//trigger ADC conversion
		ADCProcessorTrigger(ADC0_BASE, 3); //Front
		ADCProcessorTrigger(ADC0_BASE, 2); //Side

		//wait for the conversion to complete
		while(!ADCIntStatus(ADC0_BASE, 3, false)) {} //Front
		while(!ADCIntStatus(ADC0_BASE, 2, false)) {} //Side

		// read the ADC value from the ADC Sample Sequencer 1 FIFO
		ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value); //Front
		ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC1Value); //Side

		//Print distance reading from front facing sensor
		if (ui32ADC0Value[0] > 900 && ui32ADC0Value[0] <= 1500) {
			UARTPutString(UART_BASE, "\n900 < V < 1500\t\t");
		}
		else if (ui32ADC0Value[0] > 1500 && ui32ADC0Value[0] <= 2200) {
			UARTPutString(UART_BASE, "\n1500 < V < 2200\t\t");
		}
		else if (ui32ADC0Value[0] > 2200) {
			UARTPutString(UART_BASE, "\n2200 < V \t\t");
		}
		else {
			UARTPutString(UART_BASE, "FRONT\t\t\t");
		}


		//Print distance reading from side facing sensor
		if (ui32ADC1Value[0] > 900 && ui32ADC1Value[0] <= 1500) {
			UARTPutString(UART_BASE, "900 < V < 1500\n\n\r");
		}
		else if (ui32ADC1Value[0] > 1500 && ui32ADC1Value[0] <= 2200) {
			UARTPutString(UART_BASE, "1500 < V < 2200\n\n\r");
		}
		else if (ui32ADC1Value[0] > 2200) {
			UARTPutString(UART_BASE, "2200 < V \n\n\r");
		}
		else {
			UARTPutString(UART_BASE, "SIDE\n\n\r");
		}

	} //End of while


}

void runPID(void) {
	//Initialize variables for PID control
	//startDistance = getVALS_ADC();
	//prevError = 0;
	//I = 0;


	//Start the motors
	motorsFWD();

	BIOS_start();
}

void PIDcontrol(void) {
	//Read current distance (in ADC units)
	currentDistance = getVALS_ADC();
	//Compare it to initial distance from the wall
	error = startDistance - currentDistance;

	//Calculate PID variables
	P = Kp * error;
	I = error + prevError;
	D = Kd * error - prevError;

	pwm = P + (Ki * I) + D;


	if (pwm > 0) { //Too far from the wall
		//pwm = 255;
		itoa(pwm, strPwm);
		UARTPutString(UART_BASE, strPwm);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
		motorsADDR(pwm);
	}
	else if ( pwm < 0) {//Too close to the wall
		pwm = abs(pwm);
		itoa(pwm, strPwm);
		UARTPutString(UART_BASE, strPwm);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
		motorsADDL(pwm);
	}
	else {//Well centered
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
	}

	//Save current error as previous error for the next interrupt
	prevError = error;

}

unsigned char* itoa(int i, unsigned char b[]){
    char const digit[] = "0123456789";
    unsigned char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}


