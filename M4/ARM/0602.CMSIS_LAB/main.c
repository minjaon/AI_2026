#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
  SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
}

void Main(void)
{
	volatile int i;

	Sys_Init(115200);
	printf("CMSIS Based Register Define\n");

	// LED Pin을 출력으로 설정
	GPIOA->MODER = 0x00000400;  // PA5를 출력으로 설정
	GPIOA -> OTYPER = 0x00000000; // PA5를 Push-Pull로 설정


	for(;;)
	{
		// LED ON

		GPIOA->ODR = 0x00000020;  // PA5를 High로 설정
		for(i=0; i<0x40000; i++);

		// LED OFF

		GPIOA->ODR = 0x00000000;  // PA5를 Low로 설정
		for(i=0; i<0x40000; i++);
	}
}
