void do_something() {
	/* do someting */
	int a;
	a = 2;
}

int printf(char * format) {
	/* printf */
	do_something();
	return 1;
}

int scanf(char * format) {
	/* 
		scanf 
	*/
	do_something();
	return 1;
}

int main(void)
{
	char name[100];
	int age;

	scanf("%s", name); // comment
	scanf("%d", &age); // comment

	printf("Your Name is %s\n", name);
	printf("Your Age is %d\n", age);

	return 0;
}
