#include <stdio.h>
#include "hw1_syscalls.h"

//int sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list, int list_size)
//int set_proc_restriction (pid_t pid, int proc_restriction_level)
//int get_process_log (pid_t pid, int size, fai* user_mem)

int main()
{
	printf("1\n");
//	sc_restrict(1000, 2, 0, 0);
	printf("2\n");
	set_proc_restriction(1001, 1);
	printf("3\n");
	get_process_log(1002, 0, 0);
	printf("4\n");
	return 0;
}
