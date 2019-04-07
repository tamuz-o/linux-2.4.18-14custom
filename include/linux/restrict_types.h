#ifndef LINUX_2_4_18_14CUSTOM_RESTRICT_TYPES_H
#define LINUX_2_4_18_14CUSTOM_RESTRICT_TYPES_H

typedef struct sys_call_restrictor {
	int syscall_num;
	int restriction_threshold;
} scr;

typedef struct forbidden_activity_info {
	int syscall_num;
	int syscall_restriction_threshold;
	int proc_restriction_level;
	int time;
} fai;

typedef struct {
	scr* restrictions;
	int count;
} scr_list_t;

typedef void* fai_log_t;  //tamuz

#endif //LINUX_2_4_18_14CUSTOM_RESTRICT_TYPES_H
