/* See LICENSE file for copyright and license details. */
#include "common.h"

USAGE("[-s | -S | -t] width height left top")

int
main(int argc, char *argv[])
{
	pid_t pid;
	char *args[6] = {"blind-crop"};
	char flag[2] = {0, 0};
	int i = 1;

	ARGBEGIN {
	case 's':
	case 'S':
	case 't':
		if (*flag != ARGC())
			usage();
		*flag = ARGC();
		break;
	} ARGEND;

	if (argc != 4)
		usage();

	bff_begin();

	if (*flag)
		args[i++] = flag;
	args[i++] = argv[0];
	args[i++] = argv[1];
	args[i++] = argv[2];
	args[i++] = argv[3];
	pid = bff_run(args, STDIN_FILENO, STDOUT_FILENO);

	bff_wait(pid, "blind-crop");

	bff_end();

	return 0;
}
