/*
 * ds1307.h
 *
 * Created: 2026-06-26
 * Author: kccistc
 */ 

#ifndef DS1307_H_
#define DS1307_H_

#define F_CPU 16000000UL     // 16MHz
#include <avr/io.h>          // PORTD, DDRD, PIND 등이 정의된 헤더
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

// --- 하드웨어 매핑 변경: PORTF -> PORTD (PD0: SCL, PD1: SDA) ---

// SCL (시리얼 클럭) -> PD0 핀 설정
#define DS1307_SCL_DDR   DDRD
#define DS1307_SCL_PORT  PORTD
#define DS1307_SCL       PD0   // 0번 핀

// SDA (시리얼 데이터) -> PD1 핀 설정
#define DS1307_SDA_DDR   DDRD
#define DS1307_SDA_PORT  PORTD
#define DS1307_SDA_PIN   PIND  // 데이터 읽기용 입력 레지스터 (PIND 사용)
#define DS1307_SDA       PD1   // 1번 핀


// --- DS1307 내부 레지스터 주소 맵 ---
#define ADDR_SECONDS     0x00
#define ADDR_MINUTES     0x01
#define ADDR_HOUR        0x02
#define ADDR_DAYOFWEEK   0x03  
#define ADDR_DATE        0x04
#define ADDR_MONTH       0x05
#define ADDR_YEAR        0x06
#define ADDR_CONTROL     0x07  

// --- DS1307 RTC 데이터 구조체 ---
typedef struct _ds1307
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;  // 1:일, 2:월 ... 7:토
	uint8_t date;
	uint8_t month;
	uint8_t year;
	uint8_t ampm;       
	uint8_t hourmode;   
} t_ds1307;

extern t_ds1307 ds1307;

#endif /* DS1307_H_ */