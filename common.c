/* See LICENSE file for copyright and license details. */
#include "common.h"

char *argv0;

static pid_t image_decoder_pid;
static pid_t image_encoder_pid;


void
eprintf(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	if (argv0 && strncmp(fmt, "usage", strlen("usage")))
		fprintf(stderr, "%s: ", argv0);

	vfprintf(stderr, fmt, ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	}

	va_end(ap);
	exit(1);
}


void
bff_begin(void)
{
	int rw[2];

	if (pipe(rw))
		eprintf("pipe:");
	switch (image_decoder_pid = fork()) {
	case -1:
		eprintf("fork:");
		break;
	case 0:
		if (prctl(PR_SET_PDEATHSIG, SIGKILL))
			eprintf("prctl PR_SET_PDEATHSIG SIGKILL:");
		close(rw[0]);
		if (dup2(rw[1], STDOUT_FILENO) < 0)
			eprintf("dup2 <pipe> <stdout>:");
		close(rw[1]);
		execlp("blind-from-image", "blind-from-image", "-f", NULL);
		eprintf("execlp blind-from-image:");
		break;
	default:
		break;
	}
	close(rw[1]);
	if (dup2(rw[0], STDIN_FILENO) < 0)
		eprintf("dup2 <pipe> <stdin>:");
	close(rw[0]);

	if (pipe(rw))
		eprintf("pipe:");
	switch (image_encoder_pid = fork()) {
	case -1:
		eprintf("fork:");
		break;
	case 0:
		if (prctl(PR_SET_PDEATHSIG, SIGKILL))
			eprintf("prctl PR_SET_PDEATHSIG SIGKILL:");
		close(rw[1]);
		if (dup2(rw[0], STDIN_FILENO) < 0)
			eprintf("dup2 <pipe> <stdin>:");
		close(rw[0]);
		execlp("blind-to-image", "blind-to-image", "-f", NULL);
		eprintf("execlp blind-to-image:");
		break;
	default:
		break;
	}
	close(rw[0]);
	if (dup2(rw[1], STDOUT_FILENO) < 0)
		eprintf("dup2 <pipe> <stdout>:");
	close(rw[1]);
}


void
bff_end(void)
{
	int status;

	if (waitpid(image_decoder_pid, &status, 0) < 0)
		eprintf("waitpid blind-from-image:");
	if (status)
		exit(1);

	if (waitpid(image_encoder_pid, &status, 0) < 0)
		eprintf("waitpid blind-to-image:");
	if (status)
		exit(1);
}


pid_t
bff_run(const char **args, int in, int out)
{
	pid_t pid;

	switch (pid = fork()) {
	case -1:
		eprintf("fork:");
		break;
	case 0:
		if (prctl(PR_SET_PDEATHSIG, SIGKILL))
			eprintf("prctl PR_SET_PDEATHSIG SIGKILL:");
		if (in != STDIN_FILENO) {
			if (dup2(in, STDIN_FILENO) < 0)
				eprintf("dup2 <pipe> <stdin>:");
			close(in);
		}
		if (out != STDOUT_FILENO) {
			if (dup2(out, STDOUT_FILENO) < 0)
				eprintf("dup2 <pipe> <stdout>:");
			close(out);
		}
		execvp(*args, (void *)args);
		eprintf("execlp %s:", *args);
		break;
	default:
		break;
	}
	close(in);
	close(out);

	return pid;
}


void
bff_wait(pid_t pid, const char *cmd)
{
	int status;
	if (waitpid(pid, &status, 0) < 0)
		eprintf("waitpid %s:", cmd);
	if (status)
		exit(1);
}
