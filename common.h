/* See LICENSE file for copyright and license details. */
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "arg.h"

#define USAGE(SYNOPSIS)\
	static void usage(void)\
	{ eprintf("usage: %s%s%s\n", argv0, *SYNOPSIS ? " " : "", SYNOPSIS); }

void eprintf(const char *fmt, ...);

void bff_begin(void);
void bff_end(void);
pid_t bff_run(const char **args, int in, int out);
void bff_wait(pid_t pid, const char *cmd);
