
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "button.h"
extern void init_speaker(void);
extern void Play_Melody_Array(int *tone, int *beats);
extern int PowerOn_Tune[];  extern int PowerOn_Beats[];
extern int OpenBuzzer_Tune[]; extern int OpenBuzzer_Beats[];
extern int DrumStop_Tune[];   extern int DrumStop_Beats[];
extern volatile int stop_flag;
void drum_washing_stop_melody(void);

// 현재 연주 중인지 기억하는 상태 변수
volatile int is_playing[4] = {0, 0, 0, 0};

//power_on_melody
void power_on_melody(void) {
    stop_flag = 0;
    is_playing[BUTTON0] = 1;
    Play_Melody_Array(PowerOn_Tune, PowerOn_Beats);
    is_playing[BUTTON0] = 0;
}

//  open_buzzer
void open_buzzer(void) {
    stop_flag = 0;
    is_playing[BUTTON1] = 1;
    Play_Melody_Array(OpenBuzzer_Tune, OpenBuzzer_Beats);
    is_playing[BUTTON1] = 0;
}

void drum_washing_stop_melody(void) {
	stop_flag = 0;
	is_playing[BUTTON3] = 1; // 재생 시작
		
	Play_Melody_Array(DrumStop_Tune, DrumStop_Beats);
		
	is_playing[BUTTON3] = 0;
	stop_flag = 0;
	OCR3A = 0;
	DDRD &= ~(1 << PORTD6); 
}
int main(void)
{
    // PORTD 버튼 입력 및 풀업 설정
    DDRD &= ~((1<<PORTD3) | (1<<PORTD4) | (1<<PORTD6));
    PORTD |= (1<<PORTD3) | (1<<PORTD4) | (1<<PORTD6);
    
    init_speaker(); 

while(1)
{
	// --- 버튼 0: Power On 
	if (get_button_pressed(BUTTON0)) {
		if (is_playing[BUTTON0]) {
			stop_flag = 1; 
			} else {
			stop_flag = 1; 
			_delay_ms(20);  
			power_on_melody();
		}
	}
	
	// --- 버튼 1: Open Buzzer 
	if (get_button_pressed(BUTTON1)) {
		if (is_playing[BUTTON1]) {
			stop_flag = 1;
			} else {
			stop_flag = 1;
			_delay_ms(20);
			open_buzzer();
		}
	}
	
	// --- 버튼 3: 세탁기 종료
	if (get_button_pressed(BUTTON3)) {
		if (is_playing[BUTTON3]) {
			stop_flag = 1;
			} else {
			stop_flag = 1;
			_delay_ms(20);
			drum_washing_stop_melody();
		}
	}
	
	_delay_ms(10);
}
	}
	
