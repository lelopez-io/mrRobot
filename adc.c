#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"
#include "adc.h"

uint32_t ui32ADC0Value[1]; // storing the data read from ADC FIFO
uint32_t ui32ADC1Value[1];

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
