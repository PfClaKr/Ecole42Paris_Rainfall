#include <unistd.h>
#include <iostream>
#include <cstring>

class N
{
	int nb;
	char annotation[100];
	int (N::*func)(N &);

	N(int value) : nb(value)
	{
		this->func = &N::operator+;
	}

	int operator+(N &rhs)
	{
		return N(rhs.nb + this->nb);
	}

	int operator-(N &rhs)
	{
		return N(this->nb - rhs.nb);
	}

	void setAnnotation(char *str)
	{
		memcpy(this->annotation, str, strlen(str));
		return ;
	}
};

int main(int ac, char **av)
{
	if (ac < 1)
		_exit(1);

	N *a = new N(5);
	N *b = new N(6);

	a->setAnnotation(av[1]);
	return (b->*(b->func))(*a);
}