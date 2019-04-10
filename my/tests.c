#include <sys/types.h>
#include <errno.h>
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
		sonf();
	} else {
		fatherf(pid);
	}
	return 0;
}

int sonf()
{
	//getcwd:  183.  sleep: 174 175 162
	//char cwd[256];
	int i;
	for (i=0; i<105; ++i) {
		sleep(2);
	}
	//getcwd(cwd, 256);
	return 0;
}

int fatherf(pid_t spid)
{
	int LOGSIZE = 7;
	scr scrs[3];
	scrs[0].syscall_num = 174;
	scrs[1].syscall_num = 175;
	scrs[2].syscall_num = 162;
	scrs[0].restriction_threshold = scrs[1].restriction_threshold = scrs[2].restriction_threshold = 2;
	printf("%d\n", spid);
	if (sc_restrict(3000, 0, 0, 0) < 0) {
		printf("fail to restrict pid 3000, errno %d\n", errno);
	}
	sc_restrict(spid, 0, scrs, 3);
	sleep(1);
	fai log[LOGSIZE];
	if (get_process_log(3000, LOGSIZE, log) < 0) {
		printf("fail to retrive from pid 3000, errno %d\n", errno);
	}
	get_process_log(spid, LOGSIZE, log);
	int i;
	for (i=0; i<LOGSIZE; ++i) {
		printf("%d\n", log[i].time);
	}
	return 0;
}
