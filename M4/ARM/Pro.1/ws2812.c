#include <stdio.h>         // printf 사용
#include "device_driver.h"
#include "stm32f4xx.h"

#define WS_ARR_BIT   (120 - 1)          // PWM 1주기 타이머 카운트값 (1.25us 펄스 생성)
#define WS_T0H       38                 // 0일 때 High 유지 시간 ( 0.4us)
#define WS_T1H       82                 // 1일 때 High 유지 시간 ( 0.8us)
#define WS_RESET_N   48                 // Reset 신호 주기 횟수 -> 60us 동안 Low

#define WS_MAX_LED   4                  // 제어할 LED 개수
#define WS_MAX_BIT   (WS_MAX_LED * 24)  // 총 송신 비트 수 (4개 x 24비트 = 96비트)

// 드라이버 내부 상태 관리 정적 변수들
static unsigned short ws_buf[WS_MAX_BIT];        // 96개 비트의 CCR3 Duty 값이 저장될 버퍼
static volatile int ws_idx, ws_total, ws_rst;    // 전송 인덱스 및 리셋 카운터
static volatile int ws_busy;                     // 전송 중 플래그 (1: 전송 중, 0: 완료)
static volatile int ws_pre;                      // 데이터 전송 전 Pre-Reset 카운터



void WS2812_Encode(unsigned char *grb, int nbyte)
{
    int i, b, k = 0;
    for(i = 0; i < nbyte; i++)
    {
        // MSB부터 LSB순서로 1비트씩 확인
        for(b = 7; b >= 0; b--)
        {
            // 비트가 1이면 WS_T1H(82), 0이면 WS_T0H(38) 값을 배열
            ws_buf[k++] = (grb[i] & (1 << b)) ? WS_T1H : WS_T0H;
        }
    }
    ws_total = k; 
}



void WS2812_Send(void)
{
    // 전송 상태 변수 초기화
    ws_idx = 0;// 읽을 버퍼 위치를 0으로 세팅
    ws_rst = 0;
    ws_pre = WS_RESET_N; // (60us) 설정
    ws_busy = 1;         // 상태 변경

    TIM3->PSC = 0;           // 분주비 0
    TIM3->ARR = WS_ARR_BIT;  // 주기 1.25us

    //출력을 0으로 하기 위해 PWM Duty 레지스터 초기화                  
    Macro_Set_Bit(TIM3->EGR, 0);        
    TIM3->CCR3 = 0;                    

    Macro_Clear_Bit(TIM3->SR, 0);       
    NVIC_ClearPendingIRQ(29);           
    NVIC_SetPriority(29, 0);          
    Macro_Set_Bit(TIM3->DIER, 0);      
    NVIC_EnableIRQ(29);                

    // TIM3 타이머 시작 (ARPE=1, CEN=1)
    TIM3->CR1 = (1<<7) | (0<<4) | (0<<3) | (1<<0);
}

int WS2812_Is_Busy(void) { return ws_busy; } //현재 전송이 진행 중인지 여부를 반환 1: 전송 중, 0: 전송 완료

void TIM3_IRQHandler(void)
{
    Macro_Clear_Bit(TIM3->SR, 0); // 타이머 인터럽트 플래그 클리어

    // 전송 전 Pre-Reset 신호 출력 
    if(ws_pre > 0) 
    {
        TIM3->CCR3 = 0;
        ws_pre--;
    }
    // CCR3에 적용하여 펄스 변조
    else if(ws_idx < ws_total)
    {
        TIM3->CCR3 = ws_buf[ws_idx++];
    }
    //데이터 전송 후 출력 
    else if(ws_rst < WS_RESET_N)
    {
        TIM3->CCR3 = 0;
        ws_rst++;
    }
    else
    {
        Macro_Clear_Bit(TIM3->CR1, 0);  // TIM3 카운터 정지
        Macro_Clear_Bit(TIM3->DIER, 0); // TIM3 인터럽트 비활성화
        NVIC_DisableIRQ(29);       
        ws_busy = 0;                  
    }
}


void WS2812_Debug_Print(void)
{
    int i;
    printf("ws_buf: ");
    for(i = 0; i < 24; i++)
        printf("%d ", ws_buf[i]);
    printf("\n");
}