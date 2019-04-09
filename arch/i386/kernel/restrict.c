#include <linux/sched.h>

/** Return 1 iff the process is restricted from calling this syscall according
 * to its restriction_level and restriction_list. */
int is_restricted(long int syscall)
{
	if (current->restriction_enabled) {
//		for (int i=0; i < current->restrictions_count; ++i) {
//
//		}
		printk("%d %d\n", current->pid, syscall);
	}
	return 0;
}
