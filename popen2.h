/*
Copyright 2021 Helena Jäger

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
#ifndef POPEN2_H
#define POPEN2_H
#include <stdio.h>
#include <unistd.h>

int popen2(char* cmd, FILE** in, FILE** out) {
	pid_t cp;
	int stdin_pipe[2];
	int stdout_pipe[2];
	if (pipe(stdin_pipe) < 0) {return -1;}
	if (pipe(stdout_pipe) < 0) {return -1;}
	cp = fork();
	if (cp < 0) {return -1;}
	else if (cp == 0) {
		close(stdin_pipe[1]);
		close(stdout_pipe[0]);
		dup2(stdin_pipe[0], 0);
		dup2(stdout_pipe[1], 1);
		execl("/bin/sh", "sh", "-c", cmd, NULL);
		return -1;
	}
	*in = fdopen(stdin_pipe[1], "w");
	*out = fdopen(stdout_pipe[0], "r");
	close(stdin_pipe[0]);
	close(stdout_pipe[1]);
	return 0;
}
#endif
