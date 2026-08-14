#if 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/* 
1. 최대 입력 data는 20개
2. 입력 fgets 함수
enter data(종료: exit) : 22
enter data(종료 : exit) : 11
enter data(종료 : exit) : 7
enter data(종료 : exit) : 3
enter data(종료 : exit) : 2
enter data(종료 : exit) : 1
enter data(종료 : exit) : exit
3. Heap sort로 data를 sort 한다.
* ***input data * ***
22 11 7 3 2 1
* ***sort data * ***
1 2 3 7 11 22
4. 찾고자 하는 data는 ? (전체: all 종료 : exit) 22
원하는 data 22를 찾았습니다
찾고자 하는 data는 ? (전체 : all 종료 : exit) 99
99를 찾을 수 없습니다.
찾고자 하는 data는 ? (전체 : all 종료 : exit) all
1 2 3 7 11 22
찾고자 하는 data는 ? (전체 : all 종료 : exit) exit
프로그램이 종료 되었습니다 !!!!!
*/

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

void heapify(int arr[], int n, int i) {
	int largest = i;          // 부모 노드
	int left = 2 * i + 1;     // 왼쪽 자식 노드
	int right = 2 * i + 2;    // 오른쪽 자식 노드


	if (left < n && arr[left] > arr[largest])
		largest = left;


	if (right < n && arr[right] > arr[largest])
		largest = right;


	if (largest != i) {
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest);
	}
}


