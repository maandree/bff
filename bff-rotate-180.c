/* See LICENSE file for copyright and license details. */
#include "common.h"

USAGE("")

int
main(int argc, char *argv[])
{
	pid_t pid;

	UNOFLAGS(argc);

	bff_begin();

	pid = bff_run((const char *[]){ "blind-rotate-180", NULL },
		      STDIN_FILENO, STDOUT_FILENO);

	bff_wait(pid, "blind-rotate-180");

	bff_end();

	return 0;
}
