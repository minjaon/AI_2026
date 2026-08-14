#define F_CPU 16000000UL  // 16MHz
#include <avr/io.h>
#include <util/delay.h>

#define  BUTTON_DDR  DDRD
#define  BUTTON_PIN  PIND

// 하드웨어 실제 핀 번호 (PORTD 3, 4, 5, 6, 7)
#define  BUTTON0PIN 0
#define  BUTTON1PIN 4
#define  BUTTON2PIN 5
#define  BUTTON3PIN 6
#define  BUTTON4PIN 7    // 5번째 버튼 추가

// 버튼의 가상 인덱스 (배열 access용)
#define  BUTTON0   0
#define  BUTTON1   1
#define  BUTTON2   2
#define  BUTTON3   3
#define  BUTTON4   4   // 5번째 버튼 인덱스 추가
#define  BUTTON_NUMBER  5   // 버튼 갯수를 5개로 변경

#define  BUTTON_PRESS   1   // Active-High (누르면 1)
#define  BUTTON_RELEASE 0   // 떼면 0