void heapSort(int arr[], int n) {

	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	for (int i = n - 1; i > 0; i--) {
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}

int main(void) {
	char buff[40];
	int arry[100];
	int count = 0;

	while (1) {
		printf("enter data (종료: exit) : ");
		fgets(buff, sizeof(buff), stdin);


		buff[strcspn(buff, "\n")] = '\0';

		if (strcmp(buff, "exit") == 0) {
			break;
		}

		arry[count] = atoi(buff);
		count++;

		if (count >= 10) {
			printf("최대 입력 개수(10개)에 도달했습니다.\n");
			break;
		}
	}

	printf("**** input data ****\n");
	for (int i = 0; i < count; i++) {
		printf("%d ", arry[i]);
	}
	printf("\n");


	heapSort(arry, count);

	printf("**** sort data ****\n");
	for (int i = 0; i < count; i++) {
		printf("%d ", arry[i]);
	}
	printf("\n");

	return 0;
}

#endif
#if 0
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/*
------- 아래 ------
 1. 최대 입력 data는 10개로
 2. 입력 fgets 함수

 enter data (종료: exit) : 11
 enter data (종료: exit) : 7
 enter data (종료: exit) : 3
 enter data (종료: exit) : exit
 **** input data ****
 11 7 3
 **** sort data ****
 3 7 11

*/


void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


void heapify(int arr[], int n, int i) {
	int largest = i;          // 부모 노드
	int left = 2 * i + 1;     // 왼쪽 자식 노드
	int right = 2 * i + 2;    // 오른쪽 자식 노드


	if (left < n && arr[left] > arr[largest])
		largest = left;


	if (right < n && arr[right] > arr[largest])
		largest = right;


	if (largest != i) {
		swap(&arr[i], &arr[largest]);
		heapify(arr, n, largest);
	}
}


void heapSort(int arr[], int n) {

	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	for (int i = n - 1; i > 0; i--) {
		swap(&arr[0], &arr[i]); 
		heapify(arr, i, 0);     
	}
}

int main(void) {
	char buff[40];
	int arry[100];
	int count = 0;

	while (1) {
		printf("enter data (종료: exit) : ");
		fgets(buff, sizeof(buff), stdin);


		buff[strcspn(buff, "\n")] = '\0';

		if (strcmp(buff, "exit") == 0) {
			break;
		}

		arry[count] = atoi(buff);
		count++;

		if (count >= 10) {
			printf("최대 입력 개수(10개)에 도달했습니다.\n");
			break;
		}
	}

	printf("**** input data ****\n");
	for (int i = 0; i < count; i++) {
		printf("%d ", arry[i]);
	}
	printf("\n");


	heapSort(arry, count);

	printf("**** sort data ****\n");
	for (int i = 0; i < count; i++) {
		printf("%d ", arry[i]);
	}
	printf("\n");

	return 0;
}
#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100
typedef struct {
	int top;
	double data[MAX];
} Stack;

void init_stack(Stack* s)
{
	s->top = -1;
}

int is_empty(Stack* s)
{
	if (s->top == -1)
		return 1;
	else
		return 0;
}

int is_full(Stack* s)
{
	if (s->top == MAX - 1)
		return 1;
	else
		return 0;
}

void push(Stack* s, double item)
{
	if (is_full(s) == 1) 
		printf("full"); 
	else
		s->data[++(s->top)] = item; 
}

double pop(Stack* s)
{
	if (is_empty(s) == 1) 
		printf("empty");
	else
		return s->data[(s->top)--];
}

int peek(Stack* s) {
	if (is_empty(s)) return 0;
	return s->data[s->top];
}

int priority(char op) {
	if (op == '*' || op == '/') return 2;
	if (op == '+' || op == '-') return 1;
	return 0;
}

double calculate(double op1, double op2, char op) {
	if (op == '+') return op1 + op2;
	if (op == '-') return op1 - op2;
	if (op == '*') return op1 * op2;
	if (op == '/') return op1 / op2;
	
	return 0;
}

int check(char * p) {
	int count = 0;

	for (int i = 0; p[i] != '\0' && p[i] != '\n'; i++) {
		if (p[i] == '(') {
			count++;
		}
		else if (p[i] == ')') {
			count--;
			if (count < 0) {
				return 0;
			}
		}
	}

	if (count == 0) return 1;
	else return 0;              
}

void infix_to_postfix(char* infix, char* postfix)
{
	Stack op;
	init_stack(&op);

	int k = 0;

	for (int i = 0; infix[i] != '\0' &&
		infix[i] != '\n' &&
		infix[i] != '='; i++)
	{
		char ch = infix[i];

		if (ch == ' ')
			continue;

		if (ch >= '0' && ch <= '9')
		{
			while (infix[i] >= '0' && infix[i] <= '9')
			{
				postfix[k++] = infix[i++];
			}

			postfix[k++] = ' ';
			i--;
		}

		else if (ch == '(')
		{
			push(&op, ch);
		}

		else if (ch == ')')
		{
			while (!is_empty(&op) && peek(&op) != '(')
			{
				postfix[k++] = (char)pop(&op);
				postfix[k++] = ' ';
			}

			pop(&op);
		}

		else if (ch == '+' || ch == '-' ||
			ch == '*' || ch == '/')
		{
			while (!is_empty(&op) &&
				priority(peek(&op)) >= priority(ch))
			{
				postfix[k++] = (char)pop(&op);
				postfix[k++] = ' ';
			}

			push(&op, ch);
		}
	}

	while (!is_empty(&op))
	{
		postfix[k++] = (char)pop(&op);
		postfix[k++] = ' ';
	}

	postfix[k] = '\0';
}

double evaluate_postfix(char* postfix)
{
	Stack s;
	init_stack(&s);

	for (int i = 0; postfix[i] != '\0'; i++)
	{
		if (postfix[i] == ' ')
			continue;

		if (postfix[i] >= '0' && postfix[i] <= '9')
		{
			double num = 0;

			while (postfix[i] >= '0' &&
				postfix[i] <= '9')
			{
				num = num * 10 + (postfix[i] - '0');
				i++;
			}

			push(&s, num);
			i--;
		}

		else if (postfix[i] == '+' ||
			postfix[i] == '-' ||
			postfix[i] == '*' ||
			postfix[i] == '/')
		{
			double op2 = pop(&s);
			double op1 = pop(&s);

			push(&s,
				calculate(op1, op2, postfix[i]));
		}
	}

	return pop(&s);
}

int main(void)
{
	Stack s; 
	init_stack(&s);
	char inbuff[MAX];
	char postfix[200];

	printf("==사칙연산==\n");

	while (1) {
		printf("계산식 입력 (종료 : exit) : ");
		fgets(inbuff, sizeof(inbuff), stdin);
		if (strncmp(inbuff, "exit", 4) == 0)
			break;

		if (check(inbuff) == 0) {
			printf("괄호 누락!!\n");
			continue; 
		}
		infix_to_postfix(inbuff, postfix);
		double result = evaluate_postfix(postfix);

		printf("계산 결과 : %.1lf\n", result);
	}
}
#endif



#if 0
// menu 실행이 종료된 뒤에 다시 메뉴를 실행시 이전 계좌정보를 유지(화일에 유지) 할 수 있도록
// 프로그램 구조를 변경 한다.  
// 화일 처리 중에서 fseek rewind 함수 활용 해서 처리 한다.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define NAME_LEN   20
#define NUM 10
enum { MAKE = 1, DEPOSIT, WITHDRAW, INQUIRE, EXIT = 9 };

FILE* filep; //file 포인터 stdio.h를 include한다

typedef struct
{
	int acc_id;      // 계좌번호
	int balance;    // 잔    액
	char cus_name[NAME_LEN];  // 고객이름
} t_account;   // total : 28bytes

void show_menu(void);       // 메뉴출력
void make_account(t_account* pa, int* pan);       // 계좌개설을 위한 함수
void deposit_money(t_account* pa, int* pan);       // 입    금
void with_draw_money(t_account* pa, int* pan);      // 출    금
void show_all_acc_info(t_account* pa, int* pan);     // 잔액조회

FILE* filep;   // file pointer  stdio.h를 include 한다.
int main()  // int main(argc, char *argv[])
{
	t_account* acc_arr;   // Account 저장을 위한 배열
	int acc_num = 0;        // 저장된 Account 수
	int choice;

	void (*fp[]) (t_account*, int*) =
	{
		NULL,   // 0
		make_account,  // 1
		deposit_money,  // 2
		with_draw_money, // 3
		show_all_acc_info  // 4
	};

	acc_arr = (t_account*)malloc(sizeof(t_account) * NUM);
	//              malloc(28*NUM);  // 280 bytes
	if (!acc_arr)
	{
		printf("메모리 할당 실패 !!!!\n");
		return -1;
	}

	if ((filep = fopen("money_file", "rb+")) == NULL)
	{
		if (!(filep = fopen("money_file", "wb+")))
		{
			fprintf(stderr, "can not open money_file !!!\n");  // stdin: keybord stdout: 화면 stderr: 화면에 오류
			return (-1);   // ./aout 2>error  ./a.out > normal
		}
	}
	while (1)
	{
		show_menu();
		printf("선택: ");
		scanf("%d", &choice);
		printf("\n");
		if (choice == 9)
		{
			free(acc_arr);
			fclose(filep);
			break;
		}
		if (choice >= 1 && choice <= 4)
			fp[choice](acc_arr, &acc_num);
	}
	return 0;
}

void show_menu(void)
{
	char* menu[] =     // menu[][]
	{
	 "-----Menu------\n",
	 "1. 계좌개설\n",
	 "2. 입    금\n",
	 "3. 출    금\n",
	 "4. 계좌정보 전체 출력\n",
	 "9. 종    료\n"
	};

	int i;

	for (int i = 0; i < 6; i++)
		printf("%s", *(menu + i));
}

void make_account(t_account* pa, int* pan)
{
	int id;
	char name[NAME_LEN];
	int balance;
	t_account* tp;

	printf("[계좌개설]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("이  름: ");
	scanf("%s", name);
	printf("입금액: ");
	scanf("%d", &balance);
	printf("\n");

	tp = pa + *pan;
	rewind(filep);  // 화일 헤더를 처음 위치로 옮기는 작업

	for (int i = 0; ; i++)
	{
		if (!fread((char*)&tp->acc_id, 1, sizeof(t_account), filep))
			break;
		else if (tp->acc_id == id)
		{
			printf("이미 존재 하는 ID: %d\n", id);
			return;
		}
	}

	fseek(filep, 0, SEEK_END);  // 화일 끝으로 head를 움직인다.


	tp->acc_id = id;
	tp->balance = balance;
	strcpy(tp->cus_name, name);
	fwrite((char*)&tp->acc_id, 1, sizeof(t_account), filep);

	*pan = *pan + 1;
}

void deposit_money(t_account* pa, int* pan)
{
	int money;
	int id, i, size;

	t_account* tp = pa;

	printf("[입    금]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("입금액: ");
	scanf("%d", &money);

	rewind(filep);   // 화일의 처음 위치로

	for (i = 0; i < *pan; i++, tp++)
	{
		if (!fread((char*)&tp->acc_id, 1, sizeof(t_account), filep))
			break;
		else if (tp->acc_id == id)
		{
			tp->balance += money;
			size = sizeof(t_account);
			fseek(filep, -size, SEEK_CUR);  // 화일 헤더를 t_account 만큼 뒤로
			fwrite((char*)&tp->acc_id, 1, sizeof(t_account), filep);
			printf("입금완료\n\n");
			return;
		}


	}
	printf("유효하지 않은 ID 입니다.\n\n");
}

void with_draw_money(t_account* pa, int* pan)
{
	int money;
	int id, i;
	t_account* tp = pa;

	printf("[출    금]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("출금액: ");
	scanf("%d", &money);

	rewind(filep);   // 화일의 처음 위치로

	for (i = 0; i < *pan; i++, tp++)
	{
		if (!fread((char*)&tp->acc_id, 1, sizeof(t_account), filep))
			break;
		else if (tp->acc_id == id)
		{
			if (tp->balance < money)
			{
				printf("잔액부족\n\n");
				return;
			}
			tp->balance -= money;
			int size = sizeof(t_account);
			fseek(filep, -size, SEEK_CUR);  // 화일 헤더를 t_account 만큼 뒤로
			fwrite((char*)&tp->acc_id, 1, sizeof(t_account), filep);
			printf("출금완료\n\n");
			return;
		}
	}
	printf("유효하지 않은 ID 입니다.\n\n");
}

void show_all_acc_info(t_account* pa, int* pan)
{
	int i;
	t_account* tp = pa;

	rewind(filep);

	printf("전체 계좌출력 \n");

	for (i = 0; ; i++, tp++)
	{
		if (!fread((char*)&tp->acc_id, 1, sizeof(t_account), filep))
			break;

		printf("계좌ID: %d\n", tp->acc_id);
		printf("이  름: %s\n", tp->cus_name);
		printf("잔  액: %d\n\n", tp->balance);
	}

	*pan = i;
}
#endif

#if 0

//전역변수를 지역변수로 변경하여 처리하기

//switch ~ case 문을 함수 포인터 배열로 동작되도록 변경

//t_account acc arr[100] //Account 저장을 위한 배열을 동적 메모리로 할당
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //malloc,atoi
#define NAME_LEN   20
#define NUMBER 10

typedef struct
{
	int acc_id;      // 계좌번호
	int balance;    // 잔    액
	char cus_name[NAME_LEN];   // 고객이름
} t_account;

void show_menu(void);       // 메뉴출력
void make_account(t_account* pa, int* pn);       // 계좌개설을 위한 함수
void deposit_money(t_account* pa, int* pn);       // 입    금
void with_draw_money(t_account* pa, int* pn);      // 출    금
void show_all_acc_info(t_account* pa, int* pn);     // 잔액조회

enum { MAKE = 1, DEPOSIT, WITHDRAW, INQUIRE, EXIT = 9 };



int main()  // int main(argc, char *argv[])
{
	t_account* acc_arr;   // Account 저장을 위한 배열
	int acc_num = 0;        // 저장된 Account 수
	int choice;

	void (*fp[])(t_account*, int*) = {
		make_account,      
		deposit_money,     
		with_draw_money,   
		show_all_acc_info  
	};

	acc_arr = (t_account*)malloc(sizeof(t_account) * NUMBER);

		while (1)
		{
			show_menu();
			printf("선택: ");
			scanf("%d", &choice);
			printf("\n");
			if (choice == EXIT)
			{
				break;
			}
			else if (choice >= MAKE && choice <= INQUIRE) {

				fp[choice - 1](acc_arr, &acc_num);
			}
			else if (choice != EXIT) {
				printf("Illegal selection..\n");
			}
		}
		free(acc_arr);
	return 0;
}

void show_menu(void)
{
	char* menu[] =
	{
	 "-----Menu------\n",
	 "1. 계좌개설\n",
	 "2. 입    금\n",
	 "3. 출    금\n",
	 "4. 계좌정보 전체 출력\n",
	 "9. 종    료\n"
	};
	int i;

	for (i = 0; i < 6; i++)
		printf("%s", *(menu + i));
}

void make_account(t_account* pa, int* pn)
{
	int id;
	char name[NAME_LEN];
	int balance;

	printf("[계좌개설]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("이  름: ");
	scanf("%s", name);
	printf("입금액: ");
	scanf("%d", &balance);
	printf("\n");

	(pa + *pn)->acc_id = id;
	(pa + *pn)->balance = balance;
	strcpy((pa + *pn)->cus_name, name);
	*pn = *pn + 1;
}

void deposit_money(t_account* pa, int* pn)
{
	int money;
	int id, i;

	printf("[입    금]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("입금액: ");
	scanf("%d", &money);

	for (i = 0; i < *pn; i++)
	{
		if ((pa + i)->acc_id == id)
		{
			(pa + i)->balance += money;
			printf("입금완료\n\n");
			return;
		}
	}
	printf("유효하지 않은 ID 입니다.\n\n");
}

void with_draw_money(t_account* pa, int* pn)
{
	int money;
	int id, i;

	printf("[출    금]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("출금액: ");
	scanf("%d", &money);

	for (i = 0; i < *pn; i++)
	{
		if ((pa + i)->acc_id == id)
		{
			if ((pa + i)->balance < money)
			{
				printf("잔액부족\n\n");
				return;
			}

			(pa + i)->balance -= money;  // acc_arr[i].balance = acc_arr[i].balance - money;
			printf("출금완료\n\n");
			return;
		}
	}
	printf("유효하지 않은 ID 입니다.\n\n");
}

void show_all_acc_info(t_account* pa, int* pn)
{
	int i;

	for (i = 0; i < *pn; i++)
	{
		printf("계좌ID: %d\n", (pa + i)->acc_id);
		printf("이  름: %s\n", (pa + i)->cus_name);
		printf("잔  액: %d\n\n", (pa + i)->balance);
	}
}
#endif


#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

//

/*
아래와 같이 출력되는 사칙연산을 (1) switch~case문과 (2)함수로 작성
==사칙연산==
계산(+, -, *, / (종료 :9 ) :
num1 :10
num2 : 20
결과 : 10 + 20 = 30

함수
목적 : 자주쓰이는 중복 기능을 한개의 모듈로 만들어서 필요시마다 호출하여 사용하므로 code의 중복을 최소화 하기 위함이다.

매크로 : 사용자가 작성한 코드를 compile시 사용자가 지정한 곳으로 대치됨
함수와 기능이 유사하나 함수는 호출에 의해서 실행, 매크로는 사용자 code로 확장 배치된다.
함수에 비해서 상대적으로 짧고 빠른 동작을 원할경우 매크로 사용
*/



void add(int a, int b) {
	printf(" %d + %d = %d\n", a, b, a + b);
}
void sub(int a, int b) {
	printf(" %d - %d = %d\n", a, b, a - b);
}
void multi(int a, int b) {
	printf(" %d * %d = %d\n", a, b, a * b);
}
void div(int a, int b) {
	printf(" %d / %d = %d\n", a, b, a / b);
}


//함수 포인터 배열
int (*fp[]) (int, int) = {
	NULL,

	add,
	sub, //fp[0] 방에는 add의; 시작번지,fp[1]에는 sub
	multi,
	div

};

int main(void) {

	char func[4];
	char buff[8];
	int num1, num2 = 0;
	int sel = 0;


	printf("==사칙연산==\n");

	while (1) {
		printf("계산(0:+, 1:-, 2:*, 3:/ (종료 :9 ) : ");
		fgets(func, 4, stdin);
		if (func[0] == '9')
			break;
		sel = atoi(func);
		printf("num1 (종료:exit) : ");
		fgets(buff, 8, stdin);
		if (strncmp(buff, "exit", 4) == 0)
			break;
		num1 = atoi(buff);

		printf("num2 (종료:exit) : ");
		fgets(buff, 8, stdin);
		if (strncmp(buff, "exit", 4) == 0)
			break;
		num2 = atoi(buff);

		fp[sel](num1, num2);
	}
}



#endif

#if 0
//전역변수를 지역변수로 변경하여 처리하기

//t_account acc arr[100] //Account 저장을 위한 배열을 동적 메모리로 할당
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h> //malloc,atoi
#define NAME_LEN   20

typedef struct
{
	int acc_id;      // 계좌번호
	int balance;    // 잔    액
	char cus_name[NAME_LEN];   // 고객이름
} t_account;

void show_menu(void);       // 메뉴출력
void make_account(t_account* pa, int* pn);       // 계좌개설을 위한 함수
void deposit_money(t_account* pa, int* pn);       // 입    금
void with_draw_money(t_account* pa, int* pn);      // 출    금
void show_all_acc_info(t_account* pa, int* pn);     // 잔액조회

enum { MAKE = 1, DEPOSIT, WITHDRAW, INQUIRE, EXIT = 9 };



int main()  // int main(argc, char *argv[])
{
	t_account* acc_arr;   // Account 저장을 위한 배열
	int acc_num = 0;        // 저장된 Account 수
	int choice;

	acc_arr = (t_account *)malloc(sizeof(t_account)*NUMBER)

	while (1)
	{
		show_menu();
		printf("선택: ");
		scanf("%d", &choice);
		printf("\n");

		switch (choice)
		{
		case MAKE:
			make_account(acc_arr, &acc_num);
			break;
		case DEPOSIT:
			deposit_money(acc_arr, &acc_num);
			break;
		case WITHDRAW:
			with_draw_money(acc_arr, &acc_num);
			break;
		case INQUIRE:
			show_all_acc_info(acc_arr, &acc_num);
			break;
		case EXIT:
			free(acc_arr);
			return 0;
		default:
			printf("Illegal selection..\n");
		}
	}
	return 0;
}

void show_menu(void)
{
	char* menu[] =
	{
	 "-----Menu------\n",
	 "1. 계좌개설\n",
	 "2. 입    금\n",
	 "3. 출    금\n",
	 "4. 계좌정보 전체 출력\n",
	 "9. 종    료\n"
	};
	int i;

	for (i = 0; i < 6; i++)
		printf("%s", *(menu + i));
}

void make_account(t_account* pa, int* pn)
{
	int id;
	char name[NAME_LEN];
	int balance;

	printf("[계좌개설]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("이  름: ");
	scanf("%s", name);
	printf("입금액: ");
	scanf("%d", &balance);
	printf("\n");

	(pa + *pn)->acc_id = id;
	(pa + *pn)->balance = balance;
	strcpy((pa + *pn)->cus_name, name);
	*pn = *pn + 1;
}

void deposit_money(t_account* pa, int* pn)
{
	int money;
	int id, i;

	printf("[입    금]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("입금액: ");
	scanf("%d", &money);

	for (i = 0; i < *pn; i++)
	{
		if ((pa + i)->acc_id == id)
		{
			(pa + i)->balance += money;
			printf("입금완료\n\n");
			return;
		}
	}
	printf("유효하지 않은 ID 입니다.\n\n");
}

void with_draw_money(t_account* pa, int* pn)
{
	int money;
	int id, i;

	printf("[출    금]\n");
	printf("계좌ID: ");
	scanf("%d", &id);
	printf("출금액: ");
	scanf("%d", &money);

	for (i = 0; i < *pn; i++)
	{
		if ((pa + i)->acc_id == id)
		{
			if ((pa + i)->balance < money)
			{
				printf("잔액부족\n\n");
				return;
			}

			(pa + i)->balance -= money;  // acc_arr[i].balance = acc_arr[i].balance - money;
			printf("출금완료\n\n");
			return;
		}
	}
	printf("유효하지 않은 ID 입니다.\n\n");
}

void show_all_acc_info(t_account* pa, int* pn)
{
	int i;

	for (i = 0; i < *pn; i++)
	{
		printf("계좌ID: %d\n", (pa + i)->acc_id);
		printf("이  름: %s\n", (pa + i)->cus_name);
		printf("잔  액: %d\n\n", (pa + i)->balance);
	}
}
#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

copy_person(t_person* p, char* pname, int* page, char* paddr);
print_person(t_person* p, int n);

/*
	person 구조체를 정적 메모리 할당한다/
	사용자로 부터 이름, 나이, 주소를 입력 받는다.
	입력 받은 내용을 프린트한다.
	단, 구조체 포인터 멤버 변수를 활용 한다.
*/
#define NUMBER 10
typedef struct {

	char name[20];
	int age;
	char address[40];

} t_person;

int main(void) {

	t_person p1[NUMBER];
	char name[20];
	int age;
	int n = 0;
	char address[40];

	while (1) {

		printf("name age address(종료 : end 0 end) : ");
		scanf("%s %d %s", name, &age, address);

		if (!strcmp(name, "end", 3)) {
			print_person(p1, n);
			break;
		}
		else {
			copy_person((t_person*)p1+n, name, &age, address);
			n++; //사람수
		}

	}

	return 0;

}

copy_person(t_person* p, char* pname, int* page, char* paddr) {

	strcpy(p->name, pname); //strcpy((*p).name, pname);
	p->age = *page;
	strcpy(p->address, paddr);
}
print_person(t_person* p, int n) {
	t_person* pt = p;

	printf("name age address\n");
	printf("==== === ====\n");
	for (int i = 0;i < n;i++) {
		printf("%-6s %d   %s\n", pt->name, pt->age, pt->address); //printf("%-6s %d   %s\n", (*pt).name, (*pt).age, (*pt).address);
		pt++;
	}
}
#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
	char a[4][10] = { "horse","fox","hippo","tiger" };
	char* pa[] = { a[0],a[1],a[2],a[3] };
	int count;
	int i;
	count = sizeof(pa) / sizeof(pa[0]);
	for (i = 0;i < count;i++) {
		printf("5c", pa[i][i]);
	}
}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void input_data(int* pa, int* pb);
void swap_data(int* pa, int* pb);
void print_data(int a, int b);

int a, b;

int main(void) {

	input_data(&a, &b);
	swap_data(&a, &b);
	print_data(a, b);
	return 0;

}
void input_data(int* pa, int* pb) {

	printf("두 정수 입력 : ");
	scanf("%d %d", pa, pb);


}
void swap_data(int *pa,int*pb) {

	int temp;
	temp = *pa;
	*pa = *pb;
	*pb = temp;
	


}

void print_data(int a, int b) {
	printf("두 정수 출력: %d %d", a, b);
}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
첫번째 문자열 입력 종료(exit) : abcdef
두번째 문자열 입력 종료(exit) :defghijklmn
합친 문자열 : abcdefdefghijklmn
첫번째 문자열 입력 종료(exit) : exit
*/

merge_func(char* o, char* s1, char* s2);

int main(void) {

	char str1[80];
	char str2[80];
	char outbuff[200];

	while (1) {

		printf("\n첫번째 문자열 입력 종료(exit) :"); //abcdefg
		fgets(str1, 80, stdin);
		if (strncmp(str1, "exit", 4) == 0)
			break;

		str1[strcspn(str1, "\n")] = '\0';

		printf("\n두번째 문자열 입력 종료(exit) :"); // 1111222323
		fgets(str2, 80, stdin);
		if (strncmp(str2, "exit", 4) == 0)
			break;

		str1[strcspn(str2, "\n")] = '\0';

		merge_func(outbuff, str1, str2);
		printf("합친 문자열 : %s", outbuff);

	}

}
merge_func(char* o, char* s1, char* s2) {

/*	char* p = 0;

	for (int i = 0;i < strlen(s1) - 1;i++) {
		*p = *(st + 1);
		*p++;
	}
	for (int i = 0;i < strlen(s2) - 1;i++) {
		*p + *(s2 + 1);
		*p++;
	}*/

	strcpy(o, s1);
	strcat(o, s2);
	

}
#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>


int main(void) {
	char ch;
	int len, temp = 0;

	while (1) {

		ch = getchar();
		if (ch == -1)
			break;
		len = 0;
		while (ch != '\n') {

			len++;
			ch = getchar();
		}
		printf("len : %d", len);
		if (len > temp)
			temp = len;
		printf("\n");
	}
	printf("가장 긴 길이 : %d", temp);

	return 0;

}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*

bubble sort(이웃한 두 항목의 크기를 비교하면서 자리 변경)와 selection sort(가장 작은값 선택및 정렬 반복) 방식이 무엇인지 설명하고 구현한다.

최대 입력데이터는 10개
입력은 fgets
enter data(종료 : exit) : 10
enter data(종료 : exit) : 8
enter data(종료 : exit) : 3
enter data(종료 : exit) :exit
****input data****
 10 8 3
****bubble_sort data****
 3 8 10
****selection_sort data****
 3 8 10
*/

void bubble_sort(int arr[]);
void selection_sort(int arr[]);

void swap(double* pa, double* pb);
void line_up(double* maxp, double* midp, double* minp);

int main(void) {

	char buff[40];
	int arr[100];
	for (int i = 0; i < 100; i++) {
		arr[i] = 0;
	}

	while (1) {
		printf("enter data(종료 : exit) : ");
		fgets(buff, 40, stdin);

		if (strcmp(buff, "exit\n") == 0) {
			printf("\n입력을 종료합니다.\n");
			break;
		}
		buff[strcspn(buff, "\n")] = '\0';
		for (int i = 0; i < 10; i++) {
			if (arr[i] == 0) {       
				arr[i] = atoi(buff); 
				break;               
			}
		}
	}

	printf("****input data****\n");
	for (int i = 0; i < 10 && arr[i] != 0; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	bubble_sort(arr);
	printf("****bubble_sort data****\n");
	for (int i = 0; i < 10 && arr[i] != 0; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");


	selection_sort(arr);
	printf("****selection_sort data****\n");
	for (int i = 0; i < 10 && arr[i] != 0; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}


void bubble_sort(int arr[]) {

	for (int i = 0; i < 10 ; i++) {
		for (int j = 0; j < 10 - i; j++) {

			if (arr[j] == 0 || arr[j + 1] == 0) continue;

			if (arr[j] > arr[j + 1]) {
				int temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}


void selection_sort(int arr[]) {

	for (int i = 0; i < 10 ; i++) {
		if (arr[i] == -1) break;
		int min = i;
		for (int j = i + 1; j < 10; j++) {
			if (arr[j] == 0) break;
			if (arr[j] < arr[min]) {
				min = j;
			}
		}
		int temp = arr[i];
		arr[i] = arr[min];
		arr[min] = temp;
	}
}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void swap(double* pa, double* pb);
void line_up(double* maxp, double* midp, double* minp);

int main(void){

	double max, mid, min;

	printf("실수값 3개 입력: ");
	scanf("%lf%lf%lf", &max, &mid, &min);
	line_up(&max, &mid, &min);
	printf("정렬된 값 출력 : %.1lf, %.1lf, %.1lf\n", max, mid, min);

	return 0;
}

void swap(double* pa, double* pb) {

	double temp;
	temp = *pa;
	*pa = *pb;
	*pb = temp;
}

void line_up(double* maxp, double* midp, double* minp) {


			if (*maxp < *midp) {
				swap(maxp, midp);
			}


			if (*maxp < *minp) {
				swap(maxp, minp);
			}


			if (*midp < *minp) {
				swap(midp, minp);
			}
		}
	
#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#define NUMBER 26

void shift_alpha(char* pa);

int main(void) {

	//v포인터 : 주소를 담는 그릇
		//변수 데이터를 담는 그릇
	char a[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	printf("a ==> %s\n", a);
	shift_alpha(a);
	printf("a[0]의 주소 :%p\n", a);
	printf("a[1]의 주소 : %p\n", &a[1]);
	shift_alpha(a);
	return 0;


}

void shift_alpha(char* pa) { //pa라는 변수는 char type 메모리 번지를 저장하는 공간
	char temp;
	char* p;

	for (int i = 0; i < NUMBER + 1;i++) {
		p = pa;
		printf("%s\n", p);
		temp = *p; 

	}

}


#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include<stdlib.h>
#define NUMBER 26

int main(void) {

	//v포인터 : 주소를 담는 그릇
		//변수 데이터를 담는 그릇
	char a[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; 
	char temp;

	for(int i=0; i<NUMBER+1; i++) {

		printf("%s\n", a); 
			temp = a[0];
		for (int j = 0;j < NUMBER-1;j++) {

			a[j] = a[j + 1];
		}
		a[25] = temp;
	}


}


#endif

#if 0

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include<stdlib.h>


void print_binary(unsigned char x);

int main(void) {
	int i;
	char* c[] = {
	"NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL",
		"BS", "TAB", "LF", "VT", "FF", "CR", "SO", "SI",
		"DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB",
		"CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US" };

	printf("%60s\n", "ASCII CODE TABLE");
	printf("%60s\n", "=====================");
	printf("%5s %5s %8s %5s   %5s %5s %8s %5s   %5s %5s %8s %5s   %5s %5s %8s %5s\n",
		"DEC", "HEX", "BIN", "CHAR", "DEC", "HEX", "BIN", "CHAR",
		"DEC", "HEX", "BIN", "CHAR", "DEC", "HEX", "BIN", "CHAR");
	printf("%5s %5s %8s %5s   %5s %5s %8s %5s   %5s %5s %8s %5s   %5s %5s %8s %5s\n",
		"===", "===", "========", "====", "===", "===", "========", "====",
		"===", "===", "========", "====", "===", "===", "========", "====");

	for (i = 0; i < 32; i++) {
		printf("%5d %5.x   ", i, i); print_binary(i); printf(" %5s   ", c[i]);
		printf("%5d %5.x   ", i + 32, i + 32); print_binary(i + 32); printf(" %5c   ", i + 32);
		printf("%5d %5.x   ", i + 64, i + 64); print_binary(i + 64); printf(" %5c   ", i + 64);

		if (i + 96 == 127) {
			printf("%5d %5.x   ", i + 96, i + 96); print_binary(i + 96); printf(" %5s\n", "DEL");
		}
		else {
			printf("%5d %5.x   ", i + 96, i + 96); print_binary(i + 96); printf(" %5c\n", i + 96);
		}
	}
	return 0;
}


void print_binary(unsigned char x)
{
	int temp;
	for (int i = 7; i >= 0; i--) {
		temp = x >> i & 1;
		printf("%d", temp);
	}
}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include<stdlib.h>

print_binary(unsigned char x);

//비트연산이 가능한 정수a 입력(exit : 999) : 
//비트연산이 가능한 정수b 입력(exit : 999) : 

int main(void) {

	int a, b, c;
	char buff[40];

	while (1) {
		printf("비트연산이 가능한 정수a 입력(exit : 999) : ");
		fgets(buff, 40, stdin);
		if (strncmp(buff, "999", 3) == 0)
			break;
		a = atoi(buff);

		printf("비트연산이 가능한 정수b 입력(exit : 999) : ");
		fgets(buff, 40, stdin);
		if (strncmp(buff, "999", 3) == 0)
			break;
		b = atoi(buff);

		printf("a&b : %0x", a & b);
		print_binary(a & b);

	printf("a*b : %0x", a * b); //a와 b의 산술연산 곱셈
	print_binary(a * b);

	printf("a|b : %0x", a | b); //a와 b의 비트연산 or
	print_binary(a | b);

	printf("~a : %x", ~a); //a의 비트연산 not
	print_binary(~a);



	}
return 0;
}

print_binary(unsigned char x) 
{
	int temp;
	for (int i = 7; i >= 0; i--) {
		temp = x >> i & 1;
		printf("%d", temp);
	}
	printf("\n");
}
#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include<stdlib.h>

/*
아래와 같이 출력되는 사칙연산을 (1) switch~case문과 (2)함수로 작성
==사칙연산==
계산(+, -, *, / (종료 :9 ) :
num1 :10
num2 : 20
결과 : 10 + 20 = 30

함수
목적 : 자주쓰이는 중복 기능을 한개의 모듈로 만들어서 필요시마다 호출하여 사용하므로 code의 중복을 최소화 하기 위함이다.

매크로 : 사용자가 작성한 코드를 compile시 사용자가 지정한 곳으로 대치됨
함수와 기능이 유사하나 함수는 호출에 의해서 실행, 매크로는 사용자 code로 확장 배치된다. 
함수에 비해서 상대적으로 짧고 빠른 동작을 원할경우 매크로 사용
*/

#define ADD(a,b) ((a)+(b))
#define SUB(a,b) ((a)-(b))
#define MUL(a,b) ((a)*(b))
#define DIV(a,b) ((a)/(b))

int add(int a, int b) {
	return a + b;
}

int sub(int a, int b) {
	return a - b;
}

int multi(int a, int b) {
	return a * b;
}
int div(int a, int b) {
	return a / b;
}

int main(void) {

	char func[4];
	char buff[8];
	int num1, num2 = 0;


	printf("==사칙연산==\n");

	while (1) {
		printf("계산(+, -, *, / (종료 :9 ) : ");
		fgets(func, 4, stdin);
		if (func[0] == '9')
			break;
		printf("num1 (종료:exit) : ");	
		fgets(buff, 8, stdin);
		if (strncmp(buff, "exit", 4) == 0)
			break;
		num1 = atoi(buff);

		printf("num2 (종료:exit) : ");
		fgets(buff, 8, stdin);
		if (strncmp(buff, "exit", 4) == 0)
			break;
		num2 = atoi(buff);

		switch (func[0]) {

		case '+':
			printf("결과 : %d + %d = %d\n", num1, num2, ADD(num1, num2));
			break;
		case '-':
			printf("결과 : %d - %d = %d\n", num1, num2, SUB(num1, num2));
			break;
		case '*':
			printf("결과 : %d * %d = %d\n", num1, num2, MUL(num1, num2));
			break;
		case '/':
			printf("결과 : %d / %d = %d\n", num1, num2, DIV(num1, num2));
			break;

		default:
			printf("잘못된 operator %c\n", func[0]);
			break;

	
/*		switch (func[0]) {

		case '+':
			printf("결과 : %d + %d = %d\n", num1, num2, add(num1, num2));
			break;
		case '-':
			printf("결과 : %d - %d = %d\n", num1, num2, sub(num1, num2));
			break;
		case '*':
			printf("결과 : %d * %d = %d\n", num1, num2, multi(num1, num2));
			break;
		case '/':
			printf("결과 : %d / %d = %d\n", num1, num2, div(num1, num2));
			break;

		default:
			printf("잘못된 operator %c\n", func[0]);
			break;
		}*/
	}
}




#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

/*
----|----|----|----|----|----|----|----|----|----|----|----|----|
	     *       *       *       *       *       *       *       *      
	   2단    3단     4단     5단     6단     7단     8단     9단

	*/

int i, j;
printf("\t2단\t3단\t4단\t5단\t6단\t7단\t8단\t9단\n");

for (i = 1;i <= 10;i++) {
	for (j = 2;j < 10;j++) {
		printf("\t%d*%d=%d", j, i, j * i);
	}
}

*/
int main(void) {

}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
//scanf로 입력
//keyb 로 1~12까지 입력받는다
	//12~2 겨울
	//3~5 봄
	//6~8 여름
	//9~11 가을
	//swiitch문으로 작성

int main(void) {

	int m; //사용자로 부터 일을 이볅받아 정수로 저장하기 위한 공간(변수)


/*	while (1) {

		printf("월 (종료 : 999) : ");
		scanf("%d", &m);
		if (m == 999)
			break;
	}
	if (m >= 3 && m <= 5) {
		printf("봄\n");
	}
	else if (m >= 6 && m <= 8) {
		printf("여름\n");
	}
	else if (m >= 9 && m <= 11) {
		printf("가을\n");
	}
	else  (m == 12 || m == 1 || m == 2) {
		printf("겨울\n");
	}*/
	
	while (1) {
		printf("월 (종료 : 999) : ");
		scanf("%d", &m);
		if (m == 999)
			break;
	}
	switch (m) {
	case 12 :
	case 1:
	case 2:
		printf("겨울\n");
		break;
	case 3:	
	case 4:
	case 5:
		printf("봄\n");
	case 6:
	case 7:
	case 8:
		printf("여름\n");
	case 9:
	case 10:
	case 11:
		printf("가을\n");
		break;
	default:
		break;
	}

	}

}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

#if 1
int main(void) {
	int a = 0b0000000000000000000000000000001010; //int a=0x0a; int a = 10; 를 권장
	//2진수는 0b로 시작, 16진수는 0x로 시작, 8진수는 0으로 시작

	int b = 0b0000000000000000000000000000001100; //b에 12 0xb
	//0b0000000000000000000000000000001100; 3번 bit reset 후00000000

	int c = 0x80000001;
	//31번 비트를 reset하고, set하시오

#else //org
int a = 10;
int b = 12;

#endif

printf("a&b : %0x\n", a & b); //a와 b의 비트연산 and
printf("a*b : %0x\n", a * b); //a와 b의 산술연산 곱셈
printf("a|b : %0x\n", a | b); //a와 b의 비트연산 or
printf("~a : %x\n", ~a); //a의 비트연산 not
printf("a<<b : %0x\n", a << 1); //a의 비트연산 left shift
printf("a>>b : %0xn", a >> 2); //a의 비트연산 right shift

 //방법1 변수a의 3번 bit를 reset 시킨다.

printf("a변수의 3번 bit를 reset 시키기 : %0x\n", a &= 0xfffffff7);
//방법2 변수b의 3번 bit를 reset시킨다.(shift연산자 이용)

printf("b변수의 3번 bit를 reset 시키기 : %0x\n", b &= ~(1 << 3)); //r결과가 4가된다.
printf("a ==> %0x\n", a);
printf("b ==> %0x\n", b);

//a변수와 b변수의 3번 비트를 set하기 
//아래의 패턴 활용, led를 on할때 
a |= 1 << 3;
b |= 1 << 3;
printf("a ==> %0x\n", a);
printf("b ==> %0x\n", b);

c &= ~(1 << 31);   // reset
printf("c ==> %0x\n", c);
c |= (1 << 31);   // set

printf("c ==> %0x\n", c);

return 0;
}

#endif

#if 0

	/*
	
	----|----|----|----|----|----|----|----|
                ASCII CODE TABLE
               ==================

	DEC      HEX      OCT      CHAR
	====     ====     ====     ====
	48	     0x30				0

	*/
#include <stdio.h>

int main(void) {
	int i;

	char* c[] = {
			"NUL","SOH","STX","ETX","EOT","ENQ","ACK","BEL",
			"BS","TAB","LF","VT","FF","CR","SO","SI",
			"DLE","DC1","DC2","DC3","DC4","NAK","SYN","ETB",
			"CAN","EM","SUB","ESC","FS","GS","RS","US"
		};
	printf("%50s\n", "ASCII CODE TABLE");
	printf("%50s\n", "=====================");

	printf("%5s %5s %5s %5s   %5s %5s %5s %5s   %5s %5s %5s %5s   %5s %5s %5s %5s\n",
		"DEC", "HEX", "OCT", "CHAR", "DEC", "HEX", "OCT", "CHAR",
		"DEC", "HEX", "OCT", "CHAR", "DEC", "HEX", "OCT", "CHAR");
	printf("%5s %5s %5s %5s   %5s %5s %5s %5s   %5s %5s %5s %5s   %5s %5s %5s %5s\n",
		"===", "===", "===", "====", "===", "===", "===", "====",
		"===", "===", "===", "====", "===", "===", "===", "====");

	for (i = 0; i < 32; i++) {

		if (i < 32)  
		printf("%5d %5.x %5o %5s   ", i, i, i, c[i]);
		printf("%5d %5.x %5o %5c   ", i + 32, i + 32, i + 32, i + 32);
		printf("%5d %5.x %5o %5c   ", i + 64, i + 64, i + 64, i + 64);
		if (i + 96 == 127) {
			printf("%5d %5.x %5o %5s\n", i + 96, i + 96, i + 96, "DEL");
		}
		else {
			printf("%5d %5.x %5o %5c\n", i + 96, i + 96, i + 96, i + 96);
		}
	}

	return 0;
}
#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>

int main(void) {

	int a, b, c, sum;
	double avg;

	while (1) {
		printf("성적을 입력 하시오\n");
		printf("국어(종료 0):");
		scanf("%d", &a);
		if (a == 0)
			break;
		printf("영어(종료 0):");
		scanf("%d", &b);
		if (b == 0)
			break;
		printf("수학(종료 0):");
		scanf("%d", &c);
		if (c == 0)
			break;

		printf("****점수****\n");
		sum = a + b + c;
		avg = sum / 3;

		printf("국어 영어 수학 총점 평균\n");
		printf("%d  %d  %d  %d  %lf\n\n", a, b, c, sum, avg);
	}

}

#endif

#if 0
#define _CRT_SECURE_NO_WARNINGS
//나이와 키가 0,0이면 종료
#include<stdio.h>
int main(void) {

	int age;
	double height;

	while (1) {
		printf("나이와 키를 입력하세요(00입력시 종료) : ");
		scanf("%d%lf", &age, &height);
		if (age == 0 && height == 0)
			break;
		printf("나이는 %d살 키는 %.1lfcm입니다.\n", age, height);
	}
}

#endif

#if 0
//아래의 1,2를 반복수행t
// 1.임의의 문자를 키보드로부터 입력 받고 
// 입력 문자가 종료문자인 'q'이다.
// 2.입력 문자를 아스키 HEX DEC OCT로 출력
#include<stdio.h>
int main(void) {

	char buff[10];

	while (1) { //무한반복

		printf("임의의 문자를 입력(종료 : q)\n");
		fgets(buff, 10, stdin); //stdid : keyboard로 부터 최대 10자까지 입력을 받거나 enter key를 만날때까지 입력받는다.
		if (buff[0] == 'q')
			break; //가장 가까운 반복문 탈출
		printf("입력 문자 %c의 acsii HEX : x%0x, DEX : %d, OCT : %o\n",buff[0], buff[0], buff[0], buff[0]);

	}

}

#endif

#if 0

	/*
	
	----|----|----|----|----|----|----|----|
                ASCII CODE TABLE
               ==================

	DEC      HEX      OCT      CHAR
	====     ====     ====     ====
	48	     0x30				0

	*/
#include <stdio.h>
int main(void) {

	int i;
	//pirntf("%30s\n", "ASCII CODE TABLE"); //총 30byte를 출력하고 나머지는 space로 채운다.
	printf("%30s\n", "ASCII CODE TABLE");
	printf("%30s\n", "==================");

	for (i = 0; i < 128;i++) {

		if (i % 30 == 0) //나머지 30라인마다 헤더를 출력
		{
			printf("%10s%10s%10s%10s\n", "DEC", "HEX", "OCT", "CHAR");
			printf("%10s%10s%10s%10s\n", "===", "===", "===", "====");

		}
		printf("%10d%10.x%10o%10c\n", i, i, i, i);

	}
	return 0;

}

#endif

#if 0
#include<stdio.h>
int main(void) {

	char a,b,c;
	int i = 10;

	printf("i ==> %d\n", i); //10을 10진수로 출력
	printf("%d\n", 10);

	//10을 0ctal(8진수)로 출력
	printf("octal i = %o\n", 10);
	printf("10 octal %o\n", 10);

	//10을 hax로 출력 : a
	printf("hax i = %0x\n", i);
	printf("10 hax : %0x\n", 10);

	printf("%lf\n", 3.4); //소수점 이하 6자리까지 출력
	printf("%.1lf\n", 3.452);
	printf("%.10lf\n", 3.4444);

	printf("%lf %.1lf %.10lf\n", 3.4, 3.452, 3.4444);

	a = 'A'; //a 라는 변수에 대문자 A값을 넣는다
		// a ='A'; null이 들어갈 공간 확보해야함
	b = 0x41;
	c = 0b01000001;

	printf("a : %c\n", a);
	printf("b : %c\n", b);
	printf("c : %c\n", c);

	//아스키 코드 10, 13 암기 


	return 0;
}
#endif

#if 0

#include<stdio.h>

int main(void) {

	// char c[10] = "abcd"; 배열의 크기는 2의 배수로 잡기*^^* 나중에 문제 생길수있음 
	char c[6] = "abcd";
	unsigned char uc[20] = "abcde";

	short s[40]; //80byte 
	unsigned short us[30]; //60

	int i[10]; //40
	long j[20]; //40

	printf("c ==> %d\n", (int)sizeof(c));
	printf("uc ==> %d\n", (int)sizeof(uc));

	printf("s ==> %d\n", (int)sizeof(s));
	printf("us ==> %d\n", (int)sizeof(us));

	printf("i ==> %d\n", (int)sizeof(i));
	printf("j ==> %d\n", (int)sizeof(j));

}

#endif

#if 0
#include <stdio.h>
int main(void) {

	//변수(variable) : data를 저장하는 메모리(ram)공간
	//상수 : 자료 값이 고정되어 있는것(ram or rom)
	char c; //1byte c라는 이름으로 1byte(8bit)를 저장하는 메모리 공간 
	//-127~127
	unsigned char uc;//1byte 0~255 0x
	unsigned char ub = 0b11111111; //uc의 초기값과 동일 0xff ob : 2진수
	short s; //s라는 이름해서 2byte 짜리 저장공간을 확보
	unsigned short us; // 2byte 0~65535 0xffff
		// 255,65535 암기
	int i; //i라는 이름으로 해서 4byte짜리 저장공간 확보
	long j;//j라는 이름으로 해서 4byte짜리 저장공간 확보

	int a[4];


	uc = 0xff; //255
	printf("hex uc ==> %d\n", uc);
	uc = 0b11111111;
	printf("bin uc ==> %d\n", uc);
	uc = 255;
	printf("dec uc ==> %d\n", uc);

	uc++; //uc=uc+1
	//carry out = 오버플로우 돼서 빠져나간 값 01010+010101 이진수 계산에서
	printf("uc++ ==> %d\n", uc); //uc++ ==> 0 오버플로우 발생으로 0
	printf("uc++ ==> %0x\n", uc); //uc++ ==> 
	printf("uc++ ==> %o\n", uc);

	s = -1;
	printf("s ==> %d\n", s); // s==> -1
	printf("s ==> %u\n", s); // s==> 65535. 4294967295 32byte로 찍힘 dec

	us = 0xffff;//65535
	printf("us ==> %d\n", us); //65535
	printf("us ==> %0x\n", us); //0xffff

	us++;
	printf("us++ ==> %d\n", us);
	printf("us++ ==> %0x\n", us); //0

	printf("char : %d\n", (int) sizeof(char)); //sizeof : 변수의 크기(byte)를 인지 계산
	printf("unsigned char: %d\n", (int)sizeof(unsigned char)); //1byte

	printf("short : %d\n", (int)sizeof(short)); //sizeof : 변수의 크기(byte)를 인지 계산
	printf("unsigned short: %d\n", (int)sizeof(unsigned short)); //2byte

	printf("int : %d\n", (int)sizeof(int)); //sizeof : 변수의 크기(byte)를 인지 계산
	printf("unsigned int: %d\n", (int)sizeof(unsigned int)); //4byte

	printf("a : %d\n", (int)sizeof(a)); //16byte

	return 0; //

}

#endif






