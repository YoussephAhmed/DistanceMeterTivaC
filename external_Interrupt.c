#include <stdint.h>
#include "C:\Keil\EE319Kware\inc\tm4c123gh6pm.h"


  #define PF4                     (*((volatile unsigned long *)0x40025040))
  #define PF3                     (*((volatile unsigned long *)0x40025020))
  #define PF2                     (*((volatile unsigned long *)0x40025010))
  #define PF1                     (*((volatile unsigned long *)0x40025008))
  #define PF0                     (*((volatile unsigned long *)0x40025004))
  #define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register



void SystemInit(){}


void init(void)
{

	//GPIO
	uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x20; // Clock to portF
	delay = 1; // dummy var for delay
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	GPIO_PORTF_AFSEL_R = 0; // Alternative Function
	GPIO_PORTF_PCTL_R = 0; // Port Control
	GPIO_PORTF_AMSEL_R = 0; // Analog disable
	GPIO_PORTF_DIR_R = 0x0E; // F0,F4 input(0) - F1 -> F3 output(1)
	GPIO_PORTF_DEN_R = 0x1F ; // Digital enable
	GPIO_PORTF_PUR_R = 0x11; // pull up the input

	//Interrupt
	GPIO_PORTF_IS_R &= ~0x10; // PF4 is edge sensitive
	GPIO_PORTF_IBE_R &= ~0x10; // PF4 is single egde
	GPIO_PORTF_IEV_R &= ~0x10; // PF4 falling edge
	GPIO_PORTF_ICR_R = 0x10; //clear PF4 flag ---------->>>>>>
	GPIO_PORTF_IM_R = 0x10; // unmask PF4 for interrupt

	NVIC_EN0_R = 0x40000000 ; // enable interrupt 30 in NVIC



}


//GPIO Interrupt
void GPIOPortF_Handler()
{
		GPIO_PORTF_ICR_R = 0x10; //clear PF4 flag ---------->>>>>>
		PF2 ^= 0x04;
}


int main()
{

	init();
	__enable_irq(); //global enable interrupt

	while(1)
	{

		/*
		if    ( (GPIO_PORTF_DATA_R & 0x11) == 0x10 ) // Sw2 is on (active low)
		{
				PF1 ^= 0x02;
		}

		*/
	}

}
