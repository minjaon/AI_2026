#include "button.h"

void init_button(void);
int get_button(int button_num, int button_pin);

// 버튼 초기화: 5개 핀을 모두 입력(0)으로 설정
void init_button(void)
{
	// PD3, PD4, PD5, PD6, PD7 비트를 0으로 만듦
	BUTTON_DDR &= ~((1 << BUTTON0PIN) | (1 << BUTTON1PIN) | (1 << BUTTON2PIN) | (1 << BUTTON3PIN) | (1 << BUTTON4PIN));
}

int get_button(int button_num, int button_pin)
{
	static unsigned char button_status[BUTTON_NUMBER] = {
		BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE, BUTTON_RELEASE
	};
	
	int current_state;
	
	// 1. 버튼을 읽고 상태를 0 또는 1로 정규화 시킵니다.
	// 비트 AND 연산 결과가 0이 아니면(참이면) 1, 0이면 0을 대입합니다.
	current_state = (BUTTON_PIN & (1 << button_pin)) ? BUTTON_PRESS : BUTTON_RELEASE;
	
	// 2. 버튼 상태 체크
	// 처음 눌려진 상태 (기존엔 떼어져 있었고 지금은 눌림)
	if (current_state == BUTTON_PRESS && button_status[button_num] == BUTTON_RELEASE)
	{
		_delay_ms(15);  // 디바운스(noise 제거)
		button_status[button_num] = BUTTON_PRESS;
		return 0;       // 아직 완전히 눌렀다 뗀 게 아님
	}
	// 유지되다가 지금 막 떼어진 상태 (기존엔 눌려있었고 지금은 떼어짐)
	else if (current_state == BUTTON_RELEASE && button_status[button_num] == BUTTON_PRESS)
	{
		_delay_ms(15);  // 디바운스(noise 제거)
		button_status[button_num] = BUTTON_RELEASE;   // 다음 체크를 위해 상태 초기화
		return 1;       // 완전히 1번 꾹 눌렀다 뗀 상태로 인정!
	}
	
	return 0;
}