#include "device_driver.h"
#include <stdlib.h>
#define TIM2_TICK         	(20) 				// usec
#define TIM2_FREQ 	  		(1000000/TIM2_TICK)	// Hz
#define TIME2_PLS_OF_1ms  	(1000/TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

#define TIM4_TICK	  		(20) 				// usec
#define TIM4_FREQ 	  		(1000000/TIM4_TICK) // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#define TIM4_MAX	  		(0xffffu)

#define TIM3_FREQ 	  			(8000000) 	      	// Hz
#define TIM3_TICK	  			(1000000/TIM3_FREQ)	// usec
#define TIME3_PLS_OF_1ms  		(1000/TIM3_TICK)
#define WS_MAX_LED   4

unsigned char led[12] = {0};

unsigned char* Get_WS2812_Buf(void) {
    return led;
}
void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	TIM2->ARR = TIM2_MAX;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	Macro_Clear_Bit(TIM2->CR1, 0);
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
	unsigned int t = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->ARR = 0xffff;
	Macro_Set_Bit(TIM2->EGR,0);

	for(i=0; i<(t/0xffffu); i++)
	{
		Macro_Set_Bit(TIM2->EGR,0);
		Macro_Clear_Bit(TIM2->SR, 0);
		Macro_Set_Bit(TIM2->CR1, 0);
		while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	}

	TIM2->ARR = t % 0xffffu;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);
	while (Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
}

#endif

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	TIM4->CR1 = (1<<4)|(0<<3);
	TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
	TIM4->ARR = TIME4_PLS_OF_1ms * time - 1;

	Macro_Set_Bit(TIM4->EGR,0);
	Macro_Clear_Bit(TIM4->SR, 0);
	Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
	if(Macro_Check_Bit_Set(TIM4->SR, 0))
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
	TIM4->ARR = TIME4_PLS_OF_1ms * time;
}

void TIM4_Repeat_Interrupt_Enable(int en, int time)
{
	if(en)
	{
		Macro_Set_Bit(RCC->APB1ENR, 2);

		TIM4->CR1 = (1<<4)|(0<<3);
		TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
		TIM4->ARR = TIME4_PLS_OF_1ms * time;
		Macro_Set_Bit(TIM4->EGR,0);

		Macro_Clear_Bit(TIM4->SR, 0);
		NVIC_ClearPendingIRQ(30);

		Macro_Set_Bit(TIM4->DIER, 0);
		NVIC_EnableIRQ(30);

		Macro_Set_Bit(TIM4->CR1, 0);
	}

	else
	{
		NVIC_DisableIRQ(30);
		Macro_Clear_Bit(TIM4->CR1, 0);
		Macro_Clear_Bit(TIM4->DIER, 0);
	}
}



void TIM3_Out_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 1); // GPIOB 클럭 활성화
    Macro_Set_Bit(RCC->APB1ENR, 1); // TIM3 클럭 활성화

    Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 0); 
    Macro_Write_Block(GPIOB->AFR[0], 0xf, 0x2, 0);  // PB0 핀에 AF02 (TIM3_CH3) 연결

    Macro_Clear_Bit(GPIOB->OTYPER, 0);              // Push-Pull 출력 모드
    Macro_Write_Block(GPIOB->OSPEEDR, 0x3, 0x2, 0); // Fast Speed 속도 설정

    // TIM3 Channel 3을 PWM Mode 1로 설정
    Macro_Write_Block(TIM3->CCMR2,0xff, 0x68, 0);   
    
    // Channel 3 출력 활성화 
    TIM3->CCER = (0<<9)|(1<<8);
}
void TIM3_Out_Fixed_Test(unsigned short ccr)
{
    TIM3->PSC  = 0;
    TIM3->ARR  = 120 - 1;
    TIM3->CCR3 = ccr;

    Macro_Set_Bit(TIM3->EGR, 0);
    Macro_Clear_Bit(TIM3->SR, 0);
    TIM3->CR1 = (1<<7)|(0<<4)|(0<<3)|(1<<0);
}

void TIM3_Out_Freq_Generation(unsigned short freq)
{
	TIM3->PSC = (unsigned int)(TIMXCLK/(double)TIM3_FREQ + 0.5)-1;
	TIM3->ARR = (double)TIM3_FREQ/freq-1;
	TIM3->CCR3 = TIM3->ARR/2;

	Macro_Set_Bit(TIM3->EGR,0);
	TIM3->CR1 = (1<<4)|(0<<3)|(0<<1)|(1<<0);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}

void WS2812_SetPixel(int index, uint8_t r, uint8_t g, uint8_t b)
{
    // LED 4개를 벗어나는 인덱스가 들어오면 메모리 오버플로우 방지를 위해 예외 처리 후 리턴
    if (index < 0 || index >= 4) return;
    
    led[index * 3 + 0] = g;  //Green
    led[index * 3 + 1] = r;  //Red
    led[index * 3 + 2] = b;  //Blue
}

void WS2812_SetHue(int index, uint8_t hue)
{
    uint8_t r = 0, g = 0, b = 0;
    
    // 0~255 범위의 Hue를 6개 영역(0~5)
    uint8_t region = hue / 43;
    
    // 0~255 
    uint8_t remainder = (hue - (region * 43)) * 6;

    // HSV 공식에 따른 보간 비율(q, t) 연산
    uint8_t q = (255 * (255 - remainder)) >> 8;               // 감소 구간 값
    uint8_t t = (255 * (255 - (255 - remainder))) >> 8;       // 증가 구간 값

    // 색상 영역에 따른 RGB
    switch (region) {
        case 0:  r = 255; g = t;   b = 0;   break; // Red -> Yellow
        case 1:  r = q;   g = 255; b = 0;   break; // Yellow -> Green
        case 2:  r = 0;   g = 255; b = t;   break; // Green -> Cyan
        case 3:  r = 0;   g = q;   b = 255; break; // Cyan -> Blue
        case 4:  r = t;   g = 0;   b = 255; break; // Blue -> Magenta
        default: r = 255; g = 0;   b = q;   break; // Magenta -> Red
    }

    //밝기 50% 
    WS2812_SetPixel(index, r >> 1, g >> 1, b >> 1);
}


void Mode_Aurora_Wave(uint8_t *step)
{
    for (int i = 0; i < 4; i++) {
        // LED 간에 35씩 색상 오프셋 그라데이션 형성
        uint8_t hue = (*step + (i * 35)) & 0xFF; // & 0xFF 0~255 
        WS2812_SetHue(i, hue);
    }
    *step += 2; // 다음 프레임에서 색상을 2단계 이동
}


void Mode_Pink_Candle_Flicker(void)
{
    for (int i = 0; i < 4; i++) {
        uint8_t flicker = rand() % 40; // 0 ~ 39  떨림값 생성
        
        // 차감 연산 적용
        uint8_t base_r = (200 > flicker) ? (200 - flicker) : 160;
        uint8_t base_g = (20 > (flicker / 2)) ? (20 - (flicker / 2)) : 5;
        uint8_t base_b = (60 > flicker) ? (60 - flicker) : 20;

        WS2812_SetPixel(i, base_r, base_g, base_b);
    }
}