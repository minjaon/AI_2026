#include "device_driver.h"
#include <stdio.h>
#define GPIOA_MODER  (*(volatile unsigned int *)0x40020000)
#define GPIOA_OTYPER (*(volatile unsigned int *)0x40020004)
#define GPIOA_ODR    (*(volatile unsigned int *)0x40020014)	
static void Sys_Init(int baud) 
{
    SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
}

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("LED ON\n");

	GPIOA->MODER = 0x1 << 10;
	GPIOA->OTYPER = 0x0 << 5;
	GPIOA->ODR = 0x1 << 5; 
}

#endif

#if 0
#define GPIOA_MODER  (*(volatile unsigned int *)0x40020000)
#define GPIOA_OTYPER (*(volatile unsigned int *)0x40020004)
#define GPIOA_ODR    (*(volatile unsigned int *)0x40020014)	

void Main(void)
{
	Sys_Init(115200);
	printf("LED ON : Bit Operation - 1\n");

	/* 비트 연산을 이용하여 LED를 ON하는 코드를 설계하시오 */
	GPIOA_MODER &= ~(0x1 << 11);
	GPIOA_MODER |= (0x1 << 10);
	GPIOA_OTYPER &= ~(0x1 << 5);

	GPIOA_ODR |= (0x1 << 5);

}

#endif

#if 1

void Main(void)
{
	Sys_Init(115200);
	printf("LED Toggling : Macro\n");

	volatile int i;

	/* 매크로를 이용하여 초기에 LED를 출력으로 설정하고 OFF */
	Macro_Clear_Bit(GPIOA->MODER, 10);
    Macro_Set_Bit(GPIOA->MODER, 10);
	Macro_Clear_Bit(GPIOA->OTYPER,5);

	for(;;)
	{
		/* LED 반전 및 Delay, Delay는 0x80000으로 설정 */
		Macro_Invert_Bit(GPIOA->ODR,5);
		for(i=0; i<0x80000; i++);

		// LED OFF

		Macro_Clear_Bit(GPIOA->ODR,5);
		for(i=0; i<0x80000; i++);

	}
}

#endif
void Main(void)
{
	// LED GPA[5]를 출력(General Open Drain) 모드로 설정하시오
	GPIOA_MODER = 0x1 << 10;
	GPIOA_OTYPER = 0x1 << 0;

	// GPA[5]LED를 ON 시키도록 설정하시오
	GPIOA_ODR = 0x0 << 5; 
}
