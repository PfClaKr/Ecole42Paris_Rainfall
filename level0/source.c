#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int ac, char **av)
{
	int nb;
	char *argv[2];
	gid_t gid;
	uid_t uid;

	nb = atoi(av[1]);
	if (nb == 423)
	{
		argv[0] = strdup("/bin/sh");
		argv[1] = NULL;

		gid = getegid();
		uid = geteuid();
		setresgid(gid, gid, gid);
		setresuid(uid, uid, uid);

		execv("/bin/sh", argv);
	}
	else
		fwrite("No !\n", 1, 5, stderr);
	return (0);
}