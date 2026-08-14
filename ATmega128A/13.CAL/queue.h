
#ifndef QUEUE_H_
#define QUEUE_H_

#define TRUE 1
#define FALSE 0
#define QUEUE_MAX 100

int queue_full(void);
int queue_empty();
uint8_t read_queue();
void queue_init();
void insert_queue(uint8_t value);
uint8_t delete_queue(void); // 이 선언이 꼭 있어야 합니다!
uint8_t is_queue_empty(void);

#endif /* QUEUE_H_ */