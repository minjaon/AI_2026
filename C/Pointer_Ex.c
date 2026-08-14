/***********************************************************/
// [0] 컴파일러 테스트
/***********************************************************/

#if 1

#include <stdio.h>

int main(void)
{
	printf("Hello C\n");

	return 0;
}

#endif

/***********************************************************/
// [1] 주소 타입의 승계
/***********************************************************/

#if 0

#include <stdio.h>

int main(void)
{
	int *p, int *q;
	
	p = (int *)0x1008;
	q = (int *)0x1000;
	
	printf("%X\n", p * q);
	printf("%X\n", p / q);
	printf("%X\n", p + q);
	printf("%X\n", p - q);
	
	printf("%X\n", p * 2);
	printf("%X\n", p / 2);
	printf("%X\n", p + 2);
	printf("%X\n", p - 2);	

	return 0;	
}

#endif

/***********************************************************/
// [2] 배열 parameter
/***********************************************************/

#if 0

#include <stdio.h>

int sum(int b[4])
{
	int i, sum = 0;

	for(i=0; i<(sizeof(b)/sizeof(b[0])); i++)
	{
		sum += b[i];
	}
	return sum;
}

int main(void)
{
	int a[4] = {1,2,3,4};

	printf("%d\n", sum(a));

	return 0;	
}

#endif

/***********************************************************/
// [3] 배열 활용식을 이용한 배열 요소 액세스
/***********************************************************/

#if 0 

#include <stdio.h>

int a[4] = {1,2,3,4};
	
int main(void)
{
	printf("%d\n", a[0]);
	printf("%d\n", a[3]);
	printf("%d\n", a[4]);
	printf("%d\n", a[-1]);

	printf("%d\n", (a+1)[2]);
	printf("%d\n", a[3]);

	printf("%d\n", a[2]);
	printf("%d\n", *(a+2));
	printf("%d\n", *(2+a));
	printf("%d\n", 2[a]);
	
	return 0;	
}

#endif

/**********************************************************/
// [4] : 무엇이 인쇄될까요?
/**********************************************************/

#if 0

#include <stdio.h>

int main(void)
{
	char a[4] = "LEW";
	char *p = a;

	printf("%X:%X \n", p, &p);
	printf("%X:%X \n", a, &a);
	printf("%X:%X \n", main, &main);
	printf("%X:%X \n", "LEW", &"LEW");
	
	return 0;	
}

#endif

/**********************************************************/
// [5] : 포인터 배열
/**********************************************************/

#if 0

#include <stdio.h> 

int x[4] = {1,2,3,4};

int main(void)
{
	int *a[4] = {x+3, x+2, x+1, x};
	
	printf("%d\n", x[2]);	

	// 배열 a를 이용하여 x[2]를 30으로

	     = 30;   

	printf("%d\n", x[2]);
	
	return 0;	
}

#endif

/**********************************************************/
// [6] : 대치법 활용
/**********************************************************/

#if 0

#include <stdio.h> 

int x[4] = {1,2,3,4};

int *f1(void)
{
	return x;
}

void f2(int *p)
{
	printf("%d == %d == %d == %d\n", x[2], *(x+2), p[0], *p);
}

int main(void)
{
	int *p;
	int *a[4] = {x+3, x+2, x+1, x};

	p=x;

	printf("%d == %d\n", x[2], p[2]);
	printf("%d == %d == %d == %d\n", x[2], *(x+2), a[3][2], *a[1]);
	printf("%d == %d == %d == %d\n", x[2], *(x+2), f1()[2], *(f1()+2));
	f2(x+2);
	
	return 0;	
}

#endif

/***********************************************************/
// [7] : 구조체는 기본형이다
/***********************************************************/

#if 0

#include <stdio.h>

int main(void) 
{
	int a, b = 10;

	a = b;
	printf("a=%d, b=%d\n", a, b);
	
	return 0;	
}

#endif

#if 0

#include <stdio.h>

int main(void) 
{
	struct st
	{
		int a;
		char b;
	} y = {100, 'A'};

	struct st x = y;

	printf("x.a=%d, x.b=%c\n", x.a, x.b);
	printf("y.a=%d, y.b=%c\n", y.a, y.b);
	
	return 0;	
}

#endif

/**********************************************************/
// [8] : 구조체의 함수 전달
/**********************************************************/

#if 0 

#include <stdio.h> 

struct math
{
	int id;
	char name[20];
	int score;
}mid, final={1, "Kim", 50};

