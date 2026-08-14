/*
 * ultrasonic.h
 *
 * Created: 2026-06-17 오후 1:32:39
 *  Author: kccistc
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //sei 등의 함수

#define TRIG_DDR DDRG
#define TRIG_PORT PORTG
#define TRIG_PIN 4

#define ECHO_DDR DDRE
#define ECHO_PORT PINE //extern INT 4
#define ECHO_PIN 4