#include <linux/sched.h>

/** Return 1 iff the process is restricted from calling this syscall according
 * to its restriction_level and restriction_list. */
int is_restricted(long int syscall)
{
	if (current->restriction_enabled) {
		printk("%d %d: ", current->pid, syscall);
		int i;
		for (i=0; i < current->restrictions_count; ++i) {
			printk("[%d]", i);
			printk("%d ", current->restrictions[i].syscall_num);
			if (current->restrictions[i].syscall_num == syscall) {
				printk("%d blocked\n",syscall);
				//tamuz log
				return 1;
			}
		}
		printk("\n");
	}
	return 0;
}
