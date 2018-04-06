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



void runPID(void) {
	//Start the motors
	motorsFWD();

	BIOS_start();
}



