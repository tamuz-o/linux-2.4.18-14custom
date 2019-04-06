#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/restrict_types.h>

int sys_sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list,
		int list_size)
{
	//tamuz too many arguments, must use stack
	printk("sc %d %d %d\n", pid, proc_restriction_level, list_size);
}

int sys_set_proc_restriction (pid_t pid, int proc_restriction_level)
{
	printk("set %d %d\n", pid, proc_restriction_level);
}

int sys_get_process_log (pid_t pid, int size, fai* user_mem)
{
	printk("log %d %d\n", pid, size);
}
