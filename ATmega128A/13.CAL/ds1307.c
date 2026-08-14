/*
 * ds1307.c
 *
 * 수정 사항: 소프트웨어 I2C 통신 규격 정밀화 및 안정적 타이밍 확보
 */ 

#include "ds1307.h"
t_ds1307 ds1307;

void init_data_time(void);
void init_gpio_ds1307(void);
void init_ddr_ds1307(void);
void init_ds1307(void);
void ds1307_main(void);
void write_ds1307(uint8_t addr, uint8_t data);
void read_burst_ds1307(void);
uint8_t dec2bcd(uint8_t data);
uint8_t read_ds1307(uint8_t addr);
uint8_t bcd2dec(uint8_t data);

void read_date_ds1307(void);
void read_time_ds1307(void);

// 소프트웨어 I2C 통신용 하위 함수
void i2c_start(void);
void i2c_stop(void);
void i2c_tx(uint8_t data);
uint8_t i2c_rx(uint8_t ack);

void ds1307_main(void)
{
	init_data_time();
	init_ddr_ds1307();  
	init_gpio_ds1307(); 
	init_ds1307();
	
	while(1)
	{
		read_burst_ds1307();
		printf("DATE %02d-%02d-%02d\n", ds1307.year, ds1307.month, ds1307.date);
		printf("TIME %02d-%02d-%02d\n", ds1307.hour, ds1307.minutes, ds1307.seconds);
		_delay_ms(1000);
	}
}

void read_burst_ds1307(void)
{
	uint8_t burst_data[7];

	i2c_start();
	i2c_tx(0xD0);          // SLA+W
	i2c_tx(0x00);          // 시작 주소 (Seconds)
	
	i2c_start();           // Repeated START
	i2c_tx(0xD1);          // SLA+R

	for (int i = 0; i < 6; i++)
	{
		burst_data[i] = i2c_rx(1); // 1 = ACK
	}
	burst_data[6] = i2c_rx(0);     // 0 = NACK 
	i2c_stop();

	ds1307.seconds   = bcd2dec(burst_data[0] & 0x7F); 
	ds1307.minutes   = bcd2dec(burst_data[1]);
	ds1307.hour      = bcd2dec(burst_data[2] & 0x3F); 
	ds1307.dayofweek = bcd2dec(burst_data[3]);        
	ds1307.date      = bcd2dec(burst_data[4]);        
	ds1307.month     = bcd2dec(burst_data[5]);
	ds1307.year      = bcd2dec(burst_data[6]);
}

void read_time_ds1307(void)
{
	ds1307.seconds = read_ds1307(0x00);
	ds1307.minutes = read_ds1307(0x01);
	ds1307.hour    = read_ds1307(0x02);
}

void read_date_ds1307(void)
{
	ds1307.date  = read_ds1307(0x04);
	ds1307.month = read_ds1307(0x05);
	ds1307.year  = read_ds1307(0x06);
}

uint8_t read_ds1307(uint8_t addr)
{
	uint8_t data8bits = 0;
	
	i2c_start();
	i2c_tx(0xD0);          
	i2c_tx(addr);          
	
	i2c_start();           
	i2c_tx(0xD1);          
	data8bits = i2c_rx(0); 
	i2c_stop();

	return (bcd2dec(data8bits));
}

void init_ds1307(void)
{
	write_ds1307(0x00, ds1307.seconds);
	write_ds1307(0x01, ds1307.minutes);
	write_ds1307(0x02, ds1307.hour);
	write_ds1307(0x03, ds1307.dayofweek);
	write_ds1307(0x04, ds1307.date);
	write_ds1307(0x05, ds1307.month);
	write_ds1307(0x06, ds1307.year);
}

void write_ds1307(uint8_t addr, uint8_t data)
{
	i2c_start();
	i2c_tx(0xD0);          
	i2c_tx(addr);          
	i2c_tx(dec2bcd(data)); 
	i2c_stop();
}

uint8_t dec2bcd(uint8_t data)
{
	return ((data / 10) << 4) + (data % 10);
}

uint8_t bcd2dec(uint8_t data)
{
	return ((data >> 4) * 10) + (data & 0x0F);
}

// =========================================================================
// 💡 핵심 수정: 소프트웨어 I2C 프로토콜 구현 함수들 개편
// =========================================================================

void i2c_start(void)
{
	// 우선 SCL, SDA를 확실한 High 상태(입력+풀업)로 시작
	DS1307_SDA_DDR &= ~(1 << DS1307_SDA);
	DS1307_SCL_DDR &= ~(1 << DS1307_SCL);
	DS1307_SDA_PORT |= (1 << DS1307_SDA);
	DS1307_SCL_PORT |= (1 << DS1307_SCL);
	_delay_us(5);
	
	// SCL이 High일 때 SDA를 Low로 떨어뜨림 (START)
	DS1307_SDA_PORT &= ~(1 << DS1307_SDA); // PORT를 0으로 준비한 후
	DS1307_SDA_DDR |= (1 << DS1307_SDA);   // 출력 모드로 바꾸어 강제 Low 실행
	_delay_us(5);
	
	// SCL도 함께 Low로 떨어뜨림
	DS1307_SCL_PORT &= ~(1 << DS1307_SCL);
	DS1307_SCL_DDR |= (1 << DS1307_SCL);
	_delay_us(5);
}

