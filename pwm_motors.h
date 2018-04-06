#ifndef _PWM_MOTORS_H_
#define _PWM_MOTORS_H_

void Configure_PWM();


void motorsFWD();
void motorsBACK();
void motorsRIGHT();
void motorsLEFT();
void motorsOFF();
void motorsSPEED(unsigned long pwmNow);
void motorsADDR(int32_t pwm);
void motorsADDL(int32_t pwm);


#endif
