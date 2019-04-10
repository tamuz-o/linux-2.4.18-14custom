#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>

int sys_sc_restrict (pid_t pid, int proc_restriction_level, scr* restrictions_list,
		int list_size)
{
	//tamuz remove printk
	/* Assert valid input */
	printk("sc %d %d %d\n", pid, proc_restriction_level, list_size);
	if (pid < 0) {
		printk("pid %d < 0\n", pid);
		return -ESRCH;
	}
	task_t *task = find_task_by_pid(pid);
	if (!task) {
		printk("task not found\n");
		return -ESRCH;
	}
	if (proc_restriction_level < 0 || proc_restriction_level > 2 || list_size < 0) {
		printk ("invalid\n");
		return -EINVAL;
	}
	/* Allocate restrictions_list if necessary */
	scr* restrictions = NULL;
	if (list_size) {
		restrictions = (scr*)kmalloc(list_size * sizeof(scr), GFP_KERNEL);
		if (!restrictions) {
			printk("malloc fail\n");
			return -ENOMEM;
		}
		if (copy_from_user(restrictions, restrictions_list, list_size * sizeof(scr))) {
			printk("copy from user fail\n");
			kfree(task->restrictions);
			return -ENOMEM;
		}
	}
	/* Allocate log if not done previously */
	if (!task->fai_log) {
		task->fai_log = (fai*)kmalloc(FAI_LOG_SIZE * sizeof(fai), GFP_KERNEL);
		if (!task->fai_log) {
			printk("fai malloc fail\n");
			kfree(restrictions);
			return -ENOMEM;
		}
	}
	/* Deallocate previous restrictions list if exists */
	if (task->restrictions) {
		kfree(task->restrictions);
	}
	/* Set the remaining properties */
	task->restrictions = restrictions;
	task->restrictions_count = list_size;
	task->restriction_level = proc_restriction_level;
	return 0;
}

int sys_set_proc_restriction (pid_t pid, int proc_restriction_level)
{
	printk("set %d %d\n", pid, proc_restriction_level);
	if (pid < 0) {
		printk("pid %d < 0\n", pid);
		return -ESRCH;
	}
	task_t *task = find_task_by_pid(pid);
	if (!task) {
		printk("task not found\n");
		return -ESRCH;
	}
	if (proc_restriction_level < 0 || proc_restriction_level > 2) {
		printk("invalid\n");
		return -EINVAL;
	}
	task->restriction_level = proc_restriction_level;
	return 0;
}

int sys_get_process_log (pid_t pid, int size, fai* user_mem)
{
	printk("log %d %d\n", pid, size);
	if (pid < 0) {
		printk("pid %d < 0\n", pid);
		return -ESRCH;
	}
	task_t *task = find_task_by_pid(pid);
	if (!task) {
		printk("task not found\n");
		return -ESRCH;
	}
	if (size == 0) {
		return 0;
	}
	if (size > FAI_LOG_SIZE || (!task->fai_full && size > task->fai_next) || size < 0) {
		printk("requested too much\n");
		return -EINVAL;
	}
	fai *last_fai = task->fai_log + task->fai_next - 1;
	if (task->fai_full) {
		if (size <= task->fai_next) {
			if (copy_to_user(user_mem, last_fai - size + 1, size*sizeof(fai))) {
				printk("copy fail\n");
				return -ENOMEM;
			}
		} else {
			int extra = size - task->fai_next;
			if (copy_to_user(user_mem, task->fai_log + FAI_LOG_SIZE - extra, extra*sizeof(fai))
					|| copy_to_user(user_mem+extra, task->fai_log, task->fai_next * sizeof(fai))) {
				printk("copy fail\n");
				return -ENOMEM;
			};
		}
	} else {
		if (copy_to_user(user_mem, last_fai - size + 1, size*sizeof(fai))) {
			printk("copy fail\n");
			return -ENOMEM;
		}
	}
	return 0;
}
