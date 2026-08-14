#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "queue.h"
#include "LCD.h"
#include "ds1307.h"
#include "button.h"
#include "keypad.h"
#include "cal.h"

#define MODE_CLOCK      0
#define MODE_CALCULATOR 1
#define MODE_SETTING    2
#define TIMEOUT_MS      60000

// 외부 장치 인터페이스 선언
extern void init_button(void);
extern int get_button(int button_num, int button_pin);
extern void init_keypad(void);
extern uint8_t keypad_scan(void);
extern void init_uart0(void);
extern void UART0_transmit(uint8_t data);
extern void init_data_time(void);
extern void init_ddr_ds1307(void);
extern void init_gpio_ds1307(void);
extern void init_ds1307(void);
extern void read_burst_ds1307(void);
void init_timer0(void); // 내부에 정의된 함수이므로 미리 선언
FILE OUTPUT = FDEV_SETUP_STREAM(UART0_transmit, NULL, _FDEV_SETUP_WRITE);
extern void calculator_processing(uint8_t key);
extern void LCD_init(void);
extern void LCD_clear(void);
extern void LCD_goto_XY(uint8_t row, uint8_t col);
extern void LCD_write_string(char *str);


// 전역 시스템 상태 제어 변수
uint8_t last_mode_button_state = 0;
uint8_t last_set_button_state = 0;
uint8_t current_mode = MODE_CLOCK;
uint8_t setting_index = 0;

volatile uint32_t keypad_counter = 0;
volatile uint32_t idle_counter = 0;

ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	if (current_mode == MODE_CALCULATOR)
	{
		idle_counter++;
		if (++keypad_counter >= 60)
		{
			keypad_counter = 0;
			uint8_t keydata = keypad_scan();
			if (keydata != 0) insert_queue(keydata);
		}
	}
}

int main(void)
{
	LCD_init(); 
	init_button(); 
	init_keypad(); 
	init_uart0();
	queue_init();
	init_data_time();
	init_ddr_ds1307();
	init_gpio_ds1307();
	init_ds1307();
	init_timer0();
	stdout = &OUTPUT; sei();
	
	LCD_write_string("System Ready...");
	 _delay_ms(1000); 
	 LCD_clear();
	
	while(1)
	{
		uint8_t current_mode_btn = get_button(BUTTON4, BUTTON4PIN);
		uint8_t current_set_btn  = get_button(BUTTON3, BUTTON3PIN);

		// [모드 전환] 시계 ↔ 계산기
		if(current_mode_btn && !last_mode_button_state && current_mode != MODE_SETTING)
		{
			current_mode = (current_mode == MODE_CLOCK) ? MODE_CALCULATOR : MODE_CLOCK;
			LCD_clear();
			if(current_mode == MODE_CALCULATOR) { calculator_processing('C'); idle_counter = 0; }
			_delay_ms(50);
		}
		last_mode_button_state = current_mode_btn;

		// [모드 전환] 시계 ↔ 세팅 설정
		if(current_set_btn && !last_set_button_state && current_mode != MODE_CALCULATOR)
		{
			if(current_mode == MODE_CLOCK) { current_mode = MODE_SETTING; setting_index = 0; }
			else if(current_mode == MODE_SETTING) { init_ds1307(); current_mode = MODE_CLOCK; }
			LCD_clear(); _delay_ms(50);
		}
		last_set_button_state = current_set_btn;

		// -------------------------------------------------------------
		// 각 모드별 핵심 구동부
		// -------------------------------------------------------------
		if(current_mode == MODE_CLOCK)
		{
			read_burst_ds1307();
			char rtc_buff[20];
			LCD_goto_XY(0, 0);
			sprintf(rtc_buff, "DATE: 20%02d-%02d-%02d", ds1307.year, ds1307.month, ds1307.date);
			LCD_write_string(rtc_buff);

			LCD_goto_XY(1, 0);
			sprintf(rtc_buff, "TIME: %02d:%02d:%02d", ds1307.hour, ds1307.minutes, ds1307.seconds);
			LCD_write_string(rtc_buff);
			_delay_ms(200);
		}
		else if(current_mode == MODE_CALCULATOR)
		{
			cal_main(&idle_counter, TIMEOUT_MS, &current_mode);
		}
		else if(current_mode == MODE_SETTING)
		{
			uint8_t btn0 = get_button(BUTTON0, BUTTON0PIN);
			uint8_t btn1 = get_button(BUTTON1, BUTTON1PIN);
			uint8_t btn2 = get_button(BUTTON2, BUTTON2PIN);
			static uint8_t l_b0=0, l_b1=0, l_b2=0;

			if (btn2 && !l_b2) { setting_index = (setting_index + 1) % 6; _delay_ms(50); }
			if (btn1 && !l_b1)
			{
				if (setting_index == 0) ds1307.year = (ds1307.year + 1) % 100;
				else if (setting_index == 1) ds1307.month = (ds1307.month % 12) + 1;
				else if (setting_index == 2) ds1307.date = (ds1307.date % 31) + 1;
				else if (setting_index == 3) ds1307.hour = (ds1307.hour + 1) % 24;
				else if (setting_index == 4) ds1307.minutes = (ds1307.minutes + 1) % 60;
				else if (setting_index == 5) ds1307.seconds = (ds1307.seconds + 1) % 60;
				_delay_ms(50);
			}
			if (btn0 && !l_b0)
			{
				if (setting_index == 0) ds1307.year = (ds1307.year == 0) ? 99 : ds1307.year - 1;
				else if (setting_index == 1) ds1307.month = (ds1307.month == 1) ? 12 : ds1307.month - 1;
				else if (setting_index == 2) ds1307.date = (ds1307.date == 1) ? 31 : ds1307.date - 1;
				else if (setting_index == 3) ds1307.hour = (ds1307.hour == 0) ? 23 : ds1307.hour - 1;
				else if (setting_index == 4) ds1307.minutes = (ds1307.minutes == 0) ? 59 : ds1307.minutes - 1;
				else if (setting_index == 5) ds1307.seconds = (ds1307.seconds == 0) ? 59 : ds1307.seconds - 1;
				_delay_ms(50);
			}
			l_b0 = btn0; l_b1 = btn1; l_b2 = btn2;

			char set_buff[20];
			LCD_goto_XY(0, 0); sprintf(set_buff, "SET: %02d-%02d-%02d", ds1307.year, ds1307.month, ds1307.date); LCD_write_string(set_buff);
			LCD_goto_XY(1, 0); sprintf(set_buff, "TIME: %02d:%02d:%02d ", ds1307.hour, ds1307.minutes, ds1307.seconds); LCD_write_string(set_buff);
			LCD_goto_XY(1, 14); const char* mode_str[] = {"YY", "MM", "DD", "HH", "MM", "SS"}; LCD_write_string((char*)mode_str[setting_index]);
			_delay_ms(100);
		}
	}
	return 0;
}

void init_timer0(void)
{
	TCNT0 = 6;
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	TCCR0 |= (1 << CS02);
	TIMSK |= (1 << TOIE0);
}