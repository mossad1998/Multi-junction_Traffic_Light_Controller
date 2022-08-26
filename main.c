#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "GPIO_Driver.h"
#include "Interrupt_Driver.h"
#include "GPTM_Driver.h"

SemaphoreHandle_t xBinarySemaphore_1 ;
SemaphoreHandle_t xBinarySemaphore_2 ;
SemaphoreHandle_t xBinarySemaphore_3 ;

int North_South_Green_Time = 5000; 
int East_West_Green_Time = 2500;
int North_South_Red_Time = 2500;
int East_West_Red_Time = 5000;

void Ports_Initialization();																								//Initialization for port A and port B 
void Timer_Initialization();																								//Initialization for Timer A
void Micro_Clock(int period);																						//Micro Second Function

void Start_North_South();																								//Start North South Green Led for 5 Seconds
void Start_East_West();																									//Start East West Green Led for 2.5 Seconds

void Red_Flash ();																													//Blink all red Leds with frequency 1 second
void Turn_Off_Leds();																											//Turn off all leds
void Siren_On ();																														//Turn on Siren
void Siren_Off ();																														//Turn off siren

void Servo_Vertical ();																											//Gate is in the vertical position
void Servo_Horizontal ();																									//Gate is in the horizontal position
	
void Normal_Task_Cars(void *pvParameters);										//Normal Task Function
void Train_Task(void *pvParameters);																//Train Task Function
void West_East_Stop(void *pvParameters);											//Pedestrians West East Task Function
void North_South_Stop(void *pvParameters);										//Pedestrians North South Task Function


int main()
{	
			
	xBinarySemaphore_1 = xSemaphoreCreateBinary();																					//Train Semaphore
	xBinarySemaphore_2 = xSemaphoreCreateBinary();																					//Pedestrians West East Semaphore
	xBinarySemaphore_3 = xSemaphoreCreateBinary();																					//Pedestrians North South
	
	Ports_Initialization();																																											//Initialization for port A and port B 
	Timer_Initialization();																																											//Initialization for Timer A
	
	xTaskCreate(Normal_Task_Cars, "Task1", 100, NULL, 1, NULL);												//Create Normal Task
	xTaskCreate(West_East_Stop, "Task3", 100, NULL, 2, NULL);													//Create Pedestrians West East Task
	xTaskCreate(North_South_Stop, "Task4", 100, NULL, 2, NULL);												//Create Pedestrians North South Task
	xTaskCreate(Train_Task, "Task2", 100, NULL, 3, NULL);																		//Create Train Task
	
	vTaskStartScheduler();																																										// Start Schedule Tasks

	for (;;);
}

void Normal_Task_Cars(void *pvParameters)
{
	for (;;) 
			{
				Start_North_South();																																									//Start North South Green Leds
				vTaskDelay(North_South_Green_Time / portTICK_RATE_MS);												//Give 5 Seconds for North South Road
				Start_East_West();																																										//Start West East Green Leds
				vTaskDelay(East_West_Green_Time / portTICK_RATE_MS);												//Give 2.5 Seconds for West East Road
			}
}


void Train_Task(void *pvParameters)
{

	for (;;) 
			{		
				if (xSemaphoreTake(xBinarySemaphore_1, portMAX_DELAY) == pdPASS)						//Wait for Train Switchs Interrupts 
					{
							Turn_Off_Leds();																																													//Make sure all Leds are turned off
							Servo_Horizontal();																																												//Set Gate a Horizontal Position
							Siren_Off ();																																																	//Make sure Siren is Turned off
							for(int i=1;i<20000;i++)																																									//Wait for Safe Time of 20 Seconds
									{
											Micro_Clock(1000);																																							//Frequency of 1 Second
											if(i%1000==0)
												{
														Red_Flash ();																																								//Toggle Red Leds
														Siren_On ();																																									//Toggle Siren on and off
												}	
									}
						}	
			}
}

