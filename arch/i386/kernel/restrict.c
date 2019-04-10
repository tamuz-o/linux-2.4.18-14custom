#include <linux/sched.h>

/** Return 1 iff the process is restricted from calling this syscall according
 * to its restriction_level and restriction_list. */
asmlinkage long is_restricted(long int syscall)
{
	if (current->restriction_enabled) {
//		printk("%d %d: ", current->pid, syscall);
		int i;
		for (i=0; i < current->restrictions_count; ++i) {
			scr *restr = &current->restrictions[i];
//			printk("[%d]", i);
//			printk("%d ", restr->syscall_num);
			if (restr->syscall_num == syscall) {
				if (restr->restriction_threshold > current->restriction_level) {
//					printk("%d blocked\n", syscall);
					/* Log forbidden activity: */
					fai *entry = current->fai_log + (current->fai_next++);
					if (current->fai_next == 100) {
						current->fai_full = 1;
						current->fai_next = 0;
					}
					entry->syscall_num = syscall;
					entry->syscall_restriction_threshold = restr->restriction_threshold;
					entry->proc_restriction_level = current->restriction_level;
					entry->time = jiffies;
					return 1;
				} else {
					break;
				}
			}
		}
//		printk("\n");
	}
	return 0;
}
