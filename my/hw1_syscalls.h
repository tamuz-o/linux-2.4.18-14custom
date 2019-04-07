#ifndef LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H
#define LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H

#include <errno.h>
//#include <linux/kernel.h> tamuz
//#include <linux/types.h>
#include <sys/types.h>

typedef struct sys_call_restrictor {
	int syscall_num;
	int restriction_threshold;
} scr;

typedef struct forbidden_activity_info {
	int syscall_num;
	int syscall_restriction_threshold;
	int proc_restriction_level;
	int time;
} fai;

typedef struct {
	scr* restrictions;
	int count;
} scr_list_t;

int sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list,
					 int list_size)  // syscall 243
{
	int res;
	scr_list_t list {restrictions_list, list_size};
	//tamuz too many arguments, use stack
	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (243), "b" (pid), "c" (proc_restriction_level), "d" (&list)
			: "memory"
			);
	if (res < 0) {
		errno = (-res);
		return -1;
	}
	return res;
}

int set_proc_restriction (pid_t pid, int proc_restriction_level)  // syscall 244
{
	int res;
	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (244), "b" (pid), "c" (proc_restriction_level)
			: "memory"
	);
	if (res < 0) {
		errno = (-res);
		return -1;
	}
	return res;
}

int get_process_log (pid_t pid, int size, fai* user_mem)  // syscall 245
{
	int res;
	__asm__(
			"int $0x80;"
			: "=a" (res)
			: "0" (245), "b" (pid), "c" (size), "d" (user_mem)
			: "memory"
	);
	if (res < 0) {
		errno = (-res);
		return -1;
	}
	return res;
}

#endif //LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H