void i2c_stop(void)
{
	// SDA를 확실하게 Low 상태로 만듦
	DS1307_SDA_PORT &= ~(1 << DS1307_SDA);
	DS1307_SDA_DDR |= (1 << DS1307_SDA);
	_delay_us(5);
	
	// SCL을 먼저 High(입력+풀업)로 띄움
	DS1307_SCL_DDR &= ~(1 << DS1307_SCL);
	_delay_us(5);
	
	// SCL이 High인 상태에서 SDA를 High(입력+풀업)로 띄움 (STOP)
	DS1307_SDA_DDR &= ~(1 << DS1307_SDA);
	_delay_us(5);
}

void i2c_tx(uint8_t data)
{
	for (int i = 0; i < 8; i++)
	{
		if (data & (0x80 >> i))
		{
			// High(1)를 보낼 때는 선을 놓아줍니다 (입력모드 전환하여 풀업이 당기게 함)
			DS1307_SDA_DDR &= ~(1 << DS1307_SDA);
		}
		else
		{
			// Low(0)를 보낼 때는 확실하게 출력 모드로 밀어버립니다
			DS1307_SDA_PORT &= ~(1 << DS1307_SDA);
			DS1307_SDA_DDR |= (1 << DS1307_SDA);
		}
		_delay_us(5);
		
		// SCL 클럭 발생
		DS1307_SCL_DDR &= ~(1 << DS1307_SCL); // SCL High (풀업)
		_delay_us(5);
		DS1307_SCL_PORT &= ~(1 << DS1307_SCL); // SCL Low
		DS1307_SCL_DDR |= (1 << DS1307_SCL);
		_delay_us(5);
	}
	
	// 9번째 클럭: 슬레이브의 ACK 응답을 받기 위해 마스터는 선을 완전히 놓음
	DS1307_SDA_DDR &= ~(1 << DS1307_SDA);
	_delay_us(5);
	
	DS1307_SCL_DDR &= ~(1 << DS1307_SCL); // SCL High 상태에서 ACK 체크 가능 타이밍
	_delay_us(5);
	
	DS1307_SCL_PORT &= ~(1 << DS1307_SCL); // SCL Low로 마무리
	DS1307_SCL_DDR |= (1 << DS1307_SCL);
	_delay_us(5);
}

uint8_t i2c_rx(uint8_t ack)
{
	uint8_t temp = 0;
	
	// 데이터를 받아야 하므로 SDA 선을 놓아줌 (입력 모드)
	DS1307_SDA_DDR &= ~(1 << DS1307_SDA);
	
	for (int i = 0; i < 8; i++)
	{
		_delay_us(5);
		DS1307_SCL_DDR &= ~(1 << DS1307_SCL); // SCL High 진입 (데이터 안착)
		_delay_us(5);
		
		if (DS1307_SDA_PIN & (1 << DS1307_SDA))
		{
			temp |= (0x80 >> i); 
		}
		
		DS1307_SCL_PORT &= ~(1 << DS1307_SCL); // SCL Low
		DS1307_SCL_DDR |= (1 << DS1307_SCL);
	}
	
	// 마스터가 ACK(0) 또는 NACK(1) 응답 송신
	if (ack)
	{
		// ACK 전송: SDA를 강제로 0(Low)으로 떨어뜨림
		DS1307_SDA_PORT &= ~(1 << DS1307_SDA);
		DS1307_SDA_DDR |= (1 << DS1307_SDA);
	}
	else
	{
		// NACK 전송: SDA를 그냥 놓아줌 (High)
		DS1307_SDA_DDR &= ~(1 << DS1307_SDA);
	}
	_delay_us(5);
	
	DS1307_SCL_DDR &= ~(1 << DS1307_SCL); // SCL High
	_delay_us(5);
	DS1307_SCL_PORT &= ~(1 << DS1307_SCL); // SCL Low
	DS1307_SCL_DDR |= (1 << DS1307_SCL);
	_delay_us(5);
	
	// 수신 종료 후 안전하게 SDA 라인 해제
	DS1307_SDA_DDR &= ~(1 << DS1307_SDA);
	
	return temp;
}

void init_ddr_ds1307(void)
{
	// 처음 초기화는 내부 풀업 저항만 사용하도록 둘 다 입력 모드로 세팅
	DS1307_SCL_DDR &= ~((1 << DS1307_SCL) | (1 << DS1307_SDA));
}

void init_gpio_ds1307(void)
{
	// 입력 모드 상태에서 PORT 레지스터를 1로 하면 '강제 내부 풀업' 활성화
	DS1307_SCL_PORT |= (1 << DS1307_SCL);
	DS1307_SDA_PORT |= (1 << DS1307_SDA);
	_delay_ms(5);
}

void init_data_time(void)
{
	ds1307.year = 26;
	ds1307.month = 07;
	ds1307.date = 02;
	ds1307.dayofweek = 6;  
	ds1307.hour = 15;
	ds1307.minutes = 20;
	ds1307.seconds = 00;
}