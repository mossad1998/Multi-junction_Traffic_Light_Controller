#include "GPTM_Driver.h"

volatile uint32_t* TIMERS_R[]={&TIMER0_CTL_R,&TIMER1_CTL_R,&TIMER2_CTL_R,                       //Array of CTL Registers for each timer
                               &TIMER3_CTL_R,&TIMER4_CTL_R,&TIMER5_CTL_R};

volatile uint32_t* TIMERS_CFG_R[]={&TIMER0_CFG_R,&TIMER1_CFG_R,&TIMER2_CFG_R,                   //Array of CFG Registers for each timer
                                   &TIMER3_CFG_R,&TIMER4_CFG_R,&TIMER5_CFG_R};

volatile uint32_t* TIMERS_TAMR_R[]={&TIMER0_TAMR_R,&TIMER1_TAMR_R,&TIMER2_TAMR_R,               //Array of TAM Registers for each timer
                                   &TIMER3_TAMR_R,&TIMER4_TAMR_R,&TIMER5_TAMR_R};

volatile uint32_t* TIMERS_Aload_R[]={&TIMER0_TAILR_R,&TIMER1_TAILR_R,&TIMER2_TAILR_R,           //Array of TAILR Registers for each timer
                                     &TIMER3_TAILR_R,&TIMER4_TAILR_R,&TIMER5_TAILR_R};

volatile uint32_t* TIMERS_Clear_R[]={&TIMER0_ICR_R,&TIMER1_ICR_R,&TIMER2_ICR_R,                 //Array of ICR Registers for each timer
                                     &TIMER3_ICR_R,&TIMER4_ICR_R,&TIMER5_ICR_R};

volatile uint32_t* TIMERS_Flag_R[]={&TIMER0_RIS_R,&TIMER1_RIS_R,&TIMER2_RIS_R,                  //Array of RIS Registers for each timer
                                     &TIMER3_RIS_R,&TIMER4_RIS_R,&TIMER5_RIS_R};



void CLK_Enable_GPTM_Module(int timer_number)                                                   //To Eable CLK for specific timer
  {
    SYSCTL_RCGCTIMER_R |= (1UL << timer_number);
  }

void Enable_Timer_Number(int timer_number,char type[10])                                        //Enable or Disable different timers
  {
    if(strcmp(type , "Enable")==0)
      {
      *TIMERS_R[timer_number] |= 1UL << 0;                                                      //To Enable Timer
      }
    else if (strcmp(type , "Disable")==0)
      {
      *TIMERS_R[timer_number] &= ~(1UL << 0);                                                   //To Disable Timer
      }
  }

void Timer_Number_Option(int timer_number,char option[10])                                      //specify 16bit, 32bit, or RTC to different timers
  {
    if((strcmp(option,"16"))==0)
      *TIMERS_CFG_R[timer_number] = 0x4;                                                        //16 bits timer
    else if((strcmp(option,"32"))==0)
      *TIMERS_CFG_R[timer_number] = 0x0;                                                        //32 bits timer
    else if ((strcmp(option,"RTC"))==0)                                                         
      *TIMERS_CFG_R[timer_number] = 0x1;                                                        //RTC timer
  }

void Timer_Number_Mode(int timer_number,char mode[10])                                          //specify one shot, periodic, or capture mode
  {
    if((strcmp(mode,"one_shot"))==0)
      *TIMERS_TAMR_R[timer_number] |= 0x1;                                                      //one shot mode                                                      
    else if((strcmp(mode,"periodic"))==0)
      *TIMERS_TAMR_R[timer_number] |= 0x2;                                                      //periodic mode
    else if((strcmp(mode,"capture"))==0)
      *TIMERS_TAMR_R[timer_number] |= 0x3;                                                      //capture mode
  }

void Timer_Number_Ticks(int timer_number,int ticks)                                             //load number of ticks 
  {
    *TIMERS_Aload_R[timer_number] = ticks-1;                                                    //number of ticks - 1
    *TIMERS_Clear_R[timer_number]= 1;                                                           //clear timer
  }
bool Timer_Number_Flag(int timer_number)
	{
		return((*TIMERS_Flag_R[timer_number] & 0x1));
	}
	void Timer_Number_Clear(int timer_number)
	{
		*TIMERS_Clear_R[timer_number]= 1;
	}
		
void GPTM_Delay_Seconds(int timer_number,int seconds)                                           //to create specific delay
  {    
    Enable_Timer_Number(1,"Enable");                                                            //Enable timer
    for(int i=0;i<seconds;i++)
    {
         while((*TIMERS_Flag_R[timer_number] & 0x1)==0);                                        //dealy of 1 second
         *TIMERS_Clear_R[timer_number]= 1;                                                      //clear timer
    }
 
  }







