#include "device_driver.h"

void Key_Poll_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 2); 
    Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 26);
}

int Key_Get_Pressed(void) //int면 return 넣고, void면 없어도 ㄱㅊ
{
    if (Macro_Check_Bit_Clear(GPIOC->IDR, 13)) 
	{
		return 1;
	}
    else return 0;
}

void Key_Wait_Key_Pressed(void) //키 눌릴때까지 대기 눌리면 return
{
    for(;;){
        if (Macro_Check_Bit_Clear(GPIOC->IDR, 13)){
            return;
        }
    }
}

void Key_Wait_Key_Released(void) //키 눌리기 전까지 대기 아니면 return
{
    for(;;) {
        if (Macro_Check_Bit_Set(GPIOC->IDR, 13))
        return;
    }
}