#include <sys/types.h>
#include <sys/times.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "hw2_syscalls.h"

int main() {
	nice(-20);
	pid_t sp = fork();
	if (!sp)
		shortp();
	else
		longp();
	return 0;
}

void longp() {
	long t1 = times(0);
	while (times(0) < t1 + 100);
}

void shortp() {
	nice(+19);
	printf("p");
}
