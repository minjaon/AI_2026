#include "device_driver.h"
#include <stdio.h>
void _Invalid_ISR(void)
{
    /* SCB->ICSR
       - 비트 0~8  영역 추출하여 현재 활성화된 예외 번호 확인 */
    unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
    
    /* Cortex-M 구조에서 System Exception(1~15번)을 제외한 
       실제 하드웨어 무효 인터럽트 IRQ 번호 출력 (Vector - 16) */
    printf("\nInvalid_Exception: %d!\n", r);
    printf("Invalid_ISR: %d!\n", r - 16);
    
    /* 무한 루프로 시스템 안전 동결 */
    for(;;);
}

extern volatile int Key_Pressed;

void EXTI15_10_IRQHandler(void)
{
    /* 1. 전역 플래그 변수 변경: 버튼이 눌렸음을 애플리케이션에 알림 */
    Key_Pressed = 1;
    
    /* 2. EXTI Pending 레지스터 클리어
       - EXTI->PR 비트 13에 1을 써서 인터럽트 요청 완료를 알림 (필수: 비우지 않으면 ISR 재귀 호출 발생) */
    EXTI->PR = 0x1 << 13;

    /* 3. NVIC 인터럽트 대기 플래그 클리어 (IRQ 40) */
    NVIC_ClearPendingIRQ(40);
}
// [전역 변수 선언]
extern volatile int Uart_Data_In;
extern volatile unsigned char Uart_Data;

extern volatile char *p;         // 전송할 문자열의 시작 주소를 가리키는 포인터
extern volatile int len;         // 문자열 길이 (본 함수에서는 사용되지 않음)
extern volatile int Uart_Tx_End; // 문자열 전송 완료 여부를 저장하는 플래그 (1: 완료)

void USART2_IRQHandler(void)
{
    /* static 변수: ISR이 종료되어도 값이 유지되며, 다음 인터럽트 발생 시 이전 값을 이어받음
       - i: 문자열 배열의 현재 전송 인덱스 */
    static int i = 0;

    /*  송신 데이터 레지스터(DR)에 현재 인덱스의 문자 세팅
       USART2->DR (Data Register): 이 레지스터에 데이터를 쓰면 하드웨어가 직렬(Serial) 신호로 출력함
       널 문자('\0')를 포함하여 현재 위치의 글자를 우선 전송 레지스터로 보냄 */
    USART2 -> DR = p[i]; 

    /* 전송할 문자가 널 문자('\0', 문자열의 끝)인지 확인 */
    if (p[i] == '\0')
    {
        /* 문자열 전송 완료 플래그를 1로 설정하여 메인 루프 등에 알림 */
        Uart_Tx_End = 1;

        /* USART2 TXE (송신 데이터 버퍼 비어있음) 인터럽트 비활성화
           USART2->CR1 비트 7 (TXEIE: TX Empty Interrupt Enable)을 0으로 클리어
        DR이 비어있을 때 반복해서 인터럽트가 발생하는 현상(무한 인터럽트)을 방지 */
        Macro_Clear_Bit(USART2->CR1, 7);

        /* NVIC 레벨에서 USART2 인터럽트 수신 차단
           - IRQn 38번 (USART2 인터럽트)을 비활성화
           - 전송이 끝났으므로 CPU가 더 이상 USART2 인터럽트로 들어오지 않도록 차단 */
        NVIC_DisableIRQ(38);
    }

    /* 다음 전송을 위해 인덱스 1 증가 */
    i++;

    /* NVIC 대기(Pending) 플래그 클리어
       IRQ 38번 (USART2)에 대기 중인 인터럽트 처리 완료를 NVIC에 알림
       클리어해주지 않으면 동일 인터럽트 요청이 오인되어 재호출될 수 있음 */
    NVIC_ClearPendingIRQ(38);
}