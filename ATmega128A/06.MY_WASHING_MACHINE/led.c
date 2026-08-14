/*
 * led.c
 * Optimized for State Machine Integration
 */ 

#include "led.h"
#include <avr/io.h>

void init_led(void);
void led_all_on(void);
void led_all_off(void);
void led_shift_left_on(void);
void led_shift_right_on(void);
int led_main(void);
void led_shift_right_keep_on(void);
void led_shift_left_keep_on(void);
void led_flower_on(void);
void led_flower_off(void);

#define FUNC_SU 6
// 외부 타이머 인터럽트 등에서 증가하는 1ms 카운터 변수 연동
extern volatile uint32_t ms_count; 

int func_state = 0;

// 함수 포인터 배열을 통한 시퀀스 제어
void (*fp[]) () = {
    led_shift_left_on,        // func_state = 0
    led_shift_right_on,       // func_state = 1
    led_shift_left_keep_on,   // func_state = 2
    led_shift_right_keep_on,  // func_state = 3
    led_flower_on,            // func_state = 4
    led_flower_off            // func_state = 5
};

int led_main(void) {
    init_led();
    led_all_off();
    
    while(1) {
        fp[func_state](); // 현재 상태의 패턴 함수 실행
    }
    return 0;
}

void init_led(void) {
    DDRA = 0xff;   // PORTA를 전비트 출력 모드로 설정
    PORTA = 0x00;  // 초기 상태 ALL OFF
}

void led_all_on(void) {
    PORTA = 0xff;
}

void led_all_off(void) {
    PORTA = 0x00;
}

// 1. LED가 왼쪽으로 하나씩 이동하며 ON (나머지는 OFF)
void led_shift_left_on(void) {
    static int i = 0; 
    
    if (ms_count >= 100) {
        ms_count = 0;
        PORTA = (1 << i);
        i = (i + 1) % 8;
        
        if (i == 0) { // 8개 이동 완료 시 다음 패턴으로
            func_state = (func_state + 1) % FUNC_SU;
        }
    }
}

// 2. LED가 오른쪽으로 하나씩 이동하며 ON (나머지는 OFF)
void led_shift_right_on(void) {
    static int i = 0; 
    
    if (ms_count >= 100) {
        ms_count = 0;
        PORTA = (0x80 >> i);
        i = (i + 1) % 8;
        
        if (i == 0) { // 8개 이동 완료 시 다음 패턴으로
            func_state = (func_state + 1) % FUNC_SU;
        }
    }
}

// 3. LED가 왼쪽으로 차례대로 켜진 상태를 유지(누적)
void led_shift_left_keep_on(void) {
    static int i = 0;
    
    if (ms_count >= 100) {
        ms_count = 0;
        PORTA |= (1 << i);
        i = (i + 1) % 8;
        
        if (i == 0) {
            func_state = (func_state + 1) % FUNC_SU; 
        }
    }
}

// 4. LED가 오른쪽으로 차례대로 켜진 상태를 유지(누적)
void led_shift_right_keep_on(void) {
    static int i = 0;
    
    if (ms_count >= 100) {
        ms_count = 0;
        PORTA |= (0x80 >> i);
        i = (i + 1) % 8;
        
        if (i == 0) {
            func_state = (func_state + 1) % FUNC_SU; 
        }
    }
}

// 5. 가운데(3,4번)에서 시작해 바깥쪽으로 퍼지며 켜짐 (총 4단계 필요: % 4)
void led_flower_on(void) {
    static int i = 0;
    
    if (ms_count >= 100) {
        ms_count = 0;
        if (i == 0) PORTA = 0x00; // 패턴 시작 시 초기화
        
        PORTA |= (1 << (3 - i) | 1 << (4 + i));
        i = (i + 1) % 4; // 0,1,2,3 단계 수행 후 종료
        
        if (i == 0) {
            func_state = (func_state + 1) % FUNC_SU; 
        }
    }
}

// 6. 바깥쪽(0,7번)에서 시작해 안쪽으로 순차적으로 꺼짐 (총 4단계 필요: % 4)
void led_flower_off(void) {
    static int i = 0;
    
    if (ms_count >= 100) {
        ms_count = 0;
        if (i == 0) PORTA = 0xff; // 패턴 시작 시 가득 채운 후 시작
        
        PORTA &= ~(1 << (0 + i) | 1 << (7 - i));
        i = (i + 1) % 4; // 0,1,2,3 단계 수행 후 종료
        
        if (i == 0) {
            func_state = (func_state + 1) % FUNC_SU; 
        }
    }
}