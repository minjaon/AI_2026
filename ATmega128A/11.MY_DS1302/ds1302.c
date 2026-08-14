/*
 * ds1302.c
 */ 

#include "ds1302.h"

void clock_ds1302(void);
void init_data_time(t_ds1302 *p_ds1302);
void init_gpio_ds1302(void);
void init_ddr_ds1302(void);
void init_ds1302(t_ds1302 *p_ds1302);
void ds1302_main(void);
void write_ds1302(uint8_t addr, uint8_t data);

uint8_t dec2bcd(uint8_t data);
uint8_t read_ds1302(uint8_t addr);
uint8_t bcd2dec(uint8_t data);

void tx_ds1302(uint8_t data);
void rx_ds1302(uint8_t *pdata8bits);
void read_time_ds1302(t_ds1302 *p_ds1302);
void read_date_ds1302(t_ds1302 *p_ds1302);


void ds1302_main(void)
{
	t_ds1302 my_ds1302;
	int delay_count = 0; 
	
	init_data_time(&my_ds1302);
	init_ddr_ds1302();
	init_gpio_ds1302();
	init_ds1302(&my_ds1302);
	
	while(1)
	{
		pc_command_processing();
		_delay_ms(1);
		delay_count++;

		if (delay_count >= 1000)
		{
			read_time_ds1302(&my_ds1302);
			read_date_ds1302(&my_ds1302);
			
			printf("DATE %d-%d-%d\n", my_ds1302.year, my_ds1302.month, my_ds1302.date);
			printf("TIME %d-%d-%d\n", my_ds1302.hour, my_ds1302.minutes, my_ds1302.seconds);
			
			delay_count = 0; // 카운터 초기화
		}
	}
}
void read_time_ds1302(t_ds1302 *p_ds1302)
{
	p_ds1302->seconds = read_ds1302(ADDR_SECONDS);
	p_ds1302->minutes = read_ds1302(ADDR_MINUTES);
	p_ds1302->hour    = read_ds1302(ADDR_HOUR);
}

void read_date_ds1302(t_ds1302 *p_ds1302)
{
	p_ds1302->year  = read_ds1302(ADDR_YEAR);
	p_ds1302->month = read_ds1302(ADDR_MONTH);
	p_ds1302->date  = read_ds1302(ADDR_DATE);
}

uint8_t read_ds1302(uint8_t addr)
{
	uint8_t data8bits = 0;
	
	DS1302_RST_PORT |= 1 << DS1302_RST;
	_delay_us(2); 

	tx_ds1302(addr | 0x01);
	
	rx_ds1302(&data8bits);

	DS1302_RST_PORT &= ~(1 << DS1302_RST);
	_delay_us(2);

	return (bcd2dec(data8bits));
}

void rx_ds1302(uint8_t *pdata8bits)
{
	uint8_t temp = 0;
	DS1302_DAT_DDR &= ~(1 << DS1302_DAT); // read mode 
	
	for (int i=0; i < 8; i++)
	{
		if(DS1302_DAT_PIN & (1 << DS1302_DAT))
		{
			temp |= 1 << i; 		
		}
		if(i != 7) 
			clock_ds1302();
	}
	*pdata8bits = temp;
}

void init_ds1302(t_ds1302 *p_ds1302)
{
	write_ds1302(ADDR_WRITEPROTECTED, 0x00); 

	write_ds1302(ADDR_SECONDS, p_ds1302->seconds);
	write_ds1302(ADDR_MINUTES, p_ds1302->minutes);
	write_ds1302(ADDR_HOUR, p_ds1302->hour);
	write_ds1302(ADDR_DATE, p_ds1302->date);
	write_ds1302(ADDR_MONTH, p_ds1302->month);
	write_ds1302(ADDR_DAYOFWEEK, p_ds1302->dayofweek);
	write_ds1302(ADDR_YEAR, p_ds1302->year);
}

uint8_t dec2bcd(uint8_t data)
{
	uint8_t high, low;
	high = (data/10) << 4; 
	low = data % 10;
	return(high+low);
}

uint8_t bcd2dec(uint8_t data)
{
	uint8_t high, low;
	low = data & 0x0F;
	high = (data >> 4) * 10;
	return(high+low);
}

void write_ds1302(uint8_t addr, uint8_t data)
{
	// 1. CE low --> High
	DS1302_RST_PORT |= 1 << DS1302_RST;
	_delay_us(5);

	// 2. ADDR 전송
	tx_ds1302(addr);

	// 3. DATA 전송
	tx_ds1302(dec2bcd(data));

	// 4. CE HIGH ---> LOW
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
}


void tx_ds1302(uint8_t data)
{
	DS1302_DAT_DDR |= 1 << DS1302_DAT;
	for (int i=0; i < 8; i++)
	{
		if (data & (1 << i))
			DS1302_DAT_PORT |= 1 << DS1302_DAT; 
		else
			DS1302_DAT_PORT &= ~(1 << DS1302_DAT); 
		clock_ds1302();
	}	
}

void clock_ds1302(void)
{
	// LOW ----> HIGH ----> LOW
	DS1302_CLK_PORT &= ~(1<<DS1302_CLK);
	_delay_us(1);
	DS1302_CLK_PORT |= 1<<DS1302_CLK;
	_delay_us(1);
	DS1302_CLK_PORT &= ~(1<<DS1302_CLK);
	_delay_us(1);
}

void init_ddr_ds1302(void)
{
	DDRF &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	DDRF |= (1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST); 
}

void init_gpio_ds1302(void)
{
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	_delay_ms(2);
}

void init_data_time(t_ds1302 *p_ds1302)
{
	p_ds1302->year = 26;
	p_ds1302->month = 06;
	p_ds1302->date = 26;
	p_ds1302->dayofweek = 6;  
	p_ds1302->hour = 15;
	p_ds1302->minutes = 20;
	p_ds1302->seconds = 00;
}