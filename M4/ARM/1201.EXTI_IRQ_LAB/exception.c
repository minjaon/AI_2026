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