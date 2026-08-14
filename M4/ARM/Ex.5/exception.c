#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
    unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
    printf("\nInvalid_Exception: %d!\n", r);
    printf("Invalid_ISR: %d!\n", r - 16);
    for(;;);
}

extern volatile int Key_Pressed;

void EXTI15_10_IRQHandler(void)
{
    Key_Pressed = 1;

    EXTI->PR = 0x1 << 13;
    NVIC_ClearPendingIRQ(40);
}

extern volatile int Uart_Data_In;
extern volatile unsigned char Uart_Data;

extern volatile char *p;
extern volatile int len;
extern volatile int Uart_Tx_End;

void USART2_IRQHandler(void)
{
	printf("UART RX_INT!!");
    if(Uart_Data_In == 0)
    {
		printf("Uart_Data_In = 0!!");
		Uart_Data = (unsigned char)USART2->DR;
        Uart_Data_In = 1;

        if(Uart_Data == 'A' || Uart_Data == 'a')
        {
			LED_On();
            TIM2_Delay(500);
            LED_Off();

			Uart_Data_In = 0;
        }
		else if(Uart_Data == 'B' || Uart_Data == 'b')
		{
			LED_Off();
			Uart_Data_In = 0;
		}
	    
		else
		{
			printf("Invalid Input. Type Again!!!\n");
			(void) USART2 -> DR;
			Uart_Data_In = 0;
		}
    }
    
	else // Uart_Data_In == 0
	{
		(void)USART2->DR;
	}

    NVIC_ClearPendingIRQ(38);

}