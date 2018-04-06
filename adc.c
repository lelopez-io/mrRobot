#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"

#include "adc.h"
#include "uart_library.h"

#define UART_BASE UART3_BASE // UART3_BASE can be switched to UART0_BASE for USB connection

uint32_t ui32ADC0Value[1]; // storing the data read from ADC FIFO (Front sensor)
uint32_t ui32ADC1Value[1]; // storing the data read from ADC FIFO (Side sensor)

void Configure_ADC(){
	//enable ADC0 peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {}

	//enable ADC0 sample, sequencer 3
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); //Front facing sensor
	ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0); //Side facing sensor

	//configure the interrupt flag and sample the AIN2 pin (PE1) and AIN3 pin (PE2)
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, (ADC_CTL_CH2|ADC_CTL_IE|ADC_CTL_END)); //Front
	ADCSequenceStepConfigure(ADC0_BASE, 2, 0, (ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END)); //Side

	//enable ADC sequencer 3 and 2
	ADCSequenceEnable(ADC0_BASE, 3); //Front
	ADCSequenceEnable(ADC0_BASE, 2); //Side
}

void runADC() {
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

uint32_t getVALF_ADC() {
	//clear ADC interrupt flag
	ADCIntClear(ADC0_BASE, 3); //Front

	//trigger ADC conversion
	ADCProcessorTrigger(ADC0_BASE, 3); //Front

	//wait for the conversion to complete
	while(!ADCIntStatus(ADC0_BASE, 3, false)) {} //Front

	// read the ADC value from the ADC Sample Sequencer 1 FIFO
	ADCSequenceDataGet(ADC0_BASE, 3, ui32ADC0Value); //Front

	return ui32ADC0Value[0];
}

uint32_t getVALS_ADC() {
	//clear ADC interrupt flag
	ADCIntClear(ADC0_BASE, 2); //Side

	//trigger ADC conversion
	ADCProcessorTrigger(ADC0_BASE, 2); //Side

	//wait for the conversion to complete
	while(!ADCIntStatus(ADC0_BASE, 2, false)) {} //Side

	// read the ADC value from the ADC Sample Sequencer 1 FIFO
	ADCSequenceDataGet(ADC0_BASE, 2, ui32ADC1Value); //Side

	return ui32ADC1Value[0];
}
