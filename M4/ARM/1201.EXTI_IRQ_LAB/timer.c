#include "device_driver.h"

/* TIM2 타이머 */

// 1. 타이머 1 틱(Tick)의 시간: 20 Microsecond (20us = 0.00002초)
// 타이머 카운터(CNT)가 1씩 증가 또는 감소할 때 걸리는 시간
#define TIM2_TICK           (20)                // usec

// 2. 타이머 동작 주파수: 50,000 Hz (50 kHz)
// 1초(1,000,000us)를 1 틱 시간(20us)으로 나누어 초당 틱 발생 횟수
#define TIM2_FREQ           (1000000/TIM2_TICK) // Hz

// 3. 1ms(Millisecond) 동안 발생하는 틱(펄스) 개수: 50개
// 1ms는 1000us이므로, 1000us / 20us = 50번의 카운트가 일어나야 1ms
// 지연 시간(delay) 계산 시 "입력받은 ms * 50"을 수행해 카운트할 총 펄스 수.
#define TIME2_PLS_OF_1ms    (1000/TIM2_TICK)

// 4. TIM2 카운터의 최대 한계 값: 65,535 (16비트 최대값 0xFFFF)
// STM32의 기본 16비트 타이머 레지스터(ARR 등)에 채울 수 있는 최대 숫자
#define TIM2_MAX            (0xffffu)


/* TIM4 타이머*/

// 1. 타이머 1 틱(Tick)의 시간: 20 Microsecond
#define TIM4_TICK           (20)                // usec

// 2. 타이머 동작 주파수: 50,000 Hz (50 kHz)
#define TIM4_FREQ           (1000000/TIM4_TICK) // Hz

// 3. 1ms 동안 발생하는 틱(펄스) 개수: 50개
#define TIME4_PLS_OF_1ms    (1000/TIM4_TICK)

// 4. TIM4 카운터의 최대 한계 값: 65,535
#define TIM4_MAX            (0xffffu)

void TIM2_Stopwatch_Start(void)
{
    /* 1. TIM2 클록 활성화 */
    Macro_Set_Bit(RCC->APB1ENR, 0);

    /* 2. Down-counter, One-pulse 설정 (CR1) */
    TIM2->CR1 = (1<<4)|(1<<3);

    /* 3. 50kHz 입력 타이머 클록 프리스케일러 계산 (20us 마다 1틱) */
    TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;

    /* 4. 카운트 시작점으로 최대 16비트 값(0xFFFF) 세팅 */
    TIM2->ARR = TIM2_MAX;

    /* 5. 레지스터 강제 업데이트 및 타이머 동작 개시 */
    Macro_Set_Bit(TIM2->EGR,0);
    Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
    unsigned int time;

    /* 1. TIM2 타이머 중지 */
    Macro_Clear_Bit(TIM2->CR1, 0);

    /* 2. 경과 시간 계산
       - 카운트다운 모드이므로 (초기값 0xFFFF - 현재 남아있는 CNT값) x Tick주기(20us)
       - 최종 경과 시간(us 단위) 반환 */
    time = (TIM2_MAX - TIM2->CNT) * TIM2_TICK;
    return time;
}
/* Delay Time Max = 65536 * 20use = 1.3sec */

#if 0

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->ARR = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);

	while(Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
}

#else

/* Delay Time Extended */

