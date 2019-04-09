#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "hw1_syscalls.h"

//int sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list, int list_size)
//int set_proc_restriction (pid_t pid, int proc_restriction_level)
//int get_process_log (pid_t pid, int size, fai* user_mem)

int main()
{
	pid_t pid = fork();
	if (!pid) { // son
		char cwd[256];
		while(1) {
			sleep(2);
//			printf("x");
			getcwd(cwd, 256);
		}
	} else {
		printf("%d\n", pid);
		sc_restrict(pid, 0, 0, 0);
		while (1);
	}
	return 0;
}
