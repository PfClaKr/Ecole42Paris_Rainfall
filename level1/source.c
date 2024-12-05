#include <stdio.h>

void run(void)
{
	fwrite("Good... Wait what?\n", 19, 1, stdout);
	system("/bin/sh");
	return;
}

int main(void)
{
	char str[0x40]; // 0x50 - 0x10

	gets(str);
	return (0);
}