void TIM2_Delay(int time)
{
    int i;
    unsigned int t = TIME2_PLS_OF_1ms * time; // 1ms당 클록 펄스 수를 곱해 총 펄스 수 계산

    /* 1. TIM2 타이머 클록 활성화
       - RCC->APB1ENR 비트 0 (TIM2EN) 설정
       - TIM2 타이머 하드웨어에 클록 공급 */
    Macro_Set_Bit(RCC->APB1ENR, 0);

    /* 2. 프리스케일러(Prescaler) 설정
       - TIM2->PSC = (TIMXCLK / TIM2_FREQ) - 1
       - 타이머 입력 클록을 분항하여 desired 틱 주기를 맞춤 */
    TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;

    /* 3. 타이머 동작 모드 설정 (CR1 레지스터)
       - Bit 4 = 1 (Downcounter): 카운트 다운 모드
       - Bit 3 = 1 (One-pulse mode): 카운트 완료 시 자동 정지
       - 정해진 펄스만큼만 카운트하고 멈추는 딜레이 동작 수행 */
    TIM2->CR1 = (1<<4)|(1<<3);

    /* 4. Auto-reload 레지스터 16비트 최대값 설정
       - TIM2->ARR = 0xffff (65535)
       - 긴 시간을 지연할 때 Overflow 단위 분할 처리를 위해 최대 카운터 값 지정 */
    TIM2->ARR = 0xffff;

    /* 5. 업데이트 이벤트 강제 발생 (EGR 레지스터)
       - TIM2->EGR 비트 0 (UG: Update Generation) 설정
       - 변경된 PSC 및 ARR 설정값을 타이머 그림자(Shadow) 레지스터에 즉시 적용 */
    Macro_Set_Bit(TIM2->EGR,0);

    /* 6. 16비트 한계를 넘는 요청 시간을 오버플로우 단위로 나누어 루프 지연 */
    for(i=0; i<(t/0xffffu); i++)
    {
        Macro_Set_Bit(TIM2->EGR,0);        // 카운터 및 레지스터 리셋
        Macro_Clear_Bit(TIM2->SR, 0);       // UIF(업데이트 인터럽트 플래그) 클리어
        Macro_Set_Bit(TIM2->CR1, 0);        // CEN(Counter Enable): 타이머 시작
        while(Macro_Check_Bit_Clear(TIM2->SR, 0)); // 카운트가 끝날 때까지 대기(UIF 비트 체크)
    }

    /* 7. 남은 펄스 수만큼 자투리 시간 지연 */
    TIM2->ARR = t % 0xffffu;                // 잔여 펄스 수를 ARR에 설정
    Macro_Set_Bit(TIM2->EGR,0);             // 변경된 ARR값 업데이트
    Macro_Clear_Bit(TIM2->SR, 0);          // UIF 플래그 클리어
    Macro_Set_Bit(TIM2->CR1, 0);           // CEN: 타이머 재시작
    while (Macro_Check_Bit_Clear(TIM2->SR, 0)); // 지연 완료 대기

    /* 8. 타이머 정지
       - TIM2->CR1 비트 0 (CEN) 클리어 */
    Macro_Clear_Bit(TIM2->CR1, 0);
}

#endif

void TIM4_Repeat(int time)
{
    /* 1. TIM4 타이머 클록 활성화
       - RCC->APB1ENR 비트 2 (TIM4EN) 설정 */
    Macro_Set_Bit(RCC->APB1ENR, 2);

    /* 2. 타이머 카운터 설정
       - Bit 4 = 1: 카운트 다운 모드
       - Bit 3 = 0: 연속 카운트 모드 (One-pulse가 아님, 반복 실행) */
    TIM4->CR1 = (1<<4)|(0<<3);

    /* 3. 분주기(PSC) 및 자동 로드(ARR) 값 계산
       - PSC: 입력 클록 분주
       - ARR: 타임아웃 주기 설정 (-1은 0부터 카운트하는 특성 때문) */
    TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
    TIM4->ARR = TIME4_PLS_OF_1ms * time - 1;

    /* 4. 레지스터 즉시 반영 및 카운터 시작
       - EGR.UG 비트로 PSC/ARR 동기화
       - SR 비트 0 클리어하여 상태 초기화
       - CR1.CEN 비트 설정으로 카운터 구동 시작 */
    Macro_Set_Bit(TIM4->EGR,0);
    Macro_Clear_Bit(TIM4->SR, 0);
    Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
    /* TIM4의 SR(Status Register) 비트 0 (UIF: Update Interrupt Flag) 확인
       - 타이머 카운트가 지정된 주기에 도달하면 1로 세팅됨 */
    if(Macro_Check_Bit_Set(TIM4->SR, 0))
    {
        Macro_Clear_Bit(TIM4->SR, 0); // 소프트웨어적으로 플래그를 0으로 클리어
        return 1; // 타임아웃 발생 알림
    }
    else
    {
        return 0; // 아직 시간 남음
    }
}

