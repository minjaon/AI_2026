/*
 * LCD.c
 *
 * Created: 2026-07-01 오전 10:40:49
 * Author: kccistc
 */ 

#include "LCD.h"

// 함수 프로토타입 선언
void LCD_init(void);
void LCD_pulse_enable(void);
void LCD_write_data(uint8_t data);
void LCD_write_command(uint8_t command);
void LCD_clear(void);
void LCD_write_string(char *string);
void LCD_goto_XY(uint8_t row, uint8_t col);
void LCD_main(void);

void LCD_main(void)
{
    LCD_init();
    LCD_write_string("hello lcd");
    _delay_ms(1000);
    LCD_clear();
    
    LCD_goto_XY(0,0);
    LCD_write_data('1');
    LCD_goto_XY(0,5);
    LCD_write_data('2');
    LCD_goto_XY(1,0);
    LCD_write_data('3');
    LCD_goto_XY(1,5);
    LCD_write_data('4');
}

void LCD_init(void)
{
    // 1. 전원 인가 후 LCD 내부 리셋 대기 (데이터시트 기준 15ms 이상 필수)
    _delay_ms(50);      

    // 2. 입출력 방향 설정 (DDR)
    DATA_DDR |= 0xF0;   // PORTC의 상위 4비트(PC4~7)를 출력으로 설정
    CTRL_DDR |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN); // PORTB의 PB5~7을 출력으로 설정

    // 3. 포트 초기 상태 출력 청소
    DATA_PORT &= 0x0F;     // PC4~7에 0 출력
    CTRL_PORT &= ~((1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN)); // PB5~7에 0 출력

    // R/W 핀은 항상 쓰기 모드(LOW)로 고정
    CTRL_PORT &= ~(1 << RW_PIN);
    // 명령어 모드로 진입하기 위해 RS = 0 설정
    CTRL_PORT &= ~(1 << RS_PIN); 

    // 4. [데이터시트 4비트 초기화 시퀀스 필수 단계]
    // 첫 번째 0x30 전송 (아직 4비트 인식이 안 되므로 상위 4비트로만 1번 펄스)
    DATA_PORT = (DATA_PORT & 0x0F) | 0x30;
    LCD_pulse_enable();
    _delay_ms(5);       // 4.1ms 이상 대기

    // 두 번째 0x30 전송
    DATA_PORT = (DATA_PORT & 0x0F) | 0x30;
    LCD_pulse_enable();
    _delay_us(150);     // 100us 이상 대기

    // 세 번째 0x30 전송
    DATA_PORT = (DATA_PORT & 0x0F) | 0x30;
    LCD_pulse_enable();
    _delay_us(150);

    // 다섯 번째: 드디어 "4비트 모드로 동작해라!" 명령어 (0x20) 전송
    DATA_PORT = (DATA_PORT & 0x0F) | 0x20;
    LCD_pulse_enable();
    _delay_ms(2);       

    // ------------------------------------------------------------------
    // ★ 이 단계 이후부터만 상위/하위 4비트를 쪼개서 보내는 하단의 함수들 작동 가능!
    // ------------------------------------------------------------------
    
    // Function Set: 4비트 모드, 2라인 디스플레이, 5x8 폰트 설정 (0x28)
    LCD_write_command(BIT_MODE_4);   
    _delay_ms(2);

    // Display On/Off: 화면 On, 커서 Off, 블링크 Off (0x0C)
    LCD_write_command(0x0C);
    _delay_ms(2);

    // 화면 Clear (0x01)
    LCD_clear();         

    // Entry Mode Set: 글자 쓰면 커서가 자동으로 우측 이동 (0x06)
    LCD_write_command(0x06);
    _delay_ms(2);
}

void LCD_pulse_enable(void)
{
    CTRL_PORT |= (1 << E_PIN);   // E(PB7) High
    _delay_us(2);                // 신호 래치를 위해 살짝 대기
    CTRL_PORT &= ~(1 << E_PIN);  // E(PB7) Low
    _delay_us(50);               // LCD 내부 처리 대기 시간
}

void LCD_write_data(uint8_t data)
{
    // 데이터 전송 모드이므로 RS(PB5) = 1 설정
    CTRL_PORT |= (1 << RS_PIN);
    _delay_us(1);
    
    // [상위 4비트 전송] 기존 하위 4비트(PC0~3) 상태 유지하면서 상위 4비트만 탑재
    DATA_PORT = (DATA_PORT & 0x0F) | (data & 0xF0);
    LCD_pulse_enable();

    // [하위 4비트 전송] 하위 4비트를 왼쪽으로 4칸 밀어서 PORTC 상위에 장착
    DATA_PORT = (DATA_PORT & 0x0F) | ((data << 4) & 0xF0);
    LCD_pulse_enable();
    
    _delay_ms(2); 
}

void LCD_write_command(uint8_t command)
{
    // 명령어 전송 모드이므로 RS(PB5) = 0 설정
    CTRL_PORT &= ~(1 << RS_PIN);     
    _delay_us(1);

    // [상위 4비트 전송] PORTC 상위 4비트에 명령어 상위 4비트 탑재
    DATA_PORT = (DATA_PORT & 0x0F) | (command & 0xF0);
    LCD_pulse_enable();              

    // [하위 4비트 전송] 명령어 하위 4비트를 밀어서 PORTC 상위에 탑재
    DATA_PORT = (DATA_PORT & 0x0F) | ((command << 4) & 0xF0);
    LCD_pulse_enable();              
    
    _delay_ms(2); 
}

void LCD_clear(void)
{
    LCD_write_command(CLEAR_DISPLAY);
    _delay_ms(3); 
}

void LCD_write_string(char *string)
{
    for(uint8_t i = 0; string[i]; i++) {
        LCD_write_data(string[i]);      
    }
}

void LCD_goto_XY(uint8_t row, uint8_t col)
{
    col %= 16;      
    row %= 2;       

    uint8_t address = (0x40 * row) + col;
    uint8_t command = 0x80 + address;  

    LCD_write_command(command);     
}