/*
 * main.c
 *
 * Created: 2026-06-10 오후 2:17:32
 *  Author: kccistc
 */ 
#define F_CPU 16000000UL    // 16MHz
#include <avr/io.h>            // PORTA PORTD 등의 I/O register들이 들어있음.
#include <util/delay.h>        // _delay_ms _delay_us 등의 함수가 들어있음.

#include "button.h"

extern void init_led(void); //init_led 함수는 다른 file에 들어있다고 complier신고
extern void init_button(void);
extern int get_button(int button_num,int button_pin);
extern void led_all_on(void);
extern void led_all_off(void);
void led_odd_on(void);
void led_even_on(void);

#if 1
int main(void){

	int button0_state = 0; //초기상태를 led all off로 출발
	init_button();
	init_led();
	
	int state = 0;

	while(1)
	{
		if(get_button(BUTTON0, BUTTON0PIN))
		{
			state++;

			if(state > 3)
			state = 0;

			if(state == 0)
			led_all_off();     
			else if(state == 1)
			led_odd_on();    
			else if(state == 2)
			led_even_on();     
			else
			led_all_on();    
		}
	}
	}
	

#endif

#if 0
int main(void)
{
	DDRA = 0b11111111;        // PORTA에 LED가 8개 연결되어 있으므로 all 1(출력)으로 설정
	// DDR : data direction register의 약자로, 방향 설정
	// 1:출력, 0:입력
	while (1)
	{
		PORTA = 0b11111111; // all on
		_delay_ms(1000);    // 1초 유지 (1000ms)
		PORTA = 0b00000000;    // all off
		_delay_ms(1000);
	}
	
}
#endif