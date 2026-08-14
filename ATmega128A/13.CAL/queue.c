#include <stdint.h> // uint8_t 사용을 위해 추가
#include <stdio.h>
#include "queue.h"

int front = -1;
int rear = -1;
uint8_t queue[QUEUE_MAX];

int queue_full(void)
{
	int tmp = (rear + 1) % QUEUE_MAX;
	if (tmp == front) return TRUE;
	else return FALSE;
}

int queue_empty(void) // 헤더와 맞춤
{
	if (rear == front) return TRUE;
	else return FALSE;
}

// 💡 링커 에러 해결: 헤더에 선언된 이름으로 변경
uint8_t delete_queue(void)
{
	if (queue_empty())
	{
		printf("Queue is empty !!!\n");
		return 0;
	}
	else
	{
		front = (front + 1) % QUEUE_MAX;
		return (queue[front]);
	}
}


uint8_t is_queue_empty(void)
{
	return (queue_empty() == TRUE);
}

void queue_init(void)
{
	front = -1;
	rear = -1;
}

void insert_queue(uint8_t value)
{
	if (queue_full())
	{
		printf("queue full!!!!\n");
		return;
	}
	else
	{
		rear = (rear + 1) % QUEUE_MAX;
		queue[rear] = value;
	}
}