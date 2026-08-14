/*
 * 02.FND_CONTROL.c
 *
 * Created: 2026-06-12 오전 10:44:22
 * Author : kccistc
 */ 


#define F_CPU 16000000UL   // 16MHz
#include <avr/io.h>  // PORTA PORTD 등의 I/O register들이 들어 있다.
#include <util/delay.h>   // _delay_ms _delay_us 등의 함수 가 들어 있다.

#include "button.h"
#include "fnd.h"



extern int fnd_main(void);


int main(void)
{
    /* Replace with your application code */
	fnd_main();
    while (1) 
    {
    }
}

