#include <stdio.h>

void run(void)
{
	fwrite("Good... Wait what?\n", 1, 19, stdout);
	system("/bin/sh");
	return;
}

int main(void)
{
	char str[64];

	gets(str);
	return (0);
}