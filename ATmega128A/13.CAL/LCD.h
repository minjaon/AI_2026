/*
 * LCD.h
 *
 * Created: 2026-07-01 오전 10:40:39
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>  // PORTA PORTB PORTD... IO관련 reg가 들어 있다.
#include <util/delay.h>  // _delay_ms _delay_us 등
#include <avr/interrupt.h>

#define DATA_PORT PORTC
#define DATA_DDR  DDRC

#define CTRL_PORT PORTB
#define CTRL_DDR  DDRB

#define RS_PIN PB5
#define RW_PIN PB6
#define E_PIN  PB7

#define CLEAR_DISPLAY 0x01 
#define BIT_MODE_8 0x38
#define BIT_MODE_4 0x28

#define DISPLAY_ON_OFF_BIT 2
#define CURSOR_ON_OFF_BIT 1
#define BLINK_ON_OFF_BIT 0

extern uint8_t MODE;

