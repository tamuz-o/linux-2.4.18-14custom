#include <sys/types.h>
#include <sys/times.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sched.h>
#include "hw2_syscalls.h"

long get_my_runtime ()
{
	struct tms t;
	times(&t);
	return t.tms_utime + t.tms_stime;
}

long ms_to_ticks(int ms) {return ms/10;}

void busy_wait (long ms)
{
	long end = times(0) + ms_to_ticks(ms);
	while (times(0) < end) ;
}

void print_repeat (long interval_ms, int repeats, const char* text)
{
	int i;
	long t1 = times(0);
	interval_ms = ms_to_ticks(interval_ms);
	for (i=0; i<repeats; ++i) {
		t1 += interval_ms;
		while (times(0) < t1) ;
		printf(text);
		fflush(stdout);
	}
}

void set_short (pid_t pid, int rtime, int prio)
{
	struct sched_param param = {.sched_priority=-1,
			.requested_time=rtime, .sched_short_prio=prio};
//	sched_setscheduler(pid, SCHED_SHORT, &param);
	int res;
	__asm__(
	"int $0x80;"
	: "=a" (res)
	: "0" (156), "b" (pid), "c" (SCHED_SHORT), "d" (&param)
	: "memory"
	);
	if (res < 0) {
		errno = (-res);
	}
}

int main() {
	pid_t sp = fork();
	if (!sp) {
		shortp(sp);
		printf("\n");
	} else {
		longp(sp);
		printf("\n");
	}
	return 0;
}

void longp(pid_t son_pid) {
	set_short(son_pid, 2000, 130);
	print_repeat(500, 6, "L");
}

void shortp(pid_t my_pid) {
	print_repeat(300, 4, "S");
}
