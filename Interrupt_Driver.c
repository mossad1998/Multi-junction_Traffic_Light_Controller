#include "GPIO_Driver.h"

//PORT_Interrupt_Arrays
volatile uint32_t* PORT_IS_R[]={&GPIO_PORTA_IS_R,&GPIO_PORTB_IS_R,&GPIO_PORTC_IS_R,              //Array of IS Registers for each port
                              &GPIO_PORTD_IS_R,&GPIO_PORTE_IS_R,&GPIO_PORTF_IS_R};

volatile uint32_t* PORT_IBE_R[]={&GPIO_PORTA_IBE_R,&GPIO_PORTB_IBE_R,&GPIO_PORTC_IBE_R,          //Array of IBE Registers for each port
                              &GPIO_PORTD_IBE_R,&GPIO_PORTE_IBE_R,&GPIO_PORTF_IBE_R};

volatile uint32_t* PORT_IEV_R[]={&GPIO_PORTA_IEV_R,&GPIO_PORTB_IEV_R,&GPIO_PORTC_IEV_R,          //Array of IEV Registers for each port
                              &GPIO_PORTD_IEV_R,&GPIO_PORTE_IEV_R,&GPIO_PORTF_IEV_R};


volatile uint32_t* PORT_ICR_R[]={&GPIO_PORTA_ICR_R,&GPIO_PORTB_ICR_R,&GPIO_PORTC_ICR_R,          //Array of ICR Registers for each port
                              &GPIO_PORTD_ICR_R,&GPIO_PORTE_ICR_R,&GPIO_PORTF_ICR_R};

volatile uint32_t* PORT_IM_R[]={&GPIO_PORTA_IM_R,&GPIO_PORTB_IM_R,&GPIO_PORTC_IM_R,              //Array of IM Registers for each port
                              &GPIO_PORTD_IM_R,&GPIO_PORTE_IM_R,&GPIO_PORTF_IM_R};

volatile uint32_t* PORT_MIS_R[]={&GPIO_PORTA_MIS_R,&GPIO_PORTB_MIS_R,&GPIO_PORTC_MIS_R,          //Array of MIS Registers for each port
                              &GPIO_PORTD_MIS_R,&GPIO_PORTE_MIS_R,&GPIO_PORTF_MIS_R};

//UART_Interrupt_Arrays
volatile uint32_t* UART_IM_R[]={&UART0_IM_R,&UART1_IM_R,&UART2_IM_R,                             //Array of IM Registers for each module
                              &UART3_IM_R,&UART4_IM_R,&UART5_IM_R};

volatile uint32_t* UART_MIS_R[]={&UART0_MIS_R,&UART1_MIS_R,&UART2_MIS_R,                         //Array of MIS Registers for each module
                              &UART3_MIS_R,&UART4_MIS_R,&UART5_MIS_R};

volatile uint32_t* UART_ICR_R[]={&UART0_ICR_R,&UART1_ICR_R,&UART2_ICR_R,                         //Array of ICR Registers for each module
                              &UART3_ICR_R,&UART4_ICR_R,&UART5_ICR_R};

//GPTM_Interrupt_Arrays
volatile uint32_t* GPTM_ICR_R[]={&TIMER0_ICR_R,&TIMER1_ICR_R,&TIMER2_ICR_R,                      //Array of ICR Registers for each timer
                              &TIMER3_ICR_R,&TIMER4_ICR_R,&TIMER5_ICR_R};

volatile uint32_t* GPTM_MIS_R[]={&TIMER0_MIS_R,&TIMER1_MIS_R,&TIMER2_MIS_R,                      //Array of MIS Registers for each timer
                              &TIMER3_MIS_R,&TIMER4_MIS_R,&TIMER5_MIS_R};

volatile uint32_t* GPTM_IMR_R[]={&TIMER0_IMR_R,&TIMER1_IMR_R,&TIMER2_IMR_R,                      //Array of IMR Registers for each timer
                              &TIMER3_IMR_R,&TIMER4_IMR_R,&TIMER5_IMR_R};


//PORT_Interrupt_Functions
void PIN_Edge_Level_Trigger(char port_name,int pin,char type[15])                                //Specify edge sensitive or level sensitive
  {
         if(strcmp(type , "Edge_Sensitive")==0)
      {
        *PORT_IS_R[port_name-65] &= ~(1UL << pin);                                               //Specify edge sensitive
      }
         else if (strcmp(type , "Level_Sensitive")==0)
      {
        *PORT_IS_R[port_name-65] |= 1UL << pin;                                                  //Specify edge sensitive
      }
  }
