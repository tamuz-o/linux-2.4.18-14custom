#include <linux/sched.h>

int sys_sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list,
		int list_size)
{
	printk("sc %d %d %d\n", pid, proc_restriction_level, list_size);
	task_t *task = find_task_by_pid(pid);
	task->restriction_enabled = 1;
}

int sys_set_proc_restriction (pid_t pid, int proc_restriction_level)
{
	printk("set %d %d\n", pid, proc_restriction_level);
}

int sys_get_process_log (pid_t pid, int size, fai* user_mem)
{
	printk("log %d %d\n", pid, size);
}
