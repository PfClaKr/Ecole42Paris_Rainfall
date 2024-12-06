#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *p(void)
{
	char buffer[64];
	void *eip;

	fflush(stdout);
	gets(buffer);
	eip = some_function_get_eip_address();
	if ((eip & 0xb0000000) == 0xb0000000)
	{
		printf("%p\n", eip);
		exit(1);
	}
	puts(buffer);
	return (strdup(buffer));
}

int main(void)
{
	p();
	return (0);
}