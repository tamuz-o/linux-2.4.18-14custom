#include <linux/sched.h>

/** Return 1 iff the process is restricted from calling this syscall according
 * to its restriction_level and restriction_list. */
int is_restricted(int syscall, pid_t pid)
{
	//tamuz
	if (current->restriction_enabled) {
		printk("%d\n", current->pid);
	}
	return 0;
}
