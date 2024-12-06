#include <stdlib.h>
#include <stdio.h>

int m;

void v(void)
{
	char buffer[512];

	fgets(buffer, 512, stdin);
	printf(buffer);
	if (64 == m) {
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
	return ;
}

int main(void)
{
	v();
	return (0);
}