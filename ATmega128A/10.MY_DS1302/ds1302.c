/*
 * ds1302.c
 *
 * Created: 2026-06-26 오후 2:42:09
 *  Author: kccistc
 */ 

#include "ds1302.h"


void init_data_time(void);
void init_gpio_ds1302(void);
void init_ddr_ds1302(void);
void init_ds1302(void);
void ds1302_main(void);
void write_ds1302(uint8_t addr, uint8_t data);
void read_burst_ds1302(void);
uint8_t dec2bcd(uint8_t data);
uint8_t read_ds1302(uint8_t addr);
uint8_t bcd2dec(uint8_t data);

void read_date_ds1302(void);
void tx_ds1302(uint8_t data);
void rx_ds1302(uint8_t *pdata8bits);
void read_time_ds1302(void);
void clock_ds1302(void);


void ds1302_main(void)
{
	
	init_data_time();
	init_ddr_ds1302();  
	init_gpio_ds1302(); 
	init_ds1302();
	
	while(1)
	{
		// 1. read time
		read_burst_ds1302();
		// 2. read date
		// 3. printf data & time
		printf("DATE %02d-%02d-%02d\n", ds1302.year, ds1302.month, ds1302.date);
		printf("TIME %02d-%02d-%02d\n", ds1302.hour, ds1302.minutes, ds1302.seconds);
		// 4. delay_ms(1000);
		_delay_ms(1000);
	}
}


//CE(RST) 핀을 한 번만 올린 채로 데이터 7개 연속 수신

void read_burst_ds1302(void)
{
	uint8_t burst_data[7]; // 초, 분, 시, 일, 월, 요일, 년 데이터를 통째로 담을 임시 배열

	DS1302_RST_PORT |= (1 << DS1302_RST);

	tx_ds1302(0xBF);

	// RST(CE) 신호가 HIGH인 상태를 그대로 유지
	for (int i = 0; i < 7; i++)
	{
		uint8_t temp = 0;
		
		// 데이터를 받아야 하므로, AVR의 DAT 핀을 입력(Read) 모드로 전환
		DS1302_DAT_DDR &= ~(1 << DS1302_DAT);
		
		// LSB(최하위 비트)부터 8비트
		for (int b = 0; b < 8; b++)
		{
			if (DS1302_DAT_PIN & (1 << DS1302_DAT))
			{
				temp |= (1 << b); // 핀 상태가 HIGH(1)이면 해당 비트 자리에 1을 채움
			}

			if (!(i == 6 && b == 7))
			{
				clock_ds1302();
			}
		}
		burst_data[i] = temp; // 1바이트 완성될 때마다 배열에 순서대로 차곡차곡 적재
	}

	DS1302_RST_PORT &= ~(1 << DS1302_RST);
	

	ds1302.seconds   = bcd2dec(burst_data[0] & 0x7F); // 상위 1비트(CH) 필터링
	ds1302.minutes   = bcd2dec(burst_data[1]);
	ds1302.hour      = bcd2dec(burst_data[2] & 0x3F); // 상위 2비트(12/24H mode 등) 필터링
	ds1302.date      = bcd2dec(burst_data[3]);
	ds1302.month     = bcd2dec(burst_data[4]);
	ds1302.dayofweek = bcd2dec(burst_data[5]);
	ds1302.year      = bcd2dec(burst_data[6]);
}
void read_time_ds1302(void)
{
	ds1302.seconds = read_ds1302(ADDR_SECONDS);
	ds1302.minutes = read_ds1302(ADDR_MINUTES);
	ds1302.hour = read_ds1302(ADDR_HOUR);
}

void read_date_ds1302(void)
{
	ds1302.year = read_ds1302(ADDR_YEAR);
	ds1302.month = read_ds1302(ADDR_MONTH);
	ds1302.date = read_ds1302(ADDR_DATE);
}


