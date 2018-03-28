  #include "C:\Keil\Labware\inc\tm4c123gh6pm.h"
	#include "stdint.h"
	
	void SystemInit(){}
		
  //________________SYSTIC TIMER_____________________________
  void SysTick_Init(void){
    NVIC_ST_CTRL_R=0;
    NVIC_ST_RELOAD_R=79; // 80 count -> 1 us 
    NVIC_ST_CURRENT_R=0;
    NVIC_ST_CTRL_R=0x07; // with interrupt
		
    }
  

  
    
  //__________________PORTF_______________________
    
	
  #define PF4                     (*((volatile unsigned long *)0x40025040))
  #define PF3                     (*((volatile unsigned long *)0x40025020))
  #define PF2                     (*((volatile unsigned long *)0x40025010))
  #define PF1                     (*((volatile unsigned long *)0x40025008))
  #define PF0                     (*((volatile unsigned long *)0x40025004))
  #define GPIO_LOCK_KEY           0x4C4F434B  // Unlocks the GPIO_CR register
  	
		
  void PortF_Init(void){ volatile unsigned long delay;
    SYSCTL_RCGC2_R |= 0x00000020;     // 1) activate clock for Port F
    delay = SYSCTL_RCGC2_R;           // allow time for clock to start
    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
    GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
    // only PF0 needs to be unlocked, other bits can't be locked
    GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
    GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
    GPIO_PORTF_DIR_R = 0xFF;          // all ports are output
    GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
    //GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
    GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0
  }
  
  
  //_________INTERRUPT HANDLER______
  uint32_t microsec=0;
  
  void SysTick_Handler(void){
    microsec++;
		
		if(microsec>=1000000) {
				
				PF2^=0x04;
				microsec = 0;
				
			}
			
			
		
    }
  
	
	int main(void){
	  SysTick_Init();
    PortF_Init();  
		__enable_irq(); //global enable interrupt

		while(1){
			
			/*
			if( (NVIC_ST_CTRL_R & 0x00010000) == 0x00010000)
			{
				microsec++;
				NVIC_ST_CURRENT_R=0; //reset Timer
			}				
			
			*/
			
		}
		
	return 0;
	}
	