void West_East_Stop(void *pvParameters)
{
	for (;;) 
			{		
					if (xSemaphoreTake(xBinarySemaphore_2, portMAX_DELAY) == pdPASS)					//Wait for Pedestrians West East Switchs Interrupts 
						{
								Start_North_South();																																									// Turn On Red Leds for North South
								vTaskDelay(North_South_Red_Time / portTICK_RATE_MS);													//Give it its Duration 
						}
			}
}

void North_South_Stop(void *pvParameters)
{
	for (;;) 
			{		
				if (xSemaphoreTake(xBinarySemaphore_3, portMAX_DELAY) == pdPASS)							//Wait for Pedestrians North South Switchs Interrupts 
					{
							Start_East_West();																																												// Turn On Red Leds for East West
							vTaskDelay(East_West_Red_Time / portTICK_RATE_MS);																//Give it its Duration 
					}
			}
}

void GPIOB_Handler (void)
{
	if(PIN_Interrupt_Flag('B',2))																																		//To Know Which Switch is Pressed
		{
				PIN_Clear_Interrupt('B',2);																																//Clear Interrupt Flag
				BaseType_t contextRequst;																														//Variable that will be updated by the semaphore API 
				contextRequst=pdFALSE;																															//Initialize it to false which means no context required now 
				xSemaphoreGiveFromISR(xBinarySemaphore_1,&contextRequst);		//Give semaphore by incrementing it to indicate ISR occured 
				portYIELD_FROM_ISR(contextRequst);																						//If higher priority task is unblocked due to giving the semaphore
		}
	if(PIN_Interrupt_Flag('B',3)||PIN_Interrupt_Flag('B',4))
		{
				PIN_Clear_Interrupt('B',3);																																		//Clear Interrupt Flag
				PIN_Clear_Interrupt('B',4);																																		//Clear Interrupt Flag
				BaseType_t contextRequst;																																//Variable that will be updated by the semaphore API 
				contextRequst=pdFALSE;																																	//Initialize it to false which means no context required now 		
				xSemaphoreGiveFromISR(xBinarySemaphore_2,&contextRequst);				//Give semaphore by incrementing it to indicate ISR occured 
				portYIELD_FROM_ISR(contextRequst);																								//If higher priority task is unblocked due to giving the semaphore
		}
	if(PIN_Interrupt_Flag('B',6)||PIN_Interrupt_Flag('B',7))
		{
				PIN_Clear_Interrupt('B',6);																																		//Clear Interrupt Flag
				PIN_Clear_Interrupt('B',7);																																		//Clear Interrupt Flag
				BaseType_t contextRequst;																																//Variable that will be updated by the semaphore API 
				contextRequst=pdFALSE;																																	//Initialize it to false which means no context required now 
				xSemaphoreGiveFromISR(xBinarySemaphore_3,&contextRequst);				//Give semaphore by incrementing it to indicate ISR occured 
				portYIELD_FROM_ISR(contextRequst);																								//If higher priority task is unblocked due to giving the semaphore
		}
}

