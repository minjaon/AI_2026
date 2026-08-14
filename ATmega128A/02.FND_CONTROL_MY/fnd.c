/*
 * fnd.c
 *
 * Created: 2026-06-12 오전 10:52:39
 *  Author: kccistc
 */ 

#include "fnd.h"
#include "button.h"

int fnd_main(void);
void init_fnd(void);
void fnd_display(void);

#define  CLOCK 0
#define SEC_CLOCK 1
#define STOPWATCH 2

#define STOP 0
#define RUN 1
#define RESET 2


uint32_t ms_count =0; //ms를 재는 count uint32_t : unsigned int
uint32_t sec_count =0;//sec를 재는 count uint32_t : unsigned int
uint8_t dot_display = 0; //Dot 제어 

uint32_t sw_ms_count = 0;
uint32_t sw_sec_count = 0;

uint8_t mode = CLOCK; //시계 모드 저장(초기값:분.초 시계)
uint8_t stopwatch = STOP; //스톱워치 상태 저장(초기값:정지)
uint8_t circle_index = 0; //초시계 원 위치 저장
uint32_t circle_ms = 0; //초시계 원 ms 카운터


int fnd_main(void){
	
	init_fnd();
	init_button();
	
	while(1){
		
		if (get_button(0, BUTTON0PIN)){
			mode = (mode +1)%3;
		}
		
		if(mode == STOPWATCH){
			if(get_button(1,BUTTON1PIN)){
				if(stopwatch == STOP){
					stopwatch = RUN;
				}
				else{
					stopwatch = STOP;
				}
			}
		if(get_button(2,BUTTON2PIN)){
			if(stopwatch == STOP){
				stopwatch = RESET;
				sw_ms_count = 0;
				sw_sec_count = 0;
				
			}
			else if(stopwatch == RESET){
				stopwatch = RUN;
			}
		}
	}
		fnd_display();
		_delay_ms(1);
		ms_count++;
		if (mode == SEC_CLOCK) {
			circle_ms++;
			if (circle_ms >= 125) {
				circle_ms = 0;
				circle_index = (circle_index + 1) % 8; // 8칸 순환
			}
		}
		if(ms_count % 500 == 0){
		dot_display = !dot_display;
		}

		if(ms_count >= 1000){ // 1000ms --> 1sec
			ms_count = 0;
			sec_count++;
		}
		if (mode == STOPWATCH && stopwatch == RUN){
			sw_ms_count++;
			if(sw_ms_count >=1000){
				sw_ms_count =0;
				sw_sec_count ++;
				
			}
		}
		
	}
	return 0;
}

void init_fnd(void){
	
	FND_DATA_DDR = 0xff; //출력 모드로 설정
	FND_DIGIT_DDR |= 1 << FND_DIGIT_D1 | 1 << FND_DIGIT_D2 | 1 << FND_DIGIT_D3 | 1 << FND_DIGIT_D4; //자리수 선택
	FND_DATA_PORT = ~0xff;
}

void fnd_display(void) {
	//0 1 2 3 4 5 6 7 8 9 .
	uint8_t fnd_font[] = { ~0xc0, ~0xf9, ~0xa4, ~0xb0, ~0x99, ~0x92, ~0x82, ~0xd8, ~0x80, ~0x98, ~0x7f }; //~0x7f (dot)
	uint8_t circle_left[]  = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x08, 0x10, 0x20 }; //초시계 원그리기위한 위치 정의 
	uint8_t circle_right[] = { 0x00, 0x01, 0x02, 0x04, 0x08, 0x00, 0x00, 0x00 };
		
	static int digit_select = 0;

	uint32_t display = 0;

	switch (mode) {
		case CLOCK: 
		display = sec_count;

		switch (digit_select) {
			case 0: FND_DIGIT_PORT = ~0x80; FND_DATA_PORT = fnd_font[display % 10]; break;
			case 1: FND_DIGIT_PORT = ~0x40; FND_DATA_PORT = fnd_font[(display / 10) % 6]; break; 
			case 2: FND_DIGIT_PORT = ~0x20;
			FND_DATA_PORT = dot_display ? (fnd_font[(display / 60) % 10] | ~0x7f) : fnd_font[(display / 60) % 10]; break; 
			case 3: FND_DIGIT_PORT = ~0x10; FND_DATA_PORT = fnd_font[(display / 600) % 6]; break; 
		}
		break;

		case SEC_CLOCK:
		display = sec_count % 60; 
		switch (digit_select) {
			case 0: FND_DIGIT_PORT = ~0x80; FND_DATA_PORT = fnd_font[display % 10]; break;
			case 1: FND_DIGIT_PORT = ~0x40; FND_DATA_PORT = fnd_font[(display / 10) % 6]; break;
			case 2: FND_DIGIT_PORT = ~0x20; FND_DATA_PORT = circle_right[circle_index] | ~0x7f; break;
			case 3: FND_DIGIT_PORT = ~0x10; FND_DATA_PORT = circle_left[circle_index]; break;
		}
		break;

		case STOPWATCH:
		switch (digit_select) {
			case 0: FND_DIGIT_PORT = ~0x80; FND_DATA_PORT = fnd_font[(sw_ms_count / 10) % 10]; break;
			case 1: FND_DIGIT_PORT = ~0x40; FND_DATA_PORT = fnd_font[(sw_ms_count / 100) % 10]; break;
			case 2: FND_DIGIT_PORT = ~0x20; FND_DATA_PORT = fnd_font[(sw_sec_count % 60) % 10] | ~0x7f; break;
			case 3: FND_DIGIT_PORT = ~0x10; FND_DATA_PORT = fnd_font[((sw_sec_count % 60) / 10) % 6];  break; }
		break;
	}
		digit_select = (digit_select + 1) % 4;
		}