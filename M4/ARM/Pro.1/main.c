#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "device_driver.h"


extern void Mode_Aurora_Wave(uint8_t *step);
extern void Mode_Pink_Candle_Flicker(void);
extern void WS2812_Encode(unsigned char *p_rgb, int len);
extern void WS2812_Send(void);
extern int WS2812_Is_Busy(void);
extern unsigned char led[12];

static void Sys_Init(int baud) 
{

    SCB->CPACR |= (0x3 << 10*2) | (0x3 << 11*2); 
    
    Clock_Init();    
    Uart2_Init(baud); 
    setvbuf(stdout, NULL, _IONBF, 0); 
    LED_Init();       
}

void Main(void)
{
    Sys_Init(115200);
    printf("=== Smart Mood Lamp Project Start ===\n");

    // WS2812B 펄스 출력용 TIM3_CH3 PWM 기능 초기화
    TIM3_Out_Init();

    uint8_t mode = 0;         // 현재 동작 모드 (0: 오로라, 1: 핑크)
    uint8_t aurora_step = 0;  // 오로라 모드의 색상 변화
    uint16_t mode_timer = 0;  // 모드 전환용 카운터

    for (;;) 
    {
        // 현재 선택된 모드로 led[12] 색상 배열 데이터 갱신
        if (mode == 0) {
            Mode_Aurora_Wave(&aurora_step);
        } else {
            Mode_Pink_Candle_Flicker();
        }

        // Raw RGB 12바이트 데이터를 96개 PWM Duty 값으로 시작
        WS2812_Encode(led, 12);
        WS2812_Send();
        while (WS2812_Is_Busy()); // 인터럽트 전송이 끝날 때까지 대기

        //9초마다 모드 전환
        mode_timer++;
        if (mode_timer > 300) { 
            mode_timer = 0;
            mode = (mode == 0) ? 1 : 0; // 0과 1을 번갈아 토글
            printf("Mode Changed: %d\n", mode);
        }

        TIM2_Delay(30);
    }
}