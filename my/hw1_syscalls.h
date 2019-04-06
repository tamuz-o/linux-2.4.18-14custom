#ifndef LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H
#define LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H
#include <linux/kernel.h>

int sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list,
					 int list_size)  // syscall 243
{

}

int set_proc_restriction (pid_t pid, int proc_restriction_level)  // syscall 244
{

}

int get_process_log (pid_t pid, int size, fai* user_mem)  // syscall 245
{

}

#endif //LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H
