#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
	Key_Poll_Init();
}

volatile int Key_Pressed = 0;

void Main(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 1); 
	Macro_Write_Block(GPIOB->MODER, 0x3, 0x0, 16);
	Macro_Write_Block(GPIOB->PUPDR, 0x3, 0x2, 16);

	Sys_Init(115200);
	printf("\nEvent Driven KEY Interrupt Test\n");

	Key_Pressed = 0;

	for(;;)
	{
		if((Key_Pressed==0)&&(Macro_Check_Bit_Clear(GPIOB->IDR, 8)))
		{
			Macro_Invert_Bit(GPIOA->ODR, 5);
			Key_Pressed = 1;
		}
		else if((Key_Pressed == 1)&&(Macro_Check_Bit_Set(GPIOB->IDR, 8)))
		{
			Key_Pressed = 0;
		}
	}
}
