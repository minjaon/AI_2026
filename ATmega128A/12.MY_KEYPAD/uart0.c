/*
 * uart0.c
 *
 * Created: 2026-06-16 오전 9:57:51
 *  Author: kccistc
 */ 
#include <stdio.h>
#include "uart0.h"
void init_uart0(void);
void UART0_transmit(uint8_t data);
void pc_command_processing(void);

ISR(USART0_RX_vect)
{
	volatile uint8_t data; 
	volatile static int i = 0;
	
	data = UDR0; //hw적으로 data에 복사되는 순간 UDR0는 null
	
	if(data == '\n' || data == '\r')
	{
		if((u_rear+1)% QUEUE_SIZE == u_front % QUEUE_SIZE){ //queue full 상태 
		return; 
		}//queue full 상태
		
		rx_buff[u_rear][i] = '\0';
		i = 0; //다음 string을 저장하기 위해서i를 0으로 만든다
		u_rear = (u_rear + 1) % QUEUE_SIZE; //0~9
	}
	else
	{
		if((u_rear+1)% QUEUE_SIZE == u_front % QUEUE_SIZE){ //queue full 상태
		return;
		}
		rx_buff[u_rear][i++] = data;
	}
}

void init_uart0(void){
	
	//1.전송속도 : 9600bps
	UBRR0H = 0x00;
	UBRR0L = 207; //9600BPS 표 8-9
	UCSR0A |= 1 << U2X0; //2배속 설정(sampling 8)
	//UART0를 송수신 다 가능하고 RX INT가 가능하도록 설정
	UCSR0B |= 1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0;
}

//UART0로 1byte를 전송하는 함수
void UART0_transmit(uint8_t data)
{
	while(!(UCSR0A & 1 << UDRE0)) // data가 송신중이면 송신이 끝날때 까지 기다림(비어있는지 확인하는 작업)
	; //No operation
	UDR0 = data; //HW전송 register에 data를 송신한다.
}

void pc_command_processing(void){
	
}