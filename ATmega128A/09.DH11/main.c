/*
 * 08.DH11.c
 *
 * Created: 2026-06-26 오전 9:25:21
 * Author : kccistc
 */ 

/*
 * 04.UART.c
 *
 * Created: 2026-06-15 오전 11:36:36
 * Author : kccistc
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //sei 등의 함수
#include <stdio.h>
extern void dht11_main(void);
extern void UART0_transmit(uint8_t data);
extern void init_uart0(void);
extern init_gpio_ds1302(void)
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);



int main(void)
{
	init_uart0();
	stdout = &OUTPUT; //printf가 동작 할 수 있도록 stdout를 설정

	
	
//	led_main();
	
	while (1) 
    {
		dht11_main();
		init_gpio_ds1302();
    }
}



