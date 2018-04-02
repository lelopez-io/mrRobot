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
	ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);

	//configure the interrupt flag and sample the AIN2 pin (PE1)
	ADCSequenceStepConfigure(ADC0_BASE, 3, 0, (ADC_CTL_CH2|ADC_CTL_IE|ADC_CTL_END));
	ADCSequenceStepConfigure(ADC0_BASE, 2, 0, (ADC_CTL_CH1|ADC_CTL_IE|ADC_CTL_END));

	//enable ADC sequencer 3
	ADCSequenceEnable(ADC0_BASE, 3);
	ADCSequenceEnable(ADC0_BASE, 2);
}

uint32_t getVal_ADC() {

	return 0;
}
