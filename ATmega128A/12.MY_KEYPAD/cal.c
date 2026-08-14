#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cal.h"

#define MAX_TOKENS 20

static char num_buffer[20];        // 입력된 숫자를 임시 저장할 버퍼
static int buf_idx = 0;            // 버퍼 인덱스

static int32_t numbers[MAX_TOKENS];
static char operators[MAX_TOKENS];
static int num_count = 0;
static int op_count = 0;

void calculator_processing(uint8_t key)
{
	// 숫자 키 입력 
	if (key >= '0' && key <= '9')
	{
		if (buf_idx < 19)
		{
			num_buffer[buf_idx++] = key;
			num_buffer[buf_idx] = '\0';
			printf("%c", key);
		}
	}
	// 연산자 입력 ('+', '-', '*', '/', '=')
	else if (key == '+' || key == '-' || key == '*' || key == '/' || key == '=')
	{
		int32_t input_val = 0;
		
		// 버퍼에 숫자가 있다면 정수로 변환 후 배열에 저장
		if (buf_idx > 0)
		{
			input_val = atol(num_buffer);
			numbers[num_count++] = input_val; // 숫자 저장
			buf_idx = 0;
			num_buffer[0] = '\0';
		}
		
		if (key == '=')
		{
			if (num_count > 0 && num_count == op_count + 1)
			{
				for (int i = 0; i < op_count; )
				{
					if (operators[i] == '*' || operators[i] == '/')
					{
						int32_t res = 0;
						if (operators[i] == '*')
						{
							res = numbers[i] * numbers[i + 1];
						}
						else if (operators[i] == '/')
						{
							if (numbers[i + 1] != 0) res = numbers[i] / numbers[i + 1];
							else {
								num_count = 0; op_count = 0; return;
							}
						}
						
						
						numbers[i] = res;
						
						
						for (int j = i + 1; j < num_count - 1; j++) numbers[j] = numbers[j + 1];
						for (int j = i; j < op_count - 1; j++) operators[j] = operators[j + 1];
						
						num_count--;
						op_count--;
					}
					else
					{
						i++;
					}
				}

		
				int32_t final_result = numbers[0];
				for (int i = 0; i < op_count; i++)
				{
					if (operators[i] == '+')      final_result += numbers[i + 1];
					else if (operators[i] == '-') final_result -= numbers[i + 1];
				}

				printf(" = %ld\r\n", final_result);
			}
			else
			{
				printf(" = 0\r\n"); 
			}

			num_count = 0;
			op_count = 0;
		}
		else
		{
			// '+', '-', '*', '/' 기호가 들어오면 배열에 저장하고 화면에 출력
			if (op_count < MAX_TOKENS)
			{
				operators[op_count++] = key;
			}
			printf(" %c ", key); 
		}
	}
}