#include <linux/types.h>
#include <linux/restrict_types.h>

/** Return 1 iff the process is restricted from calling this syscall according
 * to its restriction_level and restriction_list. */
int is_restricted(int syscall, pid_t pid)
{
	//tamuz
	printk("is_restricted %d %d\n", syscall, pid);
	return 0;
}
