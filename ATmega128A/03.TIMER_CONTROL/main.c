/*
 * 03.TIMER_CONTROL.c
 *
 * Created: 2026-06-15 오전 11:36:36
 * Author : kccistc
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  //sei 등의 함수

extern int led_main(void);
void init_timer0(void);

volatile uint32_t msec_count = 0; //volatile 최적화 방지

/*
ISR(Interrupt Service Routine) : 인터럽트 처리함수 ISR로 시작
TIMER0_OVF_vect : Timer 0 Overflow INT 가 발생되면 이곳으로 진입함
250개의 펄스를 count (1ms) 하면 이곳으로 자동 진입한다
ISR은 가능한 짧게 작성한다
*/

ISR(TIMER0_OVF_vect) //타이머0의 오버플로우 인터럽트 함수
{
	TCNT0 = 6; //한번 인터럽트 하고나면 TCNT0가 0이 되므로 재정의 
	msec_count++; //1ms count
}

int main(void)
{
	init_timer0();
	led_main();
	
	while (1) 
    {
    }
}


/*
1.timer0을 초기화 한다.
AVR에서 8bit timer 0/2 두개가 있는데, 0번을 초기화 한다.
임베디드 쪽에서 가장 신경써야할 부분이 초기화 하는것이다.
초기화가 잘못되면 이후가 다 꼬인다.

2.8bit를 가지고 1ms를 측정하는 timer/counter를 만들고자 한다.
 2.1 분주비 설정 (64분주)
     16000000Hz /64 ==> 250,000Hz 
 2.2 1주기가 잡아먹는 시간 계산
	 T = 1/f = 1/250,000 ==> 0.000004sec (4us) ==> 0.004ms
 2.3 8bit 가지고 count 하는 시간을 계산 (8bit timer OV(Overflow)) 
     0.004ms x 256개 ==>  0.001024 sec (1.024ms)
	 0.004ms X 250개 ==> 0.001sec(1ms)
*/

void init_timer0(void){
	
	TCNT0 = 6;  //TCNT0 6~256 : 250개 펄스 count 하기 위해 
	
	TCCR0 |= 1 << CS02 | 0 << CS01 | 0 << CS00; //64분주
	TIMSK |= 1 << TOIE0; //TIMER0 Overflow INT 
	sei(); // 전역(대문)interrupt 허용
}
