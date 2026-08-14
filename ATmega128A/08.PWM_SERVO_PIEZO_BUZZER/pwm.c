#include <avr/io.h>         
#include "button.h"



// FND에 상태를 보내기 위한 외부 전역 변수 공유 선언
extern volatile char car_current_status;

/*
   PB6 : OC1B : NOT USE
*/

void init_timer1_pwm(void);
int servo_motor_main(void);

void init_timer1_pwm(void)
{
    // 모드 14 : 고속 PWM 모드 사용 하겠다. timer1 (P327 표14-5)
    TCCR1A |= 1 << WGM11;    // TOP --> ICR1으로 설정
    TCCR1B |= 1 << WGM13 | 1 << WGM12;

    // 비반전 모드 top : ICR1 비교일치 값 (PWM) 지정 OCR1A P350 표15-7
    // 비교 일치 발생시 OCR1 출력핀은 LOW로 바뀌고 BOTTOM에서 HIGH로 바뀐다.
    TCCR1A |= 1 << COM1A1;

    //---- 분주비 설정 ---------
    // 분주비 8를 하는 이유 16MHz를 가지고 50Hz의 주파수를 바로 만들지 못한다.
    // 16000000Hz / 8 --> 2MHz
    // T=1/f 1/16000000Hz ==> 0.0000000625sec x 8 ==> 0.0000005sec (0.5us)
    // T=1/f 1/2000000Hz ==> 0.0000005sec (0.5us)
    // 16bit 까지 count할 수 있는 최대 0xffff(65535)
    // 0.0000005sec x 65535 ==> 0.0327675sec (32.7675ms) 32.7675ms 마다 timer INT
    // 20ms 길이(Duty)를 갖는 것을 만든다고 한다면
    // 0.0000005sec x 40000개 ==> 0.02sec (20ms)
    // TCNT3 : 0~255(0x00ff) 까지 count한 후 0으로 다시 돌아 간다.
    
    // 💡 오타 수정: CS12와 1 사이에 | 연산자 추가 (numeric constant 에러 해결)
    TCCR1B &= ~(1 << CS12 | 1 << CS11 | 1 << CS10);   // all reset (기존의 값을 무시)
    TCCR1B |= 1 << CS11;   // 분주비 8

    ICR1 = 40000-1 ; //0.0000005sec x 40000개 ==> 20ms TOP 값
}

int servo_motor_main(void)
{
    //servo motor가 PB5를 출력으로 설정
    DDRB |= 1 << 5 ;
    init_timer1_pwm();
    
    //0(1ms) --> 90(1.5ms) --> 180(2ms)
    while(1)
    {
        //1ms : 2000개 count
        //4000(20ms) / 20 --> 1ms(2000)
        
        OCR1A = 1000;
        _delay_ms(1000);
        
        //1.5ms : 3000개 count 
        OCR1A = 2900;
        _delay_ms(1000);
        
        //2ms : 4000개 count
        OCR1A = 4900;
        _delay_ms(1000);
    }
    return 0;
}