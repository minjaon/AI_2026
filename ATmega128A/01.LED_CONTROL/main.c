#define F_CPU 16000000UL
#include <avr/io.h>

int main(void)
{
	DDRA |= (1 << PA0);      // LED 출력

	DDRD &= ~(1 << PD2);     // PD2 입력
	PORTD |= (1 << PD2);     // 내부 풀업 사용

	while(1)
	{
		if(!(PIND & (1 << PD2)))   // 버튼 누름
		{
			PORTA |= (1 << PA0);   // LED ON
		}
		else
		{
			PORTA &= ~(1 << PA0);  // LED OFF
		}
	}
}