/*
 * uart_library.h
 *
 *  Created on: Feb 22, 2018
 *      Author: lelopez
 */

#ifndef UART_LIBRARY_H_
#define UART_LIBRARY_H_
/////////////////////

void Configure_UART0();
void Configure_UART3();
void UARTPutString(uint32_t UART_BASE, unsigned char *stringToSend);
void UARTGetString(uint32_t UART_BASE, unsigned char *stringFromUART, unsigned long ulCount);
////////////////////
#endif /* UART_LIBRARY_H_ */