void TIM4_Stop(void)
{
    /* TIM4->CR1 비트 0 (CEN) 클리어: 타이머 동작 중지 */
    Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Change_Value(int time)
{
    /* TIM4 주기를 실시간으로 변경: ARR(Auto-reload register) 값 재설정 */
    TIM4->ARR = TIME4_PLS_OF_1ms * time;
}
#define TIM3_FREQ 	  			(8000000) 	      	// Hz
#define TIM3_TICK	  			(1000000/TIM3_FREQ)	// usec
#define TIME3_PLS_OF_1ms  		(1000/TIM3_TICK)

void TIM3_Out_Init(void)
{
    /* 1. GPIOB 및 TIM3 클록 활성화
       - RCC->AHB1ENR Bit 1: GPIOB 클록 ON
       - RCC->APB1ENR Bit 1: TIM3 클록 ON */
    Macro_Set_Bit(RCC->AHB1ENR, 1);
    Macro_Set_Bit(RCC->APB1ENR, 1);

    /* 2. PB0 핀을 Alternate Function(AF) 모드로 설정
       - GPIOB->MODER 비트 0~1을 0b10(AF 모드)으로 설정 */
    Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 0);

    /* 3. PB0 핀의 AF 기능으로 TIM3_CH3 선택
       - GPIOB->AFR[0] (AFRL) 비트 0~3에 0x2 (AF2 = TIM3~5) 지정 */
    Macro_Write_Block(GPIOB->AFR[0], 0xf, 0x2, 0);

    /* 4. TIM3 Channel 3 출력 비교/PWM 모드 설정 (CCMR2 레지스터)
       - 비트 0~7 영역에 0x60 설정 (PWM Mode 1: CNT < CCR 일 때 Active 출력) */
    Macro_Write_Block(TIM3->CCMR2, 0xff, 0x60, 0);

    /* 5. TIM3 Channel 3 출력 활성화 (CCER 레지스터)
       - Bit 8 (CC3E = 1): CH3 신호 핀 출력 Enable
       - Bit 9 (CC3P = 0): High-Active 출력 극성 */
    TIM3->CCER = (0<<9)|(1<<8);
}

void TIM3_Out_Freq_Generation(unsigned short freq)
{
    /* 1. 분주기(PSC) 설정 */
    TIM3->PSC = (unsigned int)(TIMXCLK/(double)TIM3_FREQ + 0.5)-1;

    /* 2. 주기(ARR) 및 듀티사이클(CCR3) 계산
       - ARR: 주파수를 결정 (목표 주파수에 맞는 카운트 한계값)
       - CCR3 = ARR / 2: 듀티비 50%의 펄스파(Square wave) 생성 */
    TIM3->ARR = (double)TIM3_FREQ/freq-1;
    TIM3->CCR3 = TIM3->ARR/2;

    /* 3. 설정 반영 및 타이머 실행
       - EGR.UG=1: 설정을 쉐도우 레지스터에 적용
       - CR1 설정: Up-counter, Continuous, 카운터 활성화(CEN=1) */
    Macro_Set_Bit(TIM3->EGR,0);
    TIM3->CR1 = (1<<4)|(0<<3)|(0<<1)|(1<<0);
}

void TIM3_Out_Stop(void)
{
    /* TIM3->CR1 비트 0 (CEN) 클리어: 파형 출력 정지 */
    Macro_Clear_Bit(TIM3->CR1, 0);
}