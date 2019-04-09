#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>

int sys_sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list,
		int list_size)
{
	printk("sc %d %d %d\n", pid, proc_restriction_level, list_size);  //tamuz remove
	if (pid < 0) {
		return -ESRCH;
	}
	if (proc_restriction_level < 0 || proc_restriction_level > 2 || list_size < 0) {
		return -EINVAL;
	}
	task_t *task = find_task_by_pid(pid);  //tamuz excp
	if (list_size) {
		task->restrictions = (scr*)kmalloc(list_size * sizeof(scr), GFP_KERNEL);
		if (!task->restrictions) {
			return -ENOMEM;
		}
		if (copy_from_user(task->restrictions, restrictions_list, list_size * sizeof(scr))) {
			kfree(task->restrictions);
			return -ENOMEM;
		}
	}
	task->restriction_enabled = 1;
	task->restriction_level = proc_restriction_level;
	task->restrictions_count = list_size;
	return 0;
}

int sys_set_proc_restriction (pid_t pid, int proc_restriction_level)
{
	printk("set %d %d\n", pid, proc_restriction_level);
}

int sys_get_process_log (pid_t pid, int size, fai* user_mem)
{
	printk("log %d %d\n", pid, size);
}