uint8_t read_ds1302(uint8_t addr)
{
	uint8_t data8bits = 0; //1bit씩 읽어서 담을 변수
	//1. CE high
	//2. addr 전송
	//3. data를 읽어들임
	//4. CE low
	//5. return (bcd to dec)
	//1. CE low --> HIGH
	DS1302_RST_PORT |= 1 << DS1302_RST;
	
	//2. ADDR 전송
	tx_ds1302(addr+1); //read addr
	
	//3. DATA 읽어들인다.
	rx_ds1302(&data8bits);

	//4. CE HIGH --> LOW
	DS1302_RST_PORT &= ~(1 << DS1302_RST);

	return (bcd2dec(data8bits));
	
}

void rx_ds1302(uint8_t *pdata8bits)
{
	uint8_t temp = 0;
	
	//1. 출력 mode로 설정
	DS1302_DAT_DDR &= ~(1 << DS1302_DAT); //read mode 
	
	//LSB로부터 차례로 입력
	for (int i=0; i < 8; i++)
	{
		if(DS1302_DAT_PIN & (1 << DS1302_DAT))
		{
			temp |= 1 << i; // 1의 조건만 set		
		}
		//3. CLK를 HIGH -- > LOW
		if(i !=7) //마지막 bit를 읽을 때는 clk를 보내주지 않는다.
		clock_ds1302();
	}
	*pdata8bits = temp;
}

void init_ds1302(void)
{
	
	write_ds1302(ADDR_SECONDS, ds1302.seconds);
	write_ds1302(ADDR_MINUTES, ds1302.minutes);
	write_ds1302(ADDR_HOUR, ds1302.hour);
	write_ds1302(ADDR_DATE, ds1302.date);
	write_ds1302(ADDR_MONTH, ds1302.month);
	write_ds1302(ADDR_DAYOFWEEK, ds1302.dayofweek);
	write_ds1302(ADDR_YEAR, ds1302.year);
}

// dec --> bcd
//ex) 25
// dec         bcd
//00011001	0010  0101
uint8_t dec2bcd(uint8_t data)
{
	uint8_t high, low;
	
	high = (data/10) << 4; //high nibble에 위치
	low = data % 10;
	
	return(high+low);
}

//1.입력 bcd
// ex)26년의 bcd
// 7654 3210
//  2     6
//  x10    x1
//============
//     26
uint8_t bcd2dec(uint8_t data)
{
	uint8_t high, low;
	
	low = data & 0x0F;
	high = (data >> 4) * 10;
	
	return(high+low);
}

void write_ds1302(uint8_t addr, uint8_t data)
{
	//1. CE low --> HIGH
	DS1302_RST_PORT |= 1 << DS1302_RST;
	
	//2. ADDR 전송
	tx_ds1302(addr);
	
	//3. DATA 전송
	tx_ds1302(dec2bcd(data));

	//4. CE HIGH --> LOW
	DS1302_RST_PORT &= ~(1 << DS1302_RST);
}

void tx_ds1302(uint8_t data)
{
	//1. 출력 mode로 설정
	DS1302_DAT_DDR |= 1 << DS1302_DAT;
	// ex) 0x80
	// M      L		
	// 1000 000
	for (int i=0; i < 8; i++)
	{
		if (data & (1 << i))
			DS1302_DAT_PORT |= 1 << DS1302_DAT; // 1
		else
			DS1302_DAT_PORT &= ~(1 << DS1302_DAT); // 0
		//3. CLK를 HIGH -- > LOW
			 clock_ds1302();
	}	
}

void clock_ds1302(void)
{
	// LOW --> HIGH --> LOW
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
	DS1302_CLK_PORT |= (1 << DS1302_CLK);
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK);
}

void init_ddr_ds1302(void)
{
	DDRF &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	DDRF |= (1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST); //출력 mode로 설정
}

void init_gpio_ds1302(void)
{
	DS1302_CLK_PORT &= ~(1 << DS1302_CLK | 1 << DS1302_DAT | 1 << DS1302_RST);
	_delay_ms(2);
}

void init_data_time(void)
{
	ds1302.year = 26;
	ds1302.month = 06;
	ds1302.date = 26;
	ds1302.dayofweek = 6;  //fri
	ds1302.hour = 15;
	ds1302.minutes = 20;
	ds1302.seconds = 00;
}