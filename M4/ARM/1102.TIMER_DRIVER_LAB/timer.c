#include "device_driver.h"

// #define TIM2_TICK 20 //usec
// #define TIM2_FREQ (1000000./TIM2_TICK) //hz
// #define TIM2_1ms_Pls (TIM2_FREQ/1000.)
// #define TIM2_MAX 0xFFFFU
#define TIM2_TICK         	(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIM2_1ms_Pls  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

#define TIM4_TICK         	(20) 				// usec
#define TIM4_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIM4_1ms_Pls  	(1000/TIM2_TICK)
#define TIM4_MAX	  		(0xffffu)

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = 0;
	Macro_Write_Block(TIM2->CR1,0x3,0x3,3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (unsigned int)(TIMXCLK/(5.*10000))-1;
	//TIM2-> PSC = (int)((TIMXCLK/TIM2_FREQ) + 0.5) -1;
	// ARR 초기값 설정 => 최대값 0xFFFF 설정
	TIM2->ARR = 0xffff;
	//TIM2->ARR = TIMw_MAX;
	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR,0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1,0);

}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	// TIM2 stop
	Macro_Clear_Bit(TIM2->CR1,0);
	// CNT 초기 설정값 (0xffff)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	time = (0xffff - TIM2->CNT)*TIM2_TICK;
	// 계산된 time 값을 리턴(단위는 usec)
	return time;

}

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (1<<4)|(1<<3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (TIMXCLK/TIM2_FREQ)-1; //(96Mhz/50khz)-1 = 1919

	//20µs마다 1씩 증가하는 카운터(50kHz)

	unsigned int pls = TIM2_1ms_Pls * time;
	int n = pls/ TIM2_MAX;
	int m = pls % TIM2_MAX;
	int i;
	for(i = 0; i<n; i++)
	{
	TIM2->ARR = TIM2_MAX;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR,0);
	Macro_Set_Bit(TIM2->CR1,0);
	while(!(Macro_Check_Bit_Set(TIM2->SR,0)));
	}
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	TIM2->ARR = m; //1ms필요한 50*time
	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR,0);
	// UIF(Update Interrupt Pending) Clear
	Macro_Clear_Bit(TIM2->SR,0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1,0);
	// Wait timeout

	while(!(Macro_Check_Bit_Set(TIM2->SR,0)));

	// TIM2 Stop
	Macro_Clear_Bit(TIM2->CR1, 0);
}

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	// TIM4 CR1: ARPE=0, down counter, repeat mode
	Macro_Clear_Bit(TIM4->CR1, 7);
	Macro_Write_Block(TIM4->CR1,0x3,0x2,3);
	// PSC(50KHz),  ARR(reload시 값) 설정
	TIM4->PSC = (TIMXCLK/TIM4_FREQ)-1;
	TIM4->ARR = (TIM4_1ms_Pls* time);
	// UG 이벤트 발생
	Macro_Set_Bit(TIM4->EGR,0);
	// Update Interrupt Pending Clear
	Macro_Clear_Bit(TIM4->SR,0);
	// TIM4 start
	Macro_Set_Bit(TIM4->CR1,0);

}

int TIM4_Check_Timeout(void)
{
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴
	if(Macro_Check_Bit_Set(TIM4->SR,0))
	{
		Macro_Clear_Bit(TIM4->SR, 0);
		return 1;
	}
	else 
	{
		return 0;
	}

}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = 50 * time;
}

