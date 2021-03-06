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
	long end = get_my_runtime() + ms_to_ticks(ms);
	while (get_my_runtime() < end) ;
}

void print_repeat (long interval_ms, int repeats, const char* text)
{
	int i;
	long t1 = get_my_runtime();
	interval_ms = ms_to_ticks(interval_ms);
	for (i=0; i<repeats; ++i) {
		t1 += interval_ms;
		while (get_my_runtime() < t1) ;
		printf(text);
		fflush(stdout);
//		write(1, text, 1);
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

int main()
{
	test_short_is_complete();
	printf("\n");
	sleep(4);
	test_short_timeslice_finished();
	printf("\n");
	return 0;
}

int test_short_is_complete() {
	pid_t pid = fork();
	if (pid) {  /*father*/
		set_short(/*getpid()*/pid, 3000, 130);
		print_repeat(200, 6, "L");
	} else {
		print_repeat(100, 4, "S");
	}
	return 0;
}

int test_short_timeslice_finished() {
	pid_t pid = fork();
	if (pid) {  /*father*/
		set_short(/*getpid()*/pid, 3000, 130);
		print_repeat(200, 6, "L");
	} else {
		print_repeat(500, 8, "S");
	}
	return 0;
}
