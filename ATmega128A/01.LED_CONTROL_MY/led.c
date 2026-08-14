/*
 * led.c
 *
 * Created: 2026-06-10 오후 3:12:16
 *  Author: kccistc
 */ 

#include "led.h"

void init_led(void);
void led_all_on(void);
void led_all_off(void);
void led_odd_on(void);
void led_even_on(void);

void init_led(void){
	
	DDRA = 0xff; //PORTA 를 출력모드로 설정
	PORTA = 0x00; //PORTA에 몰려있는 led를 all off	
}

void led_all_on(void){
	PORTA = 0xff;	
}

void led_all_off(void){
	PORTA = 0x00;
}

void led_odd_on(void){
	PORTA = (1<<1)|(1<<3)|(1<<5)|(1<<7);
}

void led_even_on(void){
	PORTA = (1<<0)|(1<<2)|(1<<4)|(1<<6);
}