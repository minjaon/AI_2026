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
    static int i = 0;
    USART2 -> DR = p[i]; //널문자 인쇄하지 말고 uart_tx가 끝나면 1로 주게끔
    if (p[i] == '\0')
    {
        Uart_Tx_End = 1;
        //인터럽트 금지
        Macro_Clear_Bit(USART2->CR1, 7);
        NVIC_DisableIRQ(38);

    }
    i++;
    // NVIC Pending Clear
    NVIC_ClearPendingIRQ(38);

}