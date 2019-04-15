#ifndef LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H
#define LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H


#include <errno.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct sys_call_restriction{
	int syscall_num;
	int restriction_threshold;
}scr;

typedef struct forbidden_activity_info{
	int syscall_num;
	int syscall_restriction_threshold;
	int proc_restriction_level;
	int time;
} fai;


struct log_array{
	int indexWrite;
	int size;
	int num_of_logs;
	int full;
    fai* array;
};


/* system call number 243 */
int sc_restrict(pid_t pid, int proc_restriction_level, scr* restriction_list, int list_size){
    unsigned int res;
     if(pid<0){
         errno=ESRCH;
         return -1;
     }
     if(list_size<0){
         errno=EINVAL;
         return -1;
     }


    __asm__(
    "int $0x80;"
    : "=a" (res)
    : "0" (243), "b" (pid), "c" (proc_restriction_level), "d" (restriction_list), "S" (list_size)
    : "memory"
    );
    if(res==0){
        return 0;
    }
	
    if(res >= (unsigned long)(-125)){
        errno = -res;
    }
	
    return -1;
}

/* system call number 244 */
int set_proc_restriction(pid_t pid, int proc_restriction_level){
	 unsigned int res;
     if(pid<0){
         errno=ESRCH;
         return -1;
     }
	 
	 __asm__(
    "int $0x80;"
    : "=a" (res)
    : "0" (244), "b" (pid), "c" (proc_restriction_level)
    : "memory"
    );
	
    if(res==0){
        return 0;
    }
	if(res >= (unsigned long)(-125)){
        errno = -res;
    }
	
    return -1;
}



/* system call number 245 */
int get_process_log(pid_t pid, int size, fai* user_mem){
	 unsigned int res;
     if(pid<0){
         errno=ESRCH;
         return -1;
     }
	 if(size==0){
		return 0;
	}

	 __asm__(
    "int $0x80;"
    : "=a" (res)
    : "0" (245), "b" (pid), "c" (size), "d" (user_mem)
    : "memory"
    );
	
    if(res==0){
        return 0;
    }
	if(res >= (unsigned long)(-125)){
        errno = -res;
    }
	
    return -1;
}


#endif //LINUX_2_4_18_14CUSTOM_HW1_SYSCALLS_H

