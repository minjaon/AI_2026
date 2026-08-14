#include "device_driver.h"
#include <stdio.h>
unsigned int func(unsigned int x);

static void Sys_Init(int baud) 
{
    SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
    Clock_Init();
    Uart2_Init(baud);
    setvbuf(stdout, NULL, _IONBF, 0);
    LED_Init();
	Key_Poll_Init();
}


void Main(void)
{
	Sys_Init(115200);
	printf("LED 0.5");

	for(;;)
	{
		Macro_Invert_Bit(GPIOA->ODR, 5); 
		TIM2_Delay(100);
	}

}