void PIN_Both_Edge_Trigger(char port_name,int pin,char type[10])                                 //Enable or Disable both edge trigger
  {
      if(strcmp(type , "False")==0)
      {
        *PORT_IBE_R[port_name-65] &= ~(1UL << pin);                                              //To Disable both edge trigger
      }
         else if (strcmp(type , "True")==0)
      {
        *PORT_IBE_R[port_name-65] |= 1UL << pin;                                                 //To Enable both edge trigger
      }
  }
void PIN_Rising_Falling_Trigger(char port_name,int pin,char type[10])                            //Specify rising or falling trigger
  {
      if(strcmp(type , "Falling")==0)
      {
        *PORT_IEV_R[port_name-65] &= ~(1UL << pin);                                              //To Specify rising trigger
      }
         else if (strcmp(type , "Rising")==0)
      {
        *PORT_IEV_R[port_name-65] |= 1UL << pin;                                                 //To Specify rising trigger
      }
  }


void PIN_Clear_Interrupt(char port_name,int pin)                                                 //To clear switches interrupt flag
  {
      *PORT_ICR_R[port_name-65] |= 1UL << pin;
  }

void PIN_Unmask(char port_name,int pin)                                                          //To unmask switch pin
  {
    *PORT_IM_R[port_name-65] |= 1UL << pin;
  }

void Interrupt_Priority(volatile uint32_t * priority_R,int bits,int priority)                    //To specify priority of interrupt
  {
    *priority_R = priority << bits ;
  }
void Interrupt_Enable(volatile uint32_t * Interrupt_Enable_R,int bits)                           //To Enable interrupt
  {
    *Interrupt_Enable_R |= 1UL << bits;
  }

bool PIN_Interrupt_Flag(char port_name,int pin)                                                  //To read interrupt of switches
  {
   return (*PORT_MIS_R[port_name-65] & 1UL << pin);
  }


//UART_Interrupt_Functions
void UART_Unmask(int UART_module,char type[3])                                                   //To unmask uart pin
  {
     if(strcmp(type , "RX")==0)
      {
        *UART_IM_R[UART_module] |= 1UL << 4;                                                     //To unmask RX
      }
         else if (strcmp(type , "TX")==0)
      {
        *UART_IM_R[UART_module] |= 1UL << 5;                                                     //To unmask TX
      }
  }

bool UART_Interrupt_Flag(int UART_module,char type[3])                                          //To read interrupt of uart
  {
       if(strcmp(type , "RX")==0)
      {
          return (*UART_MIS_R[UART_module] & 1UL << 4);                                         //RX flag                                                
      }
         else if (strcmp(type , "TX")==0)
      {
          return (*UART_MIS_R[UART_module] & 1UL << 5);                                         //TX flag
      }
      return 0;
  }

void UART_Clear_Interrupt(int UART_module,char type[3])                                         //To clear uart interrupt flag
  {     volatile int readback;
      if(strcmp(type , "RX")==0)
      {
          *UART_ICR_R[UART_module] = 0x0010;                                                    //To clear RX
          readback = *UART_ICR_R[UART_module];
      }
      else if (strcmp(type , "TX")==0)
      {
          *UART_ICR_R[UART_module] = 0x0020;                                                    //To clear TX
          readback = *UART_ICR_R[UART_module];
      }
  }


//GPTM_Interrupt_Functions
void GPTM_Unmask(int timer_number,char type)                                                    //To unmask GPTM pin
{
     if(type == 'A')
      {
        *GPTM_IMR_R[timer_number] |= 1UL << 0;                                                  //To unmask  timer A
      }
     else if (type == 'B')
      {
        *GPTM_IMR_R[timer_number] |= 1UL << 8;                                                  //To unmask timer B
      }
  }

bool GPTM_Interrupt_Flag(int timer_number,char type)                                            //To read interrupt of GPTM
  {
      if(type == 'A')
      {
          return (*GPTM_MIS_R[timer_number] & 1UL << 0);                                        //Timer A flag 
      }
         else if (type == 'B')
      {
          return (*GPTM_MIS_R[timer_number] & 1UL << 8);                                        //Timer A flag
      }
      return 0;
  }

void GPTM_Clear_Interrupt(int timer_number,char type)                                           //To clear GPTM interrupt flag
  {
     volatile int readback;
      if(type == 'A')
      {
          *GPTM_ICR_R[timer_number] = 1UL << 0;                                                 //To clear Timer A
          readback = *GPTM_ICR_R[timer_number];
      }
      else if (type == 'B')
      {
          *GPTM_ICR_R[timer_number] = 1UL << 8;                                                 //To clear Timer B
          readback = *GPTM_ICR_R[timer_number];
      }
    
  }