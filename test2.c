// 전역 변수 선언
int val = 3;

// enum 선언
typedef enum foo
{
	A = 10
} FOO;

// 구조체 선언
typedef struct hello
{
	int world;
	char is;
	float mine;
} HELLO;

// 함수 선언
int my_printf(char *b, ...)
{
	/*
	 * 지역 변수 선언
	 */
	int loc = 20, i;

	// 대입
	loc = 100 + val;
	val++;

	// 탈출 문자, 함수 호출
	if (val < 10)
	{
		my_printf("local var: %d\n", loc);
	}

	// 반복문
	for (i = 0; i < val; i++)
	{
		printf("%d\n", loc + i);
	}

	// 리턴문
	return 0;
}
