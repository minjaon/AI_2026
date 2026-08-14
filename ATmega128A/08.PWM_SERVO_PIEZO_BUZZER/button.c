#include "button.h"
#include <util/delay.h>

static uint8_t prev_button_state[BUTTON_NUMBER] = {0, 0, 0, 0};

int get_button_pressed(int btn_num) {
	uint8_t current_state = 0;
	
	switch(btn_num) {
		case BUTTON0: current_state = !(PIND & (1 << PORTD3)); break;
		case BUTTON1: current_state = !(PIND & (1 << PORTD4)); break;
		case BUTTON2: current_state = !(PIND & (1 << PORTD5)); break;
		case BUTTON3: current_state = !(PIND & (1 << PORTD6)); break;
	}
	

	if (current_state) {
		if (!prev_button_state[btn_num]) {
			_delay_ms(30); 
			prev_button_state[btn_num] = 1;
			return 1; 
		}
		} else {
		prev_button_state[btn_num] = 0; 
	}
	
	return 0;
}