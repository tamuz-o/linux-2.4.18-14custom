#include <asm/uaccess.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>

/* Return 1 iff process is SHORT.
 * If PID doesn't exist, return -ESRCH. */
int sys_is_short(pid_t pid)
{
	//tamuz
}

/* Return the process's remaining timeslice, in ms.
 * If PID doesn't exist return -ESRCH.
 * If process isn't SHORT return -EINVAL. */
int sys_short_remaining_time(pid_t pid)
{
	//tamuz
}

/* Return the number of SHORT process scheduled to run strictly before the given process.
 * If PID doesn't exist return -ESRCH.
 * If process isn't SHORT return -EINVAL. */
int sys_short_place_in_queue(pid_t pid)
{
	//tamuz
}
