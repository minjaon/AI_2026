#include "device_driver.h"

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2); 
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);
}

int Key_Get_Pressed(void)
{
	return Macro_Check_Bit_Clear(GPIOC->IDR, 13);	
}

void Key_Wait_Key_Pressed(void)
{
	while(!Macro_Check_Bit_Clear(GPIOC->IDR, 13));
}

void Key_Wait_Key_Released(void)
{
	while(!Macro_Check_Bit_Set(GPIOC->IDR, 13));
}

void Key_ISR_Enable(int en)
{
    if(en)
    {
        /* GPIOC 클록 활성화
           RCC->AHB1ENR 비트 2 (GPIOCEN) 설정
           GPIOC 주변장치에 클록을 공급해야 PC13 핀을 사용할 수 있음 */
        Macro_Set_Bit(RCC->AHB1ENR, 2); 

        /* PC13 핀을 입력 모드(0x0)로 설정
           GPIOC->MODER의 비트 26~27(MODER13)을 0b00(Input)으로 변경
           버튼/키 입력을 받아들이기 위해 핀 방향을 입력으로 지정 */
        Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);

        /* 3. SYSCFG(시스템 구성) 장치 클록 활성화
           RCC->APB2ENR 비트 14 (SYSCFGEN) 설정
           GPIO 핀을 EXTI(외부 인터럽트) 라인에 매핑하려면 SYSCFG 모듈이 활성화되어야 함 */
        Macro_Set_Bit(RCC->APB2ENR, 14);

        /* 4. EXTI13의 신호 소스로 GPIOC(PC13) 지정
           SYSCFG->EXTICR[3]의 비트 4~7 영역에 0x2(GPIOC)를 기록
           EXTI 라인 13에 PA13, PB13, PC13 중 PC13을 연결하기 위함 */
        Macro_Write_Block(SYSCFG->EXTICR[3], 0xf, 0x2, 4);

        /* 5. EXTI 라인 13을 하강 엣지(Falling Edge) 트레이거로 설정
           EXTI->FTSR 비트 13 설정
           버튼을 누를 때(High -> Low) 인터럽트를 발생시키기 위함 */
        Macro_Set_Bit(EXTI->FTSR, 13);

        /* 6. EXTI13의 기존 대기(Pending) 인터럽트 플래그 클리어
           - EXTI->PR 비트 13에 1을 써서 클리어
           - 설정 과정에서 의도치 않게 발생했을 수 있는 잔여 인터럽트 신호 제거 */
        EXTI->PR = 0x1 << 13;

        /* 7. NVIC(내장 인터럽트 제어기) 대기 상태 클리어
           - IRQn 40 (EXTI15_10)에 대기 중인 인터럽트 취소
           - 중첩 또는 이전 잔여 인터럽트로 인해 활성화되자마자 ISR로 튀는 현상 방지 */
        NVIC_ClearPendingIRQ(40);

        /* 8. EXTI 라인 13 인터럽트 마스크 해제 (Enable)
           - EXTI->IMR 비트 13 설정
           - EXTI13에서 발생한 신호가 NVIC로 전달되도록 허용 */
        Macro_Set_Bit(EXTI->IMR, 13);

        /* 9. NVIC level에서 EXTI15_9 인터럽트 활성화
           - NVIC_EnableIRQ(40) 호출 (IRQ 40 = EXTI15_10_IRQn)
           - 최종적으로 CPU가 해당 인터럽트를 수신하여 ISR을 실행하도록 허용 */
        NVIC_EnableIRQ(40);
    }
    else
    {
        /* 인터럽트 비활성화: NVIC 수준에서 EXTI15_10 인터럽트 수신 차단 */
        NVIC_DisableIRQ((IRQn_Type)40);
    }
}