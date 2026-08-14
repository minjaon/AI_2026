#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

void Main(void)
{

	Macro_Set_Bit(RCC->AHB1ENR, 1); 
	Macro_Write_Block(GPIOB->MODER, 0x3, 0x0, 16);
	Macro_Write_Block(GPIOB->PUPDR, 0x3, 0x2, 16);

	Sys_Init(115200);
	printf("KEY Input Test #1\n");

	/* 아래 코드 수정 금지 : Port-B Clock Enable */


	for(;;)
	{
		// KEY가 눌렸으면 LED(PA5) ON, 안 눌렸으면 OFF
		if(Macro_Check_Bit_Set(GPIOB->IDR,8)) //C핀에 어떤 전류가 들어오는지 보는 IDR
		{
			LED_On();
		}
		else
		{
			LED_Off();
		}

		// LED OFF

	}
}
