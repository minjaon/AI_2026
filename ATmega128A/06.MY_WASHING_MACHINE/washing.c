#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "washing.h"

void Washing_UpdateTimer(void);
void Washing_init(void);
void Washing_Process(int button_num, int button_pin);

extern int get_button(int button_num, int button_pin);
extern void init_fnd(void);
extern void init_timer3_pwm(void);
extern void init_motor_driver(void);


extern volatile uint8_t mode;        
extern volatile uint8_t stopwatch;   
extern volatile uint32_t sw_ms_count;
extern volatile uint32_t sw_sec_count; 
extern volatile uint8_t circle_index;  
extern volatile uint32_t sec_count;   

#define LED_PORT PORTA
#define LED_DDR  DDRA

static int currentState = STANDBY;       // 현재 상태 (대기, 세탁, 헹굼)
static uint32_t washTime = 0;            // 세탁 시간 (분 단위)
static uint32_t rinseTime = 0;           // 헹굼 시간 
static uint32_t spinTime = 0;            // 탈수 시간 


static uint8_t is_paused = 0;


static void Enter_State(int newState) {
	currentState = newState; // 상태 업데이트
	
	switch (currentState) {
		case STANDBY: // 대기 상태
			OCR3C = 0;           // 모터 정지
			stopwatch = 0;       // 타이머 정지
			mode = 2;            // STOPWATCH 모드로 초기 세팅
			LED_PORT &= ~0x07;   // 세탁, 헹굼, 탈수 LED 전부 끄기
			
			// 시간 변수들 전부 0
			sw_sec_count = 0;
			sw_ms_count = 0;
			sec_count = 0;
			is_paused = 0;       // 일시정지 해제
			break;
		
		case SET_WASH_TIME: //세탁 시간
			mode = 2;            
			sw_sec_count = washTime * 60;
			break;

		case SET_RINSE_TIME: //헹굼 시간
			mode = 2;
			sw_sec_count = rinseTime * 60;
			break;

		case SET_SPIN_TIME: //탈수 시간
			mode = 2;
			sw_sec_count = spinTime * 60;
			break;

		case STATE_WASHING: //세탁 시작
			LED_PORT = (LED_PORT & ~0x07) | (1 << 0); // 첫 번째 LED
			PORTF &= ~((1 << 6) | (1 << 7));          // 
			PORTF |= (1 << 6);                        // 정방향 신호 
			OCR3C = 150;                              // 모터 속도 설정
			
			// 분 단위 설정을 초 단위로 변경
			sw_sec_count = washTime * 60;         
			sw_ms_count = 0;
			stopwatch = 1;                            
			
			mode = 1;                                 // 분 자리에 원 애니메이션 
			is_paused = 0;                            
			break;

		case STATE_RINSING: // 헹굼 시작
			LED_PORT = (LED_PORT & ~0x07) | (1 << 1); // 두 번째 LED
			PORTF &= ~((1 << 6) | (1 << 7));
			PORTF |= (1 << 6);                        // 정방향 신호
			OCR3C = 70;                               // 약한 속도로 설정
			
			sw_sec_count = rinseTime * 60;
			sw_ms_count = 0;
			stopwatch = 1;                            // 인터럽트
			
			mode = 1;                                 
			is_paused = 0;
			break;

		case STATE_SPINNING: // 탈수 시작
			LED_PORT = (LED_PORT & ~0x07) | (1 << 2); // 세 번째 LED
			PORTF &= ~((1 << 6) | (1 << 7));
			PORTF |= (1 << 7);                        // 강력 역방향
			OCR3C = 254;                              // 모터 최대치
			
			sw_sec_count = spinTime * 60;
			sw_ms_count = 0;
			stopwatch = 1;                            // 인터럽트

			mode = 1;                                 
			is_paused = 0;
			break;

		default:
			OCR3C = 0; 
			break;
	}
}


void Washing_init(void){
	init_fnd();           
	init_button();        
	init_timer3_pwm();    
	init_motor_driver();  
	
	LED_DDR |= 0x07;      
	LED_PORT &= ~0x07;    
	
	// 시간 변수 리셋
	washTime = 0;
	rinseTime = 0;
	spinTime = 0;
	is_paused = 0;
	
	Enter_State(STANDBY);
}


