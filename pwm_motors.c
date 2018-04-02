#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "pwm_motors.h"

void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

void Configure_PWM(){

	// PWM settings
	uint32_t period = 320;
	uint32_t duty = 150;

	uint32_t PWM_FREQUENCY = 400;
	uint32_t ui8Adjust = 440;
	uint32_t ui32PWMClock = SysCtlClockGet() / 64;
	uint32_t ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;

	//period = (ui32PWMClock / PWM_FREQUENCY) - 1;
	//duty = (ui8Adjust * ui32Load) / 1000;

	//Configure PWM Clock to match system
	SysCtlPWMClockSet(SYSCTL_SYSDIV_5);

	//Enable peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	// Configure PD0, PD1 Pins as PWM
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);


	// Configure PWM Options
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

	//Set the Period (expressed in clock ticks)
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, period);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, period);

	// Set PWM duty
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, duty);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, duty);

	// Enable the PWM generator
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);
	PWMGenEnable(PWM1_BASE, PWM_GEN_1);

	// Turn on the output pins
	//PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT |PWM_OUT_1_BIT, true);
}

void motorsOFF(){
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT , false);
}

void motorsFWD(){
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, false);

	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1, true);
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);
}

void motorsBACK(){
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, true);

	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0 | GPIO_PIN_1, true);
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);
}

void motorsRIGHT(){
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, false);
	delayMS(2000);
	PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
}

void motorsLEFT(){
	PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, false);
	delayMS(2000);
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
}



void motorsSPEED(unsigned long pwmNow){
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, pwmNow);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, pwmNow);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_4, pwmNow);
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, pwmNow);
}
