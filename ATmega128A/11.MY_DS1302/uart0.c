/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:57:51
 * Author: kccistc
 */ 
#include <stdio.h>
#include <string.h>   // strncmp, strlen
#include <util/delay.h> // _delay_us
#include "uart0.h"

extern void write_ds1302(uint8_t addr, uint8_t data);

void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing(void);

/*
	1. 전송속도 : 9600bps
	2. start/stop 설정
	3. RX(수신) : interrupt로 설정 
*/


ISR(USART0_RX_vect)
{
	volatile uint8_t data;
	volatile static int i = 0;
	
	data = UDR0; // 하드웨어에서 데이터 읽기
	
	// 엔터 키(\n 또는 \r)가 들어오면 문자열의 끝으로 처리
	if(data == '\n' || data == '\r')
	{
		if (i > 0) // 버퍼에 글자가 쌓여있을 때만 마감 처리
		{
			rx_buff[rear][i] = '\0'; // 문자열 끝에 널문자 대입
			i = 0;                   // 다음 문자열을 위해 인덱스 초기화
			rear = (rear + 1) % QUEUE_SIZE; // 큐 인덱스 이동
		}
	}
	else
	{
		// 큐가 가득 차지 않았다면 버퍼에 문자 저장
		if(((rear + 1) % QUEUE_SIZE) != (front % QUEUE_SIZE))
		{
			// 혹시 모를 오버플로우 방지 (버퍼 크기보다 작을 때만 저장)
			if (i < 40)
			{
				rx_buff[rear][i++] = data;
			}
		}
	}
}

void init_uart0(void){
	
	// 1.전송속도 : 9600bps
	UBRR0H = 0x00;
	UBRR0L = 207; // 9600BPS 표 8-9
	UCSR0A |= 1 << U2X0; // 2배속 설정(sampling 8)
	// UART0를 송수신 다 가능하고 RX INT가 가능하도록 설정
	UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
}

// UART0로 1byte를 전송하는 함수
void UART0_transmit(uint8_t data)
{
	while(!(UCSR0A & 1 << UDRE0)) // data가 송신중이면 송신이 끝날때 까지 기다림
	; // No operation
	UDR0 = data; // HW전송 register에 data를 송신한다.
}

void pc_command_processing(void){
	
	if (front != rear) // data가 rx_buff에 존재하는지 check
	{
		printf("Input Cmd: %s\n", rx_buff[front]);


		if (strncmp((char *)rx_buff[front], "setrtc", 6) == 0)
		{
			int year, month, date, hour, min, sec;
			
			if (sscanf((char *)&rx_buff[front][6], "%2d%2d%2d%2d%2d%2d", &year, &month, &date, &hour, &min, &sec) == 6)
			{

				write_ds1302(0x8E, 0x00); // 1. 쓰기 금지 해제 
				_delay_us(5);
				
				write_ds1302(0x8C, (uint8_t)year);    // 2. 년 (0x8C)
				write_ds1302(0x88, (uint8_t)month);   // 3. 월 (0x88)
				write_ds1302(0x86, (uint8_t)date);    // 4. 일 (0x86)
				write_ds1302(0x84, (uint8_t)hour);    // 5. 시 (0x84)
				write_ds1302(0x82, (uint8_t)min);     // 6. 분 (0x82)
				write_ds1302(0x80, (uint8_t)sec);     // 7. 초 (0x80)
				
				printf("\n[RTC] 시간 동기화 성공! -> 20%02d-%02d-%02d %02d:%02d:%02d\n", year, month, date, hour, min, sec);
			}
			else
			{
				printf("\n[RTC] 포맷 에러! (ex: setrtc260629112200)\n");
			}
		}
		
		front = (front + 1) % QUEUE_SIZE; 
	}
}