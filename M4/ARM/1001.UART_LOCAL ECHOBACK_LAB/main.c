#include "device_driver.h"
#include <stdio.h>

void stop(void);
void forward(void);
void reverse(void);

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("\nUART Echo-Back Test\n");

	Uart1_Init(115200);

	Macro_Set_Bit(RCC->AHB1ENR, 0); 
   	Macro_Write_Block(GPIOA->MODER, 0xf, 0x5, 0);
	Macro_Clear_Bit(GPIOA->OTYPER,0);
	Macro_Clear_Bit(GPIOA->OTYPER,1);
	Macro_Write_Block(GPIOA->MODER, 0x3, 0x0, 10);
	Macro_Write_Block(GPIOA->PUPDR, 0x3, 0x1, 10);
	//처음에는 stop, 스위치PA5 누를때마다 cw-ccw(방향전환할때 delay(1))
	//스위치 3초이상 누르면 off, 다시 누르면 start
	stop();
	int flag;
	for(;;){
	if (Macro_Check_Bit_Clear(GPIOA->IDR, 5)) 
        {
            stop();
            
            if (flag == 0 && (Macro_Check_Bit_Set(GPIOA->ODR, 0)))
            {
				flag = 1;
                forward();
            }
            else 
            {
				TIM2_Delay(1000); 
                reverse();
            }

            while (Macro_Check_Bit_Clear(GPIOA->IDR, 5));
        }
	}



}

void stop(void)
{
	Macro_Clear_Bit(GPIOA->ODR,0);
	Macro_Clear_Bit(GPIOA->ODR,1);
}

void forward(void)
{
	Macro_Set_Bit(GPIOA->ODR,0);
	Macro_Clear_Bit(GPIOA->ODR,1);
}

void reverse(void)
{
	Macro_Clear_Bit(GPIOA->ODR,0);
	Macro_Set_Bit(GPIOA->ODR,1);
}


#endif

#if 0
void Main(void)
{
	Sys_Init(115200);
	printf("\nUART Echo-Back Test\n");

	Uart1_Init(115200);

	for(;;)
	{
		char x;

		//수신?
		while(!(Macro_Check_Bit_Set(USART1->SR,5)))
		{

		}
		//x = dr
		x = USART1->DR;
		//송신?
		while(!(Macro_Check_Bit_Set(USART1->SR,7)))
		{

		}
		//dr = x;
		USART1->DR = x;
	}
}

#endif

#if 0
void Main(void)
{
	Sys_Init(115200);
	printf("\nUART Echo-Back Test\n");

	Uart1_Init(115200);


	while(!(Macro_Check_Bit_Set(USART1->SR,7)))
	{
	}
	USART1->DR = 'A';

	while(!(Macro_Check_Bit_Set(USART1->SR,7)))
	{
	}
	USART1->DR = 'B';
	while(!(Macro_Check_Bit_Set(USART1->SR,7)))
	{
	}
	USART1->DR = 'C';
}
#endif