int main(void)
{
	mid = final;
	
	printf("%d\n", mid.id);
	printf("%s\n", mid.name);
	printf("%d\n", mid.score);
	
	return 0;	
}

#endif

#if 0

#include <stdio.h>

struct math
{
	int id;
	char name[20];
	int score;
};

void cheat(struct math test);

int main(void)
{
	struct math final={1, "Kim", 50};
	cheat(final);
	printf("%d\n", final.score);
	
	return 0;	
}

void cheat(struct math test)
{
	test.score = 100;
}

#endif

/**********************************************************/
// [9] : 구조체 주소의 함수 전달
/**********************************************************/

#if 0

#include <stdio.h> 
 
struct math
{
	int id;
	char name[20];
	int score;
};

void cheat(struct math * test);

int main(void)
{
	struct math final={1, "Kim", 50};
	cheat(&final);
	printf("%d\n", final.score);
	
	return 0;	
}

void cheat(struct math * test)
{
			 = 100;
}

#endif

/**********************************************************/
// [10] : 2차원배열의 전달
/**********************************************************/

#if 0

#include <stdio.h>

void draw_pixel(int y, int x, int value,      p     )
{
	p[y][x] = value;
}

int main(void)
{
	int a[2][3] = {1,2,3,4,5,6};

	printf("%d\n", a[1][2]);
	draw_pixel(1, 2, 10, a);
	printf("%d\n", a[1][2]);
	
	return 0;	
}

#endif

/**********************************************************/
// [11] : 2차원배열의 리턴
/**********************************************************/

#if 0

#include <stdio.h>

       func(void)
{
	static int a[3][4] = {1,2,3,4,5,6,7,8,9,10,11,12};
	return a;
}

int main(void)
{
	printf("%d\n",     func()        );
	
	return 0;	
}

#endif

/**********************************************************/
// [12] : 함수의 실행
/**********************************************************/

#if 0

#include <stdio.h> 

int add(int a, int b)
{
	return a+b;
}

int main(void)
{
	printf("%d\n", add(3,4));
	printf("%d\n", (&add)(3,4));
	printf("%d\n", (*add)(3,4));
	printf("%d\n", (**add)(3,4));
	
	return 0;	
}

#endif

/***********************************************************/
// [13] : 함수 등가포인터의 실행
/***********************************************************/

#if 0

#include <stdio.h>

int add(int a, int b)
{
	return a+b;
}	

void f1(void)
{
	printf("func\n");
}

int * f2(void)
{
	static int a[4] = {1,2,3,4};

	return a;
}

int main(void)
{
	// p, q, r 선언

	// p, q, r에 대응 함수 대입

	printf("%d\n", add(3,4));
	f1();
	printf("%d\n", f2()[2]);

	// 위와 동일한 결과가 나오도록 p, q, r로 실행

}	
	return 0;	
}

#endif

/***********************************************************/
// [14] : 함수를 함수에 전달하자
/***********************************************************/

#if 0

#include <stdio.h> 

int add(int a, int b)
{
	return a+b;
}

int sub(int a, int b)
{
	return a-b;
}

void func(                  )
{
	printf("%d\n", p(3,4));
}

int main(void)
{
	func(add);
	func(sub);
	
	return 0;	
}
#endif

/***********************************************************/
// [15] : 조건에 따른 함수 호출 엔진
/***********************************************************/

#if 0

#include <stdio.h> 
#include <stdlib.h> 

int add(int a, int b)
{
	return a+b;
}

int sub(int a, int b)
{
	return a-b;
}

int mul(int a, int b)
{
	return a*b;
}

int get_key(void)
{
	return rand() % 3;
}

int op(int a, int b)
{
	switch(get_key())
	{
		case 0	: return add(a,b);
		case 1	: return sub(a,b);
		case 2	: return mul(a,b);
	}
}

int main(void)
{
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	
	return 0;	
}

#endif

/***********************************************************/
// [16] : 함수 Lookup table
/***********************************************************/

#if 0

#include <stdio.h> 
#include <stdlib.h> 

int add(int a, int b)
{
	return a+b;
}

int sub(int a, int b)
{
	return a-b;
}

int mul(int a, int b)
{
	return a*b;
}

int get_key(void)
{
	return rand() % 3;
}

       fa[3]        = {add, sub, mul};

int op(int a, int b)
{
	return fa[get_key()](a,b);
}

int main(void)
{
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	printf("%d\n", op(3, 4));
	
	return 0;	
}

#endif
