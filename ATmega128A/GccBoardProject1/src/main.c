#define F_CPU 16000000UL // ATmega128A 클럭 주파수 (16MHz 설정)
#include <avr/io.h>
#include <util/delay.h>

// 모터 제어 핀 정의 (PORTA 사용)
#define MOTOR_PORT PORTA
#define MOTOR_DDR  DDRA

// 왼쪽 모터 (PA0, PA1) / 오른쪽 모터 (PA2, PA3)
#define GO_FORWARD  0x05  // 0000 0101 (PA0, PA2 HIGH -> 전진)
#define GO_BACKWARD 0x0A  // 0000 1010 (PA1, PA3 HIGH -> 후진)
#define STOP        0x00  // 0000 0000 (모두 LOW -> 정지)

void init_motors() {
	MOTOR_DDR = 0x0F;     // PA0, PA1, PA2, PA3 핀을 출력(1)으로 설정
	MOTOR_PORT = STOP;    // 초기 상태는 정지
}

int main(void) {
	init_motors();        // 모터 초기화

	while (1) {
		// 1. 전진 (2초)
		MOTOR_PORT = GO_FORWARD;
		_delay_ms(2000);

		// 2. 정지 (1초)
		MOTOR_PORT = STOP;
		_delay_ms(1000);

		// 3. 후진 (2초)
		MOTOR_PORT = GO_BACKWARD;
		_delay_ms(2000);

		// 4. 정지 (1초)
		MOTOR_PORT = STOP;
		_delay_ms(1000);
	}

	return 0;
}