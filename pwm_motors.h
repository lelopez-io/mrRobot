#ifndef _PWM_MOTORS_H_
#define _PWM_MOTORS_H_

void Configure_PWM();

void motorsOFF();
void motorsFWD();
void motorsRIGHT();
void motorsLEFT();
void motorsBACK();
void motorsSPEED(unsigned long pwmNow);

void delayMS(int ms);

#endif
