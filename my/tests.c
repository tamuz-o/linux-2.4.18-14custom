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
		son();
	} else {
		father(pid);
	}
	return 0;
}

void son()
{
	//getcwd:  183.  sleep: 174 175 162
	//char cwd[256];
	int i;
	for (i=0; i<5; ++i) {
		sleep(2);
	}
	//getcwd(cwd, 256);
}

void father(pid_t spid)
{
	scr scrs[3];
	scrs[0].syscall_num = 174;
	scrs[1].syscall_num = 175;
	scrs[2].syscall_num = 162;
	scrs[0].restriction_threshold = scrs[1].restriction_threshold = scrs[2].restriction_threshold = 2;
	printf("%d\n", spid);
	sc_restrict(pid, 0, scrs, 3);
	sleep(2);
}
