#include "device_driver.h"

void SysTick_Run(unsigned int msec)
{
	// Timer 설정 : 인터럽트 발생 안함, clock source는 HCLK/8, Timer 정지 -> 전부 0

	
	SysTick->LOAD = ((HCLK/(8.*1000.))*msec+0.5);
	SysTick->CTRL = (0<<2)|(0<<1)|(0<<0);
	SysTick->VAL = 0;

	// 주어진 msec 값 만큼의 msec를 count하는 초기값 설정 (LOAD) -> 12Mhz(기준펄스), 1msec에 12000펄스

	//Macro_Write_Block(SysTick->LOAD, 0xffffff, )

	// VAL 레지스터 값 초기화(0) 및 COUNTFLAG Clear -> SysTick->VAL(아무값)/LOAD/CTRL 

	// Timer Start (시작이 되면 자동으로 LOAD의 값을 VAL로 가져간다) -> ENLAble에 1
	Macro_Set_Bit(SysTick->CTRL, 0);

}

int SysTick_Check_Timeout(void)//flag확인
{
	// Timer의 Timeout이 발생하면(flag = 1) 참(1)리턴, 아니면 거짓(0) 리턴
	if(Macro_Check_Bit_Set(SysTick->CTRL, 16))
	{
		return 1;
	}
	else
	{
		return 0;
	}


}

unsigned int SysTick_Get_Time(void) //val
{
	// Timer의 현재 count 값 리턴
	return SysTick->VAL;

}

unsigned int SysTick_Get_Load_Time(void)//load
{
	return SysTick->LOAD;
	// Timer에 설정된 초기값을 리턴

}

void SysTick_Stop(void)
{
	Macro_Clear_Bit(SysTick->CTRL, 0);
	// Timer Stop

}