void Washing_Process(int button_num, int button_pin){
	switch (currentState){
		case STANDBY: // 대기 상태
			if(button_num == 1){          // 버튼 1 -> 세탁 시간
				Enter_State(SET_WASH_TIME);
			}
			else if (button_num == 0){     // 버튼 0 -> 세탁 시작
				if (washTime > 0)          Enter_State(STATE_WASHING);  
				else if (rinseTime > 0)    Enter_State(STATE_RINSING);  
				else if (spinTime > 0)     Enter_State(STATE_SPINNING); 
			}
			break;

		case SET_WASH_TIME: // 세탁 시간 설정
			if (button_num == 2) {         // 버튼 2를 누르면 1분씩 증가
				if (washTime < 99) washTime += 1;
				sw_sec_count = washTime * 60;
			}
			else if (button_num == 1) {    // 버튼 1을 누르면 다음 단계 헹굼 
				Enter_State(SET_RINSE_TIME);
			}
			break;

		case SET_RINSE_TIME: // 헹굼 시간 설정 
			if (button_num == 2) {         // 버튼 2를 누르면 1분씩 증가
				if (rinseTime < 99) rinseTime += 1;
				sw_sec_count = rinseTime * 60;
			}
			else if (button_num == 1) {    // 버튼 1을 누르면 탈수 설정
				Enter_State(SET_SPIN_TIME);
			}
			break;

		case SET_SPIN_TIME: // 탈수 시간 설정 
			if (button_num == 2) {         // 버튼 2를 누르면 1분씩 증가
				if (spinTime < 99) spinTime += 1;
				sw_sec_count = spinTime * 60;
			}
			else if (button_num == 1) {    // 버튼 1을 누르면 대기 상태
				Enter_State(STANDBY);
			}
			break;
		
		case STATE_WASHING:
		case STATE_RINSING:
		case STATE_SPINNING:
		
			// 작동 중 버튼 0 -> 일시정지
			if (button_num == 0) {
				if (is_paused == 0) { // 실행 중 -> 일시정지
					is_paused = 1;   
					OCR3C = 0;        // 모터 정지
					stopwatch = 0;    // 타이머 카운터 일시 정지
				} else {              // 일시정지 중 -> 재개
					is_paused = 0;   
					stopwatch = 1;    // 타이머 가동
					
					// 현재 상태 확인 및 모터 
					if (currentState == STATE_WASHING)  OCR3C = 150;
					if (currentState == STATE_RINSING)  OCR3C = 70;
					if (currentState == STATE_SPINNING) OCR3C = 254;
				}
			}
			break;
	}
}


void Washing_UpdateTimer(void) {
	// 시간 설정중
	if (currentState == SET_WASH_TIME || currentState == SET_RINSE_TIME || currentState == SET_SPIN_TIME) {
		uint32_t current_set_val = 0;
		if (currentState == SET_WASH_TIME)  current_set_val = washTime;
		if (currentState == SET_RINSE_TIME) current_set_val = rinseTime;
		if (currentState == SET_SPIN_TIME)  current_set_val = spinTime;

		sw_ms_count = (current_set_val % 10) * 10 + ((current_set_val / 10) % 10) * 100;
		sw_sec_count = 0; // 왼쪽 두 자리 숨기기
		return;         
	}

	// 일시정지
	if (is_paused == 1) {
		return;           // 애니메이션 루프가 안 돌게 함
	}

	// 세탁, 헹굼, 탈수
	if (currentState == STATE_WASHING || currentState == STATE_RINSING || currentState == STATE_SPINNING) {
		

		// 모터의 회전 방향 판단
		uint8_t is_forward = 1; 
		
		if (currentState == STATE_WASHING) {
			// 4초 주기로 정방향 2초, 역방향 2초
			if ((sw_sec_count % 4) >= 2) {
				PORTF = (PORTF & ~((1 << 6) | (1 << 7))) | (1 << 6); // 정방향 핀 ON
				is_forward = 1;
			} else {
				PORTF = (PORTF & ~((1 << 6) | (1 << 7))) | (1 << 7); // 역방향 핀 ON
				is_forward = 0;
			}
		}
		else if (currentState == STATE_RINSING) {
			is_forward = 1; // 헹굼은 정방향 회전
		}
		else if (currentState == STATE_SPINNING) {
			is_forward = 0; // 탈수는 역방향 회전
		}

		// 모터가 도는 방향에 맞춰 FND의 원 애니메이션 방향 제어
		static uint16_t anim_ms = 0;
		anim_ms++;
		if (anim_ms >= 125) { // 125ms 주기로 원을 한 칸씩 돌립니다.
			anim_ms = 0;
			if (is_forward) {
				circle_index = (circle_index + 1) % 8; // 모터가 정방향이면 시계 방향 회전
			} else {
				if (circle_index == 0) circle_index = 7;
				else circle_index--;                   // 모터가 역방향이면 반시계 방향 회전
			}
		}

		// 오른쪽 두 자리에 'sec_count' 변수
		if (sw_sec_count >= 60) {
			sec_count = sw_sec_count / 60; // 1분 이상 남았을 때는 '분'
		} else {
			sec_count = sw_sec_count;      // 1분 미만이 되면 '초'
		}

		// 카운터가 0이 되면 다음 공정
		if (sw_sec_count == 0 && sw_ms_count == 0) {
			if (currentState == STATE_WASHING) {
				if (rinseTime > 0)      Enter_State(STATE_RINSING);   
				else if (spinTime > 0)  Enter_State(STATE_SPINNING); 
				else                    Enter_State(STANDBY);        
			}
			else if (currentState == STATE_RINSING) {
				if (spinTime > 0)       Enter_State(STATE_SPINNING); 
				else                    Enter_State(STANDBY);         
			}
			else if (currentState == STATE_SPINNING) {
				washTime = 0; rinseTime = 0; spinTime = 0;
				Enter_State(STANDBY);
			}
		}
	}
}