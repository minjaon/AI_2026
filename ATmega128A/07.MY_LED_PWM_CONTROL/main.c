/*
 * 07.MY_LED_PWM_CONTROL.c
 *
 * Created: 2026-06-18 오전 10:25:49
 * Author : kccistc
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>

#include <util/delay.h>
#include <avr/interrupt.h>  //sei 등의 함수



// void turn_on_LED_in_PWM_manner(int dim){
// 	
// 	int i;
// 	PORTA = 0xFF;
// 	for(i = 0; i <256; i++){
// 		if(i>dim) PORTA = 0x00;
// 		_delay_us(LED_TIME);
// 	}
// }

int main(void)
{
#if 1

	DDRB |= (1 << PB5) | (1 << PB6);
	
	TCCR1A |= (1 << WGM10);
	
	TCCR1A |= (1 << COM1A1);
	TCCR1A |= (1 << )
	
#else
	int dim = 0;
	int direction = 1;
	
	DDRB |= (1 << PB4);
	
	TCCR0 |= (1 << WGM01) | (1 << WGM00);
	TCCR0 |= (1 << COM01);
	TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);
	
    while (1) 
    {
		OCR0 = dim;
		_delay_ms(10);
		
		dim += direction;
		
		if(dim == 0) direction = 1;
		if(dim == 255) direction = -1;
#endif		
    }
	return 0;
}

