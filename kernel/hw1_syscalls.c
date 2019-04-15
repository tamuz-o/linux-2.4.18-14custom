
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <linux/types.h>
#include <linux/errno.h>



#define LOG_SIZE 100


/* system call number 243 */
int sys_sc_restrict(pid_t pid, int proc_restriction_level, scr* restriction_list, int list_size){
	struct task_struct* p = find_task_by_pid(pid);
	scr* temp_list = NULL;
	int WasRestricted = 1; // 1 - was , 0 - not
	if(!p){
		return -ESRCH;
	}

	if(proc_restriction_level<0 || proc_restriction_level>2){
		 return -EINVAL;
	 }
	if(p->logArray == NULL){ // allocating new log array only if the log array isn't allocated yet
		WasRestricted = 0;
		p->logArray = (struct log_array*)kmalloc(sizeof(struct log_array),GFP_KERNEL);
		if(!p->logArray){
			return -ENOMEM;
		}
		p->logArray->array=(fai*)kmalloc(LOG_SIZE* sizeof(fai), GFP_KERNEL);
		if(!p->logArray->array){
			kfree(p->logArray);
			return -ENOMEM;
		}
		p->logArray->indexWrite=0;
		p->logArray->size=LOG_SIZE;
		p->logArray->num_of_logs = 0;
		p->logArray->full=0;
	}
	if(list_size >0){
		temp_list = (scr*)kmalloc(sizeof(*p->restrictionList)*list_size,GFP_KERNEL);
		if(!temp_list){
			if(WasRestricted == 0){ // returning the last state of the process
				kfree(p->logArray->array);
				kfree(p->logArray);
				p->logArray = NULL;
			}
			return -ENOMEM;
		}
		
		if(copy_from_user(temp_list, restriction_list, (list_size)* sizeof(scr)) != 0){
			if(WasRestricted == 0){ // returning the last state of the process
				kfree(p->logArray->array);
				kfree(p->logArray);
				p->logArray = NULL;
			}
			kfree(temp_list);
			return -ENOMEM;
		}
	}
	if(p->restrictionList != NULL){ // if the restriction list is allready allocated should first offall release it 
		kfree(p->restrictionList);
		p->restrictionList = NULL;
	}

	p->restrictionList = temp_list;
	p->restriction_list_size = list_size;
    p->restriction_level = proc_restriction_level;
	return 0;
}


/* system call number 244 */
int sys_set_proc_restriction(pid_t pid, int proc_restriction_level){
	struct task_struct* p = find_task_by_pid(pid);
	if(!p){
		return -ESRCH;
	}

	
	if(proc_restriction_level<0 || proc_restriction_level>2){
		 return -EINVAL;
	}
	
	p->restriction_level = proc_restriction_level;
	
	return 0;
}



/* system call number 245 */
int sys_get_process_log(pid_t pid, int size, fai* user_mem){
	
	int write_curr;
    int currRead;
    fai* tempArray;
	struct task_struct* p = find_task_by_pid(pid);
	
	
	if(!p){
		return -ESRCH;
	}
	if(p->logArray == NULL){
		return -EINVAL;
	}
	if(size > p->logArray->size || size < 0 || size > p->logArray->num_of_logs){
		return -EINVAL;
	}
	
	tempArray = (fai*)kmalloc(size* sizeof(fai),GFP_KERNEL);
    if(!tempArray){
        return -ENOMEM;
    }
	currRead = p->logArray->indexWrite-1;
	if(p->logArray->full == 1 && currRead == -1){ // the array is full read from write index-1
		currRead = p->logArray->size-1;
	}
    for(write_curr = size-1; write_curr >= 0; --write_curr){
        tempArray[write_curr] = p->logArray->array[currRead];
		if(currRead == 0){ 
            currRead = p->logArray->size;
        }
        --currRead;


    }

    if(copy_to_user(user_mem, tempArray, (size)* sizeof(fai)) != 0){
        kfree(tempArray);
        return -ENOMEM;
    }
	
    kfree(tempArray);
	return 0;
}