void Ports_Initialization(void)
{																										
			CLK_Enable('A');																																			//Enable Port A
			CLK_Enable('B');																																			//Enable Port B
	
      PIN_ANALOG_DIGITAL('B',2,"Digital");                                      //Enable Digital For Pin B2
      PIN_ANALOG_DIGITAL('B',3,"Digital");                                      //Enable Digital For Pin B3
	    PIN_ANALOG_DIGITAL('B',4,"Digital");                                      //Enable Digital For Pin B4
      PIN_ANALOG_DIGITAL('B',5,"Digital");                                      //Enable Digital For Pin B5
	    PIN_ANALOG_DIGITAL('B',6,"Digital");                                      //Enable Digital For Pin B6
      PIN_ANALOG_DIGITAL('B',7,"Digital");                                      //Enable Digital For Pin B7
	
      PIN_FUNCTION_SELECT('B',2,"General I/O");                            //To specifiy pin B2 as General I/O
	    PIN_FUNCTION_SELECT('B',3,"General I/O");                            //To specifiy pin B3 as General I/O
	    PIN_FUNCTION_SELECT('B',4,"General I/O");                            //To specifiy pin B4 as General I/O
	    PIN_FUNCTION_SELECT('B',5,"General I/O");                            //To specifiy pin B5 as General I/O
	    PIN_FUNCTION_SELECT('B',6,"General I/O");                            //To specifiy pin B6 as General I/O
	    PIN_FUNCTION_SELECT('B',7,"General I/O");                            //To specifiy pin B7 as General I/O
	
      PIN_INPUT_OUTPUT('B',2,"Input");                                     	  	  //To specifiy pin B2 as Input
      PIN_INPUT_OUTPUT('B',3,"Input");                                      	    //To specifiy pin B3 as Input
	    PIN_INPUT_OUTPUT('B',4,"Input");                                     	  	  //To specifiy pin B4 as Input
      PIN_INPUT_OUTPUT('B',5,"Input");                                     	      //To specifiy pin B5 as Input
	    PIN_INPUT_OUTPUT('B',6,"Input");                                        	  //To specifiy pin B6 as Input
      PIN_INPUT_OUTPUT('B',7,"Input");                                     	      //To specifiy pin B7 as Input
			
      PIN_PULLUP_RESISTOR('B',2,"Pullup");                                   //To specifiy pin B2 as pullup
      PIN_PULLUP_RESISTOR('B',3,"Pullup");                                   //To specifiy pin B3 as pullup
      PIN_PULLUP_RESISTOR('B',4,"Pullup");                                   //To specifiy pin B4 as pullup
      PIN_PULLUP_RESISTOR('B',5,"Pullup");                                   //To specifiy pin B5 as pullup
      PIN_PULLUP_RESISTOR('B',6,"Pullup");                                   //To specifiy pin B6 as pullup
      PIN_PULLUP_RESISTOR('B',7,"Pullup");                                   //To specifiy pin B7 as pullup

      PIN_Edge_Level_Trigger('B',2,"Edge_Sensitive");                        //Specify level sensitive for pin B2
      PIN_Edge_Level_Trigger('B',3,"Edge_Sensitive");                        //Specify level sensitive for pin B3
      PIN_Edge_Level_Trigger('B',4,"Edge_Sensitive");                        //Specify level sensitive for pin B4
      PIN_Edge_Level_Trigger('B',5,"Edge_Sensitive");                        //Specify level sensitive for pin B5
      PIN_Edge_Level_Trigger('B',6,"Edge_Sensitive");                        //Specify level sensitive for pin B6
      PIN_Edge_Level_Trigger('B',7,"Edge_Sensitive");                        //Specify level sensitive for pin B7

      PIN_Both_Edge_Trigger('B',2,"False");                                   	  //Disable both edge trigger for pin B2
      PIN_Both_Edge_Trigger('B',3,"False");                                    	  //Disable both edge trigger for pin B3
      PIN_Both_Edge_Trigger('B',4,"False");                                   	  //Disable both edge trigger for pin B4
      PIN_Both_Edge_Trigger('B',5,"False");                                   	  //Disable both edge trigger for pin B5
      PIN_Both_Edge_Trigger('B',6,"False");                                    	  //Disable both edge trigger for pin B6
      PIN_Both_Edge_Trigger('B',7,"False");                                  	    //Disable both edge trigger for pin B7

      PIN_Rising_Falling_Trigger('B',2,"Falling");                        		      //Specify falling trigger for pin B2
      PIN_Rising_Falling_Trigger('B',3,"Falling");                             	  //Specify falling trigger for pin B3
      PIN_Rising_Falling_Trigger('B',4,"Falling");                           	    //Specify falling trigger for pin B4
      PIN_Rising_Falling_Trigger('B',5,"Falling");                             	  //Specify falling trigger for pin B5
      PIN_Rising_Falling_Trigger('B',6,"Falling");                             	  //Specify falling trigger for pin B6
      PIN_Rising_Falling_Trigger('B',7,"Falling");                             	  //Specify falling trigger for pin B7

      PIN_Clear_Interrupt('B',2);                                          					      //To clear pin B2 interrupt flag
      PIN_Clear_Interrupt('B',3);                                            					    //To clear pin B3 interrupt flag
      PIN_Clear_Interrupt('B',4);                                             					  //To clear pin B4 interrupt flag
      PIN_Clear_Interrupt('B',5);                                             					  //To clear pin B5 interrupt flag
      PIN_Clear_Interrupt('B',6);                                               					//To clear pin B6 interrupt flag
      PIN_Clear_Interrupt('B',7);                                            					    //To clear pin B7 interrupt flag

      PIN_Unmask('B',2);																																	//To unmask pin B2
			PIN_Unmask('B',3);																																	//To unmask pin B3
      PIN_Unmask('B',4);																																	//To unmask pin B4
			PIN_Unmask('B',5);																																	//To unmask pin B5
      PIN_Unmask('B',6);																																	//To unmask pin B6
			PIN_Unmask('B',7);																																	//To unmask pin B7

			NVIC_SetPriority(1,5);																															//Set Priority For Port B Handler for 5
			NVIC_EnableIRQ(1);																																//Enable Interrupt from Port B
	
			PIN_ANALOG_DIGITAL('A',2,"Digital");																			//Enable Digital For Pin A2
			PIN_ANALOG_DIGITAL('A',3,"Digital");																			//Enable Digital For Pin A3
			PIN_ANALOG_DIGITAL('A',4,"Digital");																			//Enable Digital For Pin A4
			PIN_ANALOG_DIGITAL('A',5,"Digital");																			//Enable Digital For Pin A5
			PIN_ANALOG_DIGITAL('A',6,"Digital");																			//Enable Digital For Pin A6
			PIN_ANALOG_DIGITAL('A',7,"Digital");																			//Enable Digital For Pin A7
			
			PIN_FUNCTION_SELECT('A',2,"General I/O");														//To specifiy pin A2 as General I/O
			PIN_FUNCTION_SELECT('A',3,"General I/O");														//To specifiy pin A3 as General I/O
			PIN_FUNCTION_SELECT('A',4,"General I/O");														//To specifiy pin A4 as General I/O
			PIN_FUNCTION_SELECT('A',5,"General I/O");														//To specifiy pin A5 as General I/O
			PIN_FUNCTION_SELECT('A',6,"General I/O");														//To specifiy pin A6 as General I/O
			PIN_FUNCTION_SELECT('A',7,"General I/O");														//To specifiy pin A7 as General I/O
			
			PIN_INPUT_OUTPUT('A',2,"Output");																					//To specifiy pin A2 as Outut
			PIN_INPUT_OUTPUT('A',3,"Output");																					//To specifiy pin A3 as Outut
			PIN_INPUT_OUTPUT('A',4,"Output");																					//To specifiy pin A4 as Outut	
			PIN_INPUT_OUTPUT('A',5,"Output");																					//To specifiy pin A5 as Outut
			PIN_INPUT_OUTPUT('A',6,"Output");																					//To specifiy pin A6 as Outut
			PIN_INPUT_OUTPUT('A',7,"Output");																					//To specifiy pin A7 as Outut
		}


