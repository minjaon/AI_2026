
#include "device_driver.h"
#include <stdio.h>

void stop(void);
void forward(void);
void reverse(void);

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("\nUART Echo-Back Test\n");

	Uart1_Init(115200);

	Macro_Set_Bit(RCC->AHB1ENR, 0); 
	Macro_Set_Bit(RCC->AHB1ENR, 2); //A,C 활성화

	Macro_Clear_Bit(GPIOA->OTYPER,0);
	Macro_Clear_Bit(GPIOA->OTYPER,1); //pushpull

	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26); //스위치 활성화
    Macro_Write_Block(GPIOA->MODER, 0xF, 0x5, 0); //모터 in1,in2에 연결

	//처음에는 stop, 스위치PC13 누를때마다 cw-ccw(방향전환할때 delay(1))
	//스위치 3초이상 누르면 off, 다시 누르면 start
	int flag = 0;
	stop();

	for(;;)
	{
	if (Macro_Check_Bit_Clear(GPIOC->IDR, 13)) //active low 눌렸을때 -> 0
        {
			int cnt = 0;
			TIM4_Repeat(1000); //1초씩 세도록 시작

			while (Macro_Check_Bit_Clear(GPIOC->IDR, 13))
            {
                if (TIM4_Check_Timeout()) 
                {
                    cnt++; 
						if(cnt >= 3)
							{
							stop();
							}
                }
            }
			TIM4_Stop();
			if (cnt < 3)
			{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                TIM2_Delay(1000);
				if(flag == 0)
                {
                    forward();
                    flag = 1;      
                }
                else
                {
                    reverse();
                    flag = 0;      
                }
			}
        }
	}
}


void stop(void)
{
	Macro_Clear_Bit(GPIOA->ODR,0);
	Macro_Clear_Bit(GPIOA->ODR,1);
}
void forward(void)
{
	Macro_Clear_Bit(GPIOA->ODR,0);
	Macro_Set_Bit(GPIOA->ODR,1);
}

void reverse(void)
{
	Macro_Set_Bit(GPIOA->ODR,0);
	Macro_Clear_Bit(GPIOA->ODR,1);
}



#endif
#if 1
#define BASE  (500) //msec

static void Buzzer_Beep(unsigned char tone, int duration)
{
	const static unsigned short tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987};

	TIM3_Out_Freq_Generation(tone_value[tone]);
	TIM2_Delay(duration);
	TIM3_Out_Stop();
}

void Main(void)
{
	Sys_Init(115200);
	printf("Buzzer Test!!\n");

	int i;
	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};
	const int song1[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2}};
	const char * note_name[] = {"C1", "C1#", "D1", "D1#", "E1", "F1", "F1#", "G1", "G1#", "A1", "A1#", "B1", "C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2"};

	TIM3_Out_Init();

	printf("%s ", note_name[C1]);
	//Buzzer_Beep(C1,N4);

	TIM3_Out_PWM_Generation(10000,80);
	TIM2_Delay(200);

	TIM3_Out_PWM_Generation(10000,20);
	TIM2_Delay(200);

	TIM3_Out_PWM_Generation(10000,50);
	TIM2_Delay(200);

	for(;;);
	
	printf("%s ", note_name[D1]);
	Buzzer_Beep(D1,N4);
	printf("%s ", note_name[E1]);
	Buzzer_Beep(E1,N4);
	printf("%s ", note_name[F1]);
	Buzzer_Beep(F1,N4);
	printf("%s ", note_name[G1]);
	Buzzer_Beep(G1,N4);
	printf("%s ", note_name[A1]);
	Buzzer_Beep(A1,N4);
	printf("%s ", note_name[B1]);
	Buzzer_Beep(B1,N4);
	printf("%s ", note_name[C2]);
	Buzzer_Beep(C2,N4);

	printf("\nSong Play\n");

	for(i=0; i<(sizeof(song1)/sizeof(song1[0])); i++)
	{
		printf("%s ", note_name[song1[i][0]]);
		Buzzer_Beep(song1[i][0], song1[i][1]);
	}
}
#endif