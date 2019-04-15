#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <linux/types.h>
#include <linux/errno.h>


void updateLog(pid_t pid, int syscall_num, int restriction_threshold);


/* checks if the system call is allowed to be called or not
	-1 - fail
	the system call number - success
*/
int IsSysCallAllowed(int syscall_num, struct task_struct* p){
	if(!p){
		return -1;
	}
	if(p->pid < 2){
			return syscall_num;
	}
	int i = 0;
	
	for(i = 0; i < p->restriction_list_size ; i++){
		if(p->restrictionList[i].syscall_num == syscall_num && p->restrictionList[i].restriction_threshold > p->restriction_level){
			updateLog(p->pid,syscall_num,p->restrictionList[i].restriction_threshold);
			return -1;
		}
	}
	return syscall_num;
}


// HELP FUNCTION 


	/* updating the log array if the system call is not allowed to be called */
void updateLog(pid_t pid, int syscall_num, int restriction_threshold){
	struct task_struct* p = find_task_by_pid(pid);
		/* adding log record to the logArray */
		p->logArray->array[p->logArray->indexWrite].syscall_num=syscall_num;
		p->logArray->array[p->logArray->indexWrite].syscall_restriction_threshold = restriction_threshold;
		p->logArray->array[p->logArray->indexWrite].proc_restriction_level = p->restriction_level;
		p->logArray->array[p->logArray->indexWrite].time=jiffies;
		++(p->logArray->indexWrite);
		if(p->logArray->num_of_logs < p->logArray->size){
			++(p->logArray->num_of_logs);
		}
		if(p->logArray->indexWrite==p->logArray->size){
			p->logArray->indexWrite=0;
			p->logArray->full=1;
		}
}
