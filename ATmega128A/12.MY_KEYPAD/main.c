/*
 * 06.DCMOTOR_PWM_CONTROL
 *
 * Created: 2026-06-15 오전 11:36:45
 * Author : user
 */ 

#define F_CPU 16000000UL     // CPU 클럭 속도를 16MHz로 설정
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>   // 인터럽트(sei, ISR 등) 기능을 쓰기 위한 헤더
#include <stdio.h>           // printf 기능을 쓰기 위한 헤더
#include "queue.h"

extern void init_keypad(void);
extern uint8_t keypad_scan(void);
extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);
extern void calculator_processing(uint8_t key);
volatile uint32_t keypad_counter = 0; 
void init_timer0(void);


ISR(TIMER0_OVF_vect)
{
	
	volatile uint8_t keydata = 0;
	
	TCNT0 = 6;    // 256방 중 6부터 세기 시작하니까 250카운트(250 x 4us = 1ms) 주기 완성!
	if(++keypad_counter >=60 ) //60ms reached
	{
		keypad_counter = 0;
		if(keydata = keypad_scan()) //keypad를 check해서 눌러진것이 있으면
		{
			insert_queue(keydata);	   //circular queue에 저장한다.
		}
	}
}

// ===================================================
// 메인 함수: 프로그램이 시작되면 가장 먼저 실행되는 본부
// ===================================================
int main(void)
{
	uint8_t key_value;
	
	init_timer0();
	init_uart0();
	init_keypad();
	queue_init();
	stdout = &OUTPUT;   
	sei();            
	
	while(1)
	{
		if (queue_empty() != TRUE)
		{
			key_value=read_queue();
			calculator_processing(key_value);
		}
	}
}


void init_timer0(void)
{
	TCNT0 = 6;
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	TCCR0 |= (1 << CS02);   // 프리스케일러(분주비)를 64로 설정! (16MHz / 64 = 250kHz -> 1카운트당 4us 소요)
	TIMSK |= (1 << TOIE0);  // 타이머0이 넘쳐 흐를 때 인터럽트를 터뜨리겠다는 허가령 활성화!
}