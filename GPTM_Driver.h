#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#ifndef GPTM_Intialization_h
#define GPTM_Intialization_h

extern void CLK_Enable_GPTM_Module(int timer_number);                           //To Eable CLK for different timers
extern void Enable_Timer_Number(int timer_number,char type[10]);                //Enable or Disable different timers
extern void Timer_Number_Option(int timer_number,char option[10]);              //specify 16bit, 32bit, or RTC to different timers
extern void Timer_Number_Mode(int timer_number,char mode[10]);                  //specify one shot, periodic, or capture mode
extern void Timer_Number_Ticks(int timer_number,int ticks);                     //load number of ticks 
extern bool Timer_Number_Flag(int timer_number);
extern void Timer_Number_Clear(int timer_number);
extern void GPTM_Delay_Seconds(int timer_number,int seconds);                   //to create specific delay

#endif
