#include <stdint.h>
#include "C:\Keil\EE319Kware\inc\tm4c123gh6pm.h"


//defines

#define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register
#define     PF7     (*((volatile uint32_t *)0x40025200))
#define     PF6     (*((volatile uint32_t *)0x40025100))
#define     PF5     (*((volatile uint32_t *)0x40025080))
#define     PF4     (*((volatile uint32_t *)0x40025040))
#define     PF3     (*((volatile uint32_t *)0x40025020))
#define     PF2     (*((volatile uint32_t *)0x40025010))
#define     PF1     (*((volatile uint32_t *)0x40025008))
#define     PF0     (*((volatile uint32_t *)0x40025004))
#define     PA7     (*((volatile uint32_t *)0x40004200))
#define     PA6     (*((volatile uint32_t *)0x40004100))
#define     PA5     (*((volatile uint32_t *)0x40004080))
#define     PA4     (*((volatile uint32_t *)0x40004040))
#define     PA3     (*((volatile uint32_t *)0x40004020))
#define     PA2     (*((volatile uint32_t *)0x40004010))
#define     PA1     (*((volatile uint32_t *)0x40004008))
#define     PA0     (*((volatile uint32_t *)0x40004004))
#define     PE7     (*((volatile uint32_t *)0x40024200))
#define     PE6     (*((volatile uint32_t *)0x40024100))
#define     PE5     (*((volatile uint32_t *)0x40024080))
#define     PE4     (*((volatile uint32_t *)0x40024040))
#define     PE3     (*((volatile uint32_t *)0x40024020))
#define     PE2     (*((volatile uint32_t *)0x40024010))
#define     PE1     (*((volatile uint32_t *)0x40024008))
#define     PE0     (*((volatile uint32_t *)0x40024004))

//global variables

volatile int microsec_10=0;
volatile int echo = 0;
volatile int time = 0;
volatile int distance = 0;

void SystemInit(){}


//peripherals Init


	//________________SYSTIC TIMER_____________________________
  void SysTick_Init(void){
    NVIC_ST_CTRL_R=0;
    NVIC_ST_RELOAD_R=160; // 160 count -> 10 us
    NVIC_ST_CTRL_R=0;
		NVIC_ST_CURRENT_R=0; // reset_timer
    }



	//________________PORTS_____________________________
void PORTS_Init(void)
{

  //GPIO
  uint32_t delay;
  SYSCTL_RCGCGPIO_R |= 0x3F; // Clock to all ports enabled
  delay = 1; // dummy var for delay

  //PORTF____________________
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;   // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  GPIO_PORTF_AFSEL_R = 0; // Alternative Function
  GPIO_PORTF_PCTL_R = 0; // Port Control
  GPIO_PORTF_AMSEL_R = 0; // Analog disable
  GPIO_PORTF_DIR_R = 0x0F; // F4 input (0) - F0 -> F3 output(1)
  GPIO_PORTF_DEN_R = 0x1F ; // Digital enable
  GPIO_PORTF_PUR_R = 0x10; // pull up the input F4


  //PORTA_____________________
  GPIO_PORTA_AMSEL_R  = 0;   //  disable analog function
  GPIO_PORTA_PCTL_R   = 0;   // GPIO clear bit PCTL
  GPIO_PORTA_AFSEL_R  = 0;   // Alternative Function
  GPIO_PORTA_DIR_R    = 0x3F; //  PA2, PA2, PA4, PA5->output(1) ,PA6,PA7 ->input(0)
  GPIO_PORTA_DEN_R    = 0xFF; // enable digital pins PA2, PA3, PA4, PA5 ,PA6,PA7


  //PORTE__________________________
  GPIO_PORTE_AMSEL_R = 0;    //  disable analog function
  GPIO_PORTE_PCTL_R  = 0;    //  GPIO clear bit PCTL
  GPIO_PORTE_DIR_R   = 0xFF; //  E0-E7 output
  GPIO_PORTE_AFSEL_R = 0;   //  no alternate function
  GPIO_PORTE_DEN_R   = 0xFF; // E0-E7 digital


}




//ISR
 void SysTick_Handler(void)
 {
    microsec_10++;

	 if(echo)
		 time++;

 }




//algorithm functions

void delay(int number) // 10 us
		{
				 NVIC_ST_CTRL_R=0x07; // with interrupt
			   NVIC_ST_CURRENT_R=0; // reset_timer
				 while( microsec_10 <= number );
			microsec_10=0;
			NVIC_ST_CTRL_R=0;
		}


 void display(int number)
  {
  int hundreds=0, tens=0,ones=0;

  if(number>99){
    hundreds =number/100;
    tens =(number - hundreds * 100) / 10;
    ones = (number - hundreds * 100 - tens * 10);
    }
  else if (number<=99){
    tens =number/10;
    ones = number%10;
    }
  else if(number<=9){
    ones=number;
    }

		hundreds=	hundreds << 1;
		tens= tens << 2;

		GPIO_PORTE_DATA_R = hundreds; // --------> most significant
		GPIO_PORTA_DATA_R = tens; //    --------> middle
		GPIO_PORTF_DATA_R = ones; //      --------> least significant

  }


int main()
{

	//PE5 output for Trigger
  //PA6 input for Echo

	PORTS_Init();

	SysTick_Init();

	__enable_irq(); //global enable interrupt



	while(1){

		PE5 = 0;
		delay(2);

		//Set the Trigger
		PE5 = 32;
		delay(1); // 10 us
		PE5 = 0;

		while(!(PA6)); // wait for Echo to rise

		echo = 1;
		NVIC_ST_CTRL_R=0x07; // enable timer with interrupt
		NVIC_ST_CURRENT_R=0; // reset_timer

		while(PA6); // wait for Echo to goes Low

			NVIC_ST_CTRL_R=0; //disable timer
			//measure distance
			// V (speed of sound) = 340 m/s  = .34 cm/10us
			// distance = (.34 * time ) / 2 = .17*time
			distance = .17 * time ;
			distance++;
			distance += distance * .02; // error correction due some measurements :'D
			time=0;
			echo = 0;

		if(distance > 999)
			distance = 999;


		display(distance);

		delay(50000); // delay .5 second


			}


return 0;

}


