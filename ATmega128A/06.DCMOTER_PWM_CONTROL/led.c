
/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:10:22
 *  Author: user
 */ 

#include "led.h"
void init_led(void);
void led_all_on(void);
void led_all_off(void);
void led_shift_left_on(void);
void led_shift_right_on(void);
int led_main(void);
void led_shift_right_keep_on(void);
void led_shift_left_keep_on(void);
void led_flower_on(void);
void led_flower_off(void);

#define  FUNC_SU 6
extern volatile uint32_t msec_count ;
int func_state = 0;

void (*fp[]) () =
{
	led_shift_left_on, //func_state = 0
	led_shift_right_on, //1
	led_shift_left_keep_on, //2
	//led_all_off, //3
	led_shift_right_keep_on, //3
	//led_all_off, //5
	led_flower_on, //4
	led_flower_off //func_state = 5
};

int led_main(void){
	
//	uint8_t led_toggle = 0;
	init_led();
	led_all_off();
	while(1)
	{
#if 1

	fp[func_state] ();

#else
		if(msec_count >= 500) //500ms  if(msec_count == 500)
		{
		msec_count = 0;
		led_toggle = !led_toggle;
		if(led_toggle){
			led_all_on();
			}
		else led_all_off();
		
	}
#endif

       /*led_shift_left_keep_on();
		led_all_off();
		led_shift_right_keep_on();
		led_all_off();
		*/
/*	   led_flower_on();

	   led_flower_off();
	   */
	   
	}
	return 0;
}

void init_led(void)
{
	DDRA=0xff;  // PORTA 를 출력 모드로 설정
	PORTA=0x00;  // PORTA에 물려있는 led를 all off 	
}

void led_all_on(void)
{
	PORTA=0xff;
}

void led_all_off(void)
{
	PORTA=0x00;
	func_state = (func_state + 1) % FUNC_SU;
}


void led_shift_left_on() {
#if 1

	static int i = 0; 
	
	if(msec_count >=100)
	{
		msec_count = 0;
		*(unsigned char*)0x3B = 1 << i;
		i = (i + 1) % 8;
// 		if ((i = (i + 1) % 8) == 0)
// 		func_state = (func_state + 1) % FUNC_SU; //다음 실행할 func 으로 jump
	}
	             

#endif
#if 0

	static int i = 0;
	
	*(unsigned char*)0x3B = 1 << i;
	_delay_ms(30);
	i = (i + 1) % 8;
	
/*	for (int i = 0; i < 8; i++) {
		*(unsigned char*)0x3B = 1 << i;
		_delay_ms(30);
*/	
	}
#endif
}


void led_shift_right_on() {
#if 1
	
	static int i = 0; 
	if(msec_count >=100)
	{
		msec_count = 0;
		*(unsigned char*)0x3B = 0x80 >> i;
		i = (i + 1) % 8;
// 		if ((i = (i + 1) % 8) == 0)
// 		func_state = (func_state + 1) % FUNC_SU; //다음 실행할 func 으로 jump
// 	}
	// *(unsigned char*)0x3B = 1 << (7 - i); 
                 }

#else
	
	for (int i = 0; i < 8; i++) {
		*(unsigned char*)0x3B = 1 << (7 - i);
		_delay_ms(30);
	}
#endif
}

void led_shift_left_keep_on(){

		static int i = 0;
		if(msec_count >=100){
		msec_count = 0;
		*(unsigned char*)0x3B |= 1 << i;
		if ((i = (i + 1) % 8) == 0)
		func_state = (func_state + 1) % FUNC_SU; //다음 실행할 func 으로 jump
				}
		}


void led_shift_right_keep_on(){
			static int i = 0;
			if(msec_count >=100){
				msec_count = 0;
				*(unsigned char*)0x3B |= 0x80 >> i;
				if ((i = (i + 1) % 8) == 0)
				func_state = (func_state + 1) % FUNC_SU; //다음 실행할 func 으로 jump
			}
}

void led_flower_on(){
		static int i = 0;
		if(msec_count >=100){
		msec_count = 0;
		*(unsigned char*)0x3B |= (1 << (3 - i) | 1 << (4 + i));
		if ((i = (i + 1) % 8) == 0)
		func_state = (func_state + 1) % FUNC_SU; //다음 실행할 func 으로 jump
				}
}
	/*
	PORTA |= (1<<4) | (1<<3); _delay_ms(300);
	PORTA |= (1<<5) | (1<<2); _delay_ms(300);
	PORTA |= (1<<6) | (1<<1); _delay_ms(300);
	PORTA |= (1<<7) | (1<<0); _delay_ms(300);
*/


void led_flower_off(){
	
		static int i = 0;
		if(msec_count >=100){
		msec_count = 0;
		*(unsigned char*)0x3B &= ~(1 << (0 + i) | 1 << (7 - i));
		if ((i = (i + 1) % 8) == 0)
		func_state = (func_state + 1) % FUNC_SU; //다음 실행할 func 으로 jump
		}
}
	/*	PORTA &= ~((1<<7) | (1<<0)); _delay_ms(300);
	PORTA &= ~((1<<6) | (1<<1)); _delay_ms(300);
	PORTA &= ~((1<<5) | (1<<2)); _delay_ms(300);
	PORTA &= ~((1<<4) | (1<<3)); _delay_ms(300);
*/

