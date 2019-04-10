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
	scr scrs[3];
	scrs[0].syscall_num = 174;
	scrs[1].syscall_num = 175;
	scrs[2].syscall_num = 162;
	scrs[0].restriction_threshold = scrs[1].restriction_threshold = scrs[2].restriction_threshold = 2;
	if (!pid) { // son
		//getcwd:  183.  sleep: 174 175 162
		//char cwd[256];
		while(1) {
			sleep(2);
//			printf("x");
			//getcwd(cwd, 256);
		}
	} else {
		printf("%d\n", pid);
		sc_restrict(pid, 0, scrs, 3);
		while (1);
	}
	return 0;
}
