#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "button.h"

extern void make_trigger(void);
extern int led_main(void);
extern void Washing_init(void);
extern void Washing_Process(int button_num, int button_pin);
extern void Washing_UpdateTimer(void);
extern void fnd_display(void);
extern void UART0_transmit(uint8_t data);
extern void init_uart0(void);
extern void init_led(void);
extern void init_button(void);
void init_timer0(void);

extern volatile char scm[50];
extern void pc_command_processing(void);
extern void init_ultrasonic(void);
extern void ultrasonic_processing(void);
extern void init_timer3_pwm(void);
extern void init_motor_driver(void);
extern void dcmoter_pwm_control_main(void);
extern int get_button(int button_num, int button_pin);

extern volatile uint32_t sw_ms_count;
extern volatile uint8_t  stopwatch;
extern volatile uint32_t sw_sec_count;

FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);

volatile uint32_t msec_count = 0;
volatile int ultrasonic_check_time = 0;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6; 
	msec_count++;             
	ultrasonic_check_time++;  
	
	if (stopwatch) {
		sw_ms_count++;
		if (sw_ms_count >= 1000) {
			sw_ms_count = 0;
			if (sw_sec_count > 0) {
				sw_sec_count--;
			}
		}
	}
}


int main(void)
{

	init_led();           
	init_timer0();        
	init_uart0();         
	init_button();        
	init_motor_driver(); 
	init_timer3_pwm();   
	Washing_init();      
	
	stdout = &OUTPUT;    
	sei();              
	

	while (1)
	{
		
		if (get_button(0, BUTTON0PIN)) {
			Washing_Process(0, BUTTON0PIN); 
		}
		else if (get_button(1, BUTTON1PIN)) {
			Washing_Process(1, BUTTON1PIN); 
		}
		else if (get_button(2, BUTTON2PIN)) {
			Washing_Process(2, BUTTON2PIN); 
		}
		else if (get_button(3, BUTTON3PIN)) {
			Washing_Process(3, BUTTON3PIN); 
		}


		Washing_UpdateTimer();
		

		fnd_display();
		
		_delay_ms(1);
	}
}

void init_timer0(void){
	TCNT0 = 6;                                  // 시작 레지스터 값 세팅
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; // 분주비를 64로 설정 
	TIMSK |= 1 << TOIE0;                        // 타이머0 오버플로우 인터럽트
}