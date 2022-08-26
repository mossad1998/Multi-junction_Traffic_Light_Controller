#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <string.h>


#ifndef __GPIO_Driver_H__
#define __GPIO_Driver_H__

//define of clk of each port
#define CLK_PORTA 	(0x01)
#define CLK_PORTB 	(0x02)
#define CLK_PORTC 	(0x04)
#define CLK_PORTD 	(0x08)
#define CLK_PORTE 	(0x10)
#define CLK_PORTF 	(0x20)

extern void CLK_Enable(char port_name);                                         //Clock for Port
extern void GPIO_PORT_LOCK(char port_name);                                     //Unlock for port
extern void PIN_ANALOG_DIGITAL(char port_name,int pin,char type[10]);           //Enable or Disable Analog and Digital
extern void PIN_FUNCTION_SELECT(char port_name,int pin,char type[15]);          //General i/o or Alternative Function
extern void PIN_INPUT_OUTPUT(char port_name,int pin,char type[10]);             //Input or Output
extern void PIN_PULLUP_RESISTOR(char port_name,int pin,char type[10]);          //Pullup or Pulldown
extern void PIN_CONFIGURE(char port_name,int pin,char type[10]);                //Specify the configuration
extern void PIN_WRITE_DIGITAL(char port_name,int pin,char data[3]);             //To Write Data
extern bool PIN_READ_DIGITAL(char port_name,int pin);                           //To Read Data

#endif