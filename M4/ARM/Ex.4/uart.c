#include "device_driver.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void Uart2_Init(int baud)
{
  double div;
  unsigned int mant;
  unsigned int frac;

  Macro_Set_Bit(RCC->AHB1ENR, 0);                   // PA2,3
  Macro_Set_Bit(RCC->APB1ENR, 17);                   // USART2 ON
  Macro_Write_Block(GPIOA->MODER, 0xf, 0xa, 4);     // PA2,3 => ALT
  Macro_Write_Block(GPIOA->AFR[0], 0xff, 0x77, 8);  // PA2,3 => AF07
  Macro_Write_Block(GPIOA->PUPDR, 0xf, 0x5, 4);     // PA2,3 => Pull-Up  

  volatile unsigned int t = GPIOA->LCKR & 0x7FFF;
  GPIOA->LCKR = (0x1<<16)|t|(0x3<<2);                // Lock PA2, 3 Configuration
  GPIOA->LCKR = (0x0<<16)|t|(0x3<<2);
  GPIOA->LCKR = (0x1<<16)|t|(0x3<<2);
  t = GPIOA->LCKR;

  div = PCLK1/(16. * baud);
  mant = (int)div;
  frac = (int)((div - mant) * 16. + 0.5);
  mant += frac >> 4;
  frac &= 0xf;

  USART2->BRR = (mant<<4)|(frac<<0);
  USART2->CR1 = (1<<13)|(0<<12)|(0<<10)|(1<<3)|(1<<2);
  USART2->CR2 = 0<<12;
  USART2->CR3 = 0;
}

void Uart2_Send_Byte(char data)
{
  /* 줄바꿈('\n') 문자 처리: Windows 방식의 개행(\r\n)을 맞추기 위해 캐리지 리턴(\r, 0x0D) 먼저 전송 */
  if(data == '\n')
  {
    /* USART2->SR 비트 7 (TXE: Transmit Data Register Empty) 확인
       - TXE가 1이 될 때까지(송신 버퍼가 빌 때까지) 대기 */
    while(!Macro_Check_Bit_Set(USART2->SR, 7));
    
    /* DR (Data Register)에 0x0D('\r') 기록 */
    USART2->DR = 0x0d;
  }

  /* 데이터 송신 버퍼가 비어있는지 재확인 */
  while(!Macro_Check_Bit_Set(USART2->SR, 7));
  
  /* 실제 보내려는 data 문자를 DR 레지스터에 작성하여 전송 실행 */
  USART2->DR = data;
}
void Uart1_Init(int baud)
{
  double div;
  unsigned int mant;
  unsigned int frac;

  /* 1. GPIOA 및 USART1 주변장치 클록 활성화
     - RCC->AHB1ENR Bit 0: GPIOA ON (PA9=TX, PA10=RX)
     - RCC->APB2ENR Bit 4: USART1 ON */
  Macro_Set_Bit(RCC->AHB1ENR, 0); 
  Macro_Set_Bit(RCC->APB2ENR, 4); 

  /* 2. PA9, PA10 핀 모드 설정
     - MODER: Alternate Function (0b10) 설정
     - AFRL/AFRH: AF07(USART1) 할당
     - PUPDR: 내부 Pull-Up 저항(0b01) 적용 (UART 통신 신호 안정화) */
  Macro_Write_Block(GPIOA->MODER, 0xf, 0xa, 18);   
  Macro_Write_Block(GPIOA->AFR[1], 0xff, 0x77, 4); 
  Macro_Write_Block(GPIOA->PUPDR, 0xf, 0x5, 18);   
  
  /* 3. GPIOA 핀 설정 잠금 
     - LCKR 레지스터 시퀀스를 수행하여 설정이 하드웨어 구동 중 오작동으로 변경되는 것 방지 */
  volatile unsigned int t = GPIOA->LCKR & 0x7FFF;
  GPIOA->LCKR = (0x1<<16)|t|(0x3<<9); 
  GPIOA->LCKR = (0x0<<16)|t|(0x3<<9);
  GPIOA->LCKR = (0x1<<16)|t|(0x3<<9);
  t = GPIOA->LCKR;

  /* 4. Baud Rate 계산 및 BRR 레지스터 설정
     - div = PCLK2 / (16 * baudrate)
     - 정수부와 소수부를 나누어 계산 */
  div = PCLK2 / (16. * baud);
  mant = (int)div;
  frac = (int)((div - mant) * 16 + 0.5);
  mant += frac >> 4; // 반올림에 의한 캐리 처리
  frac &= 0xf;
  
  /* 계산된 보레이트 분주값을 BRR 레지스터에 기록 */
  USART1->BRR = (mant<<4)|(frac<<0);

  /* 5. USART1 제어 레지스터 설정
     - CR1: Bit 13(UE: Enable USART), Bit 3(TE: Transmit Enable), Bit 2(RE: Receive Enable)
     - CR2: STOP 비트 길이 지정 (0b00 = 1 Stop bit)
     - CR3: 하드웨어 흐름제어 비활성화 */
  USART1->CR1 = (1<<13)|(0<<12)|(0<<10)|(1<<3)|(1<<2);
  USART1->CR2 = 0 << 12;
  USART1->CR3 = 0;
}

void Uart1_Send_Byte(char data)
{
  if(data == '\n')
  {
    while(!Macro_Check_Bit_Set(USART1->SR, 7));
    USART1->DR = 0x0d;
  }

  while(!Macro_Check_Bit_Set(USART1->SR, 7));
  USART1->DR = data;
}

void Uart1_Send_String(char *pt)
{
  while(*pt != 0)
  {
    Uart1_Send_Byte(*pt++);
  }
}

void Uart1_Printf(char *fmt,...)
{
	va_list ap;
	char string[256];

	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	Uart1_Send_String(string);
	va_end(ap);
}

char Uart1_Get_Pressed(void)
{
	if(Macro_Check_Bit_Set(USART1->SR, 5))
	{
		return (char)USART1->DR;
	}

	else
	{
		return (char)0;
	}
}

char Uart1_Get_Char(void)
{
	while(!Macro_Check_Bit_Set(USART1->SR, 5));
	return (char)USART1->DR;
}