void Timer_Initialization()
{
		CLK_Enable_GPTM_Module(1); 																												//Enable Clock For Timer A
		Enable_Timer_Number(1,"Disable");																						  		//Disable Timer A
		Timer_Number_Option(1,"16");																													//16 bit Timer
		Timer_Number_Mode(1,"periodic");																										//Periodic Timer
		Timer_Number_Ticks(1,16);																															//16 Tick
		Enable_Timer_Number(1,"Enable");																								  	//Enable Timer A
}

void Micro_Clock(int period)
{
	int i;
		for(i = 0; i < period; i++)
    {
        while (Timer_Number_Flag(1) == 0) ;  																		        //Wait for TimerA timeout flag
        Timer_Number_Clear(1);    																													  //Clear the TimerA timeout flag */
    }

}

void Start_North_South()
{
	//A7 Green_North_South
	//A6 Red_East_West
		Turn_Off_Leds();																																						//Turn Off All Leds
		Siren_Off ();																																										//Turn Off Siren
		PIN_WRITE_DIGITAL('A',7,"HIGH");																							 		//Green Led North South On
		PIN_WRITE_DIGITAL('A',6,"HIGH");																									//Red Led East West On
		Servo_Vertical ();																																						//Servo in Vertical Position
}

void Start_East_West()
{
	//A5 Green_East_West
	//A4 Red_North_South
		Turn_Off_Leds();																																						//Turn Off All Leds
		Siren_Off ();																																										//Turn Off Siren
		PIN_WRITE_DIGITAL('A',5,"HIGH");																									//Green Led East West On
		PIN_WRITE_DIGITAL('A',4,"HIGH");																									//Red Led North South On
		Servo_Vertical ();																																						//Servo in Vertical Position
}

void Turn_Off_Leds()
{
	PIN_WRITE_DIGITAL('A',7,"LOW");																										//Green Led North South Off
	PIN_WRITE_DIGITAL('A',6,"LOW");																										//Red Led East West Off
	PIN_WRITE_DIGITAL('A',5,"LOW");																										//Green Led East West Off
	PIN_WRITE_DIGITAL('A',4,"LOW");																										//Red Led North South Off
}
void Siren_On ()
{
	//A3 siren
		static bool toggle = 0;
	if(toggle == 0)
		PIN_WRITE_DIGITAL('A',3,"LOW");																								//Turn On Siren
	else if(toggle == 1)				
		PIN_WRITE_DIGITAL('A',3,"HIGH");																								//Turn Off Siren
	toggle = (!toggle);																																						//Toggle

}

void Siren_Off ()
{
		PIN_WRITE_DIGITAL('A',3,"LOW");																								//Turn Off Siren
}
	
void Red_Flash ()
{
	static bool toggle = 0;
	if(toggle == 0)
	{
		PIN_WRITE_DIGITAL('A',7,"LOW");																								//Red Led East West Off
		PIN_WRITE_DIGITAL('A',5,"LOW");																								//Red Led North South Off
	}
	else if(toggle == 1)
	{
		PIN_WRITE_DIGITAL('A',7,"HIGH");																								//Red Led East West On
		PIN_WRITE_DIGITAL('A',5,"HIGH");																								//Red Led North South On
	}
	toggle = (!toggle);																																						//Toggle

}

void Servo_Vertical ()
{
   int i=0; 
	 for(i=0; i<50; i++) 
    {	
																																																				 //Given 7% duty cycle trigger pulse
	  PIN_WRITE_DIGITAL('A',2,"HIGH");																									 //Make control  pin high
	  Micro_Clock(1600); 																																				 //1.4ms seconds delay
	  PIN_WRITE_DIGITAL('A',2,"LOW");																									 //Make control  pin low
	  Micro_Clock(18400); 																																		 	//1.86ms seconds delay
		}
	
}
void Servo_Horizontal ()
{

   int i=0; 
	 for(i=0; i<50; i++) 
    {	
																																																			//Given 7% duty cycle trigger pulse
	  PIN_WRITE_DIGITAL('A',2,"HIGH"); 																							//Make control  pin high
	  Micro_Clock(600); 																																				//1.4ms seconds delay
	  PIN_WRITE_DIGITAL('A',2,"LOW");																								//Make control  pin low
	  Micro_Clock(19400);																																 	 //1.86ms seconds delay
		}
}
	

	