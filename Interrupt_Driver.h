#include <stdint.h>
#include <stdbool.h>





#ifndef __Interrupt_Driver_H__
#define __Interrupt_Driver_H__




//PORT_Interrupt_Functions
extern void PIN_Edge_Level_Trigger(char port_name,int pin,char type[15]);       //Specify edge sensitive or level sensitive
extern void PIN_Both_Edge_Trigger(char port_name,int pin,char type[10]);        //Enable or Disable both edge trigger
extern void PIN_Rising_Falling_Trigger(char port_name,int pin,char type[10]);   //Specify rising or falling trigger
extern void PIN_Clear_Interrupt(char port_name,int pin);                        //To clear switches interrupt flag
extern void PIN_Unmask(char port_name,int pin);                                 //To unmask switch pin
extern void Interrupt_Priority(volatile uint32_t * priority_R,int bits,int priority);//To specify priority of interrupt
extern void Interrupt_Enable(volatile uint32_t * Interrupt_Enable_R,int bits);  //To Enable interrupt
extern bool PIN_Interrupt_Flag(char port_name,int pin);                         //To read interrupt of switches
//UART_Interrupt_Functions
extern void UART_Unmask(int UART_module,char type[3]);                          //To unmask uart pin
extern bool UART_Interrupt_Flag(int UART_module,char type[3]);                  //To read interrupt of uart
extern void UART_Clear_Interrupt(int UART_module,char type[3]);                 //To clear uart interrupt flag
//GPTM_Interrupt_Functions
extern void GPTM_Unmask(int timer_number,char type);                            //To unmask GPTM pin
extern bool GPTM_Interrupt_Flag(int timer_number,char type);                    //To read interrupt of GPTM
extern void GPTM_Clear_Interrupt(int timer_number,char type);                   //To clear GPTM interrupt flag


#endif