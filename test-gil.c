#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <wait.h>
#include <stdlib.h>
#include "hw1_syscalls.h"

#define STRESS_TEST

#define assertTest(expression)\
    do {\
        if (!(expression)) {\
            printf("Assertion failed at %s:%d: "\
                   "in function \"%s\" "\
                   "with assertion \"%s\".\n",\
                   __FILE__, __LINE__, __func__, (#expression));\
        }\
    } while (0)

#define assertFail(index, syscall, syscall_threshold, proc_restriction)\
    do {\
        assertTest(fail[(index)].syscall_num == (syscall));\
        assertTest(fail[(index)].syscall_restriction_threshold == (syscall_threshold));\
        assertTest(fail[(index)].proc_restriction_level == (proc_restriction));\
        assertTest(fail[(index)].time > 0);\
    } while (0)

static void callTestFunction(void (*testFunc)());

static void reset();

static void restrict_fail_pidNegative();

static void restrict_fail_procPrivNegative();

static void restrict_fail_procPrivGreaterThan2();

static void restrict_success_scrlNull();

static void restrict_fail_sizeNegative();

static void restrict_fail_returnESRCHBeforeEINVAL();

static void restrict_success_simple();

static void restrict_success_overrideScrDifferentPriv();

static void restrict_success_overrideMoreScr();

static void restrict_success_overrideLessScr();

static void restrict_success_restrictForOther();

static void restrict_success_sonHasNoRestrictions();

static void exampleTest();

static void set_proc_restriction_fail_pidNegative();

static void set_proc_restriction_fail_procPrivNegative();

static void set_proc_restriction_fail_procPrivGreaterThan2();

static void set_proc_restriction_fail_returnESRCHBeforeEINVAL();

static void set_proc_restriction_success();

static void get_process_log_fail_pidNegative();

static void get_process_log_fail_sizeMoreThanLogHas_hasNoneRequest1();

static void get_process_log_fail_sizeMoreThanLogHas_has1Request2();

static void get_process_log_fail_sizeMoreThan100();

static void get_process_log_fail_sizeNegative();

static void get_process_log_fail_returnESRCHBeforeEINVAL();

static void get_process_log_success_simple();

static void get_process_log_success_moreThan100();

static void get_process_log_success_failSorted();

static void get_process_log_success_sonHasNoLogs();

static void test_zombieRetainInfo();

static void stress_restrict_override();

static void stress_restrictions_list();

static void stress_user_mem();

int main() {
    /* Must succeed restrict_success_sonHasNoRestrictions() before other tests. */
    callTestFunction(restrict_success_sonHasNoRestrictions);
    callTestFunction(get_process_log_success_sonHasNoLogs);

    callTestFunction(restrict_fail_pidNegative);
    callTestFunction(restrict_fail_procPrivNegative);
    callTestFunction(restrict_fail_procPrivGreaterThan2);
    callTestFunction(restrict_success_scrlNull);
    callTestFunction(restrict_fail_sizeNegative);
    callTestFunction(restrict_fail_returnESRCHBeforeEINVAL);
    callTestFunction(restrict_success_simple);
    callTestFunction(restrict_success_overrideScrDifferentPriv);
    callTestFunction(restrict_success_overrideMoreScr);
    callTestFunction(restrict_success_overrideLessScr);
    callTestFunction(restrict_success_restrictForOther);
    callTestFunction(exampleTest);
    callTestFunction(set_proc_restriction_fail_pidNegative);
    callTestFunction(set_proc_restriction_fail_procPrivNegative);
    callTestFunction(set_proc_restriction_fail_procPrivGreaterThan2);
    callTestFunction(set_proc_restriction_fail_returnESRCHBeforeEINVAL);
    callTestFunction(set_proc_restriction_success);
    callTestFunction(get_process_log_fail_pidNegative);
    callTestFunction(get_process_log_fail_sizeMoreThanLogHas_hasNoneRequest1);
    callTestFunction(get_process_log_fail_sizeMoreThanLogHas_has1Request2);
    callTestFunction(get_process_log_fail_sizeMoreThan100);
    callTestFunction(get_process_log_fail_sizeNegative);
    callTestFunction(get_process_log_fail_returnESRCHBeforeEINVAL);
    callTestFunction(get_process_log_success_simple);
    callTestFunction(get_process_log_success_moreThan100);
    callTestFunction(get_process_log_success_failSorted);
    callTestFunction(test_zombieRetainInfo);

#ifdef STRESS_TEST
    callTestFunction(stress_restrict_override);
    callTestFunction(stress_restrictions_list);
    callTestFunction(stress_user_mem);  /* Long. */
#endif

    printf("End.\n");
    return 0;
}

static void callTestFunction(void (*testFunc)()) {
    reset();
    testFunc();
    reset();
}

static void reset() {
    pid_t pid = fork();
    if (pid != 0) {
        exit(0);
    }
}

static void restrict_fail_pidNegative() {
    scr scrl[1];
    assertTest(sc_restrict(-1, 0, scrl, 0) == -1);
    assertTest(errno == ESRCH);
}

static void restrict_fail_procPrivNegative() {
    scr scrl[1];
    assertTest(sc_restrict(getpid(), -1, scrl, 0) == -1);
    assertTest(errno == EINVAL);
}

static void restrict_fail_procPrivGreaterThan2() {
    scr scrl[1];
    assertTest(sc_restrict(getpid(), 3, scrl, 0) == -1);
    assertTest(errno == EINVAL);
}

static void restrict_success_scrlNull() {
    scr scrl[] = {{13, 0}}; /* 13 = time. */
    assertTest(sc_restrict(getpid(), 0, scrl, 1) == 0);

    assertTest(sc_restrict(getpid(), 0, NULL, 0) == 0);
}

static void restrict_fail_sizeNegative() {
    scr scrl[1];
    assertTest(sc_restrict(getpid(), 0, scrl, -1) == -1);
    assertTest(errno == EINVAL);
}

static void restrict_fail_returnESRCHBeforeEINVAL() {
    assertTest(sc_restrict(-1, -1, NULL, -1) == -1);
    assertTest(errno == ESRCH);
}

static void restrict_success_simple() {
    scr scrl[] = {{13, 0},  /* 13 = time. */
                  {20, 1},  /* 20 = getpid. */
                  {64, 2}}; /* 64 = getppid. */
    assertTest(sc_restrict(getpid(), 1, scrl, 3) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() == -1);
    assertTest(errno == ENOSYS);
}

static void restrict_success_overrideScrDifferentPriv() {
    scr scrl[] = {{13, 0},  /* 13 = time. */
                  {20, 1},  /* 20 = getpid. */
                  {64, 2}}; /* 64 = getppid. */

    assertTest(sc_restrict(getpid(), 1, scrl, 3) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() == -1);
    assertTest(errno == ENOSYS);

    assertTest(sc_restrict(getpid(), 2, scrl, 3) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() != -1);
}

static void restrict_success_overrideMoreScr() {
    scr scrl1[] = {{13, 0},  /* 13 = time. */
                   {20, 1}}; /* 20 = getpid. */

    scr scrl2[] = {{13, 0},  /* 13 = time. */
                   {20, 1},  /* 20 = getpid. */
                   {64, 2}}; /* 64 = getppid. */

    assertTest(sc_restrict(getpid(), 1, scrl1, 2) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() != -1);

    assertTest(sc_restrict(getpid(), 1, scrl2, 3) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() == -1);
    assertTest(errno == ENOSYS);
}

static void restrict_success_overrideLessScr() {
    scr scrl1[] = {{13, 0},  /* 13 = time. */
                   {20, 1},  /* 20 = getpid. */
                   {64, 2}}; /* 64 = getppid. */

    scr scrl2[] = {{13, 0},  /* 13 = time. */
                   {20, 1}}; /* 20 = getpid. */

    assertTest(sc_restrict(getpid(), 1, scrl1, 3) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() == -1);
    assertTest(errno == ENOSYS);

    assertTest(sc_restrict(getpid(), 1, scrl2, 2) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() != -1);
}

static void restrict_success_restrictForOther() {
    pid_t sonsPid;

    scr scrl[] = {{13, 0},  /* 13 = time. */
                  {20, 1},  /* 20 = getpid. */
                  {64, 2}}; /* 64 = getppid. */

    sonsPid = fork();
    if (sonsPid == 0) {
        assertTest(sc_restrict(getppid(), 1, scrl, 3) == 0);
        exit(0);
    } else {
        waitpid(sonsPid, NULL, 0);
        assertTest(time(NULL) != -1);
        assertTest(getpid() != -1);
        assertTest(getppid() == -1);
        assertTest(errno == ENOSYS);
    }
}

static void restrict_success_sonHasNoRestrictions() {
    pid_t sonPid;

    scr scrl[] = {{13, 2},  /* 13 = time. */
                  {20, 1},  /* 20 = getpid. */
                  {64, 0}}; /* 64 = getppid. */

    assertTest(sc_restrict(getpid(), 1, scrl, 3) == 0);

    sonPid = fork();
    if (sonPid == 0) {
        assertTest(getpid() != -1);
        assertTest(getppid() != -1);
        assertTest(time(NULL) != -1);
        exit(0);
    } else {
        assertTest(getppid() != -1);
        assertTest(getpid() != -1);
        assertTest(time(NULL) == -1);
        assertTest(errno == ENOSYS);
    }
}

static void get_process_log_success_sonHasNoLogs() {
    fai fail[1];
    pid_t sonPid;

    scr scrl[] = {{13, 2}};  /* 13 = time. */
    assertTest(sc_restrict(getpid(), 1, scrl, 1) == 0);

    assertTest(time(NULL) == -1);
    assertTest(errno == ENOSYS);

    sonPid = fork();
    if (sonPid == 0) {
        assertTest(get_process_log(getpid(), 1, fail) == -1);
        assertTest(errno == EINVAL);
        exit(0);
    } else {
        assertTest(get_process_log(getpid(), 1, fail) == 0);
        assertFail(0, 13, 2, 1);
    }
}

static void exampleTest() {
    pid_t pid = getpid();
    scr scrl = {20, 2}; /* 20 = getpid. */
    assertTest(sc_restrict(pid, 0, &scrl, 1) == 0);
    assertTest(getpid() == -1);
    assertTest(errno == ENOSYS);
    assertTest(set_proc_restriction(pid, 2) == 0);
    assertTest(getpid() != -1);
}

static void set_proc_restriction_fail_pidNegative() {
    assertTest(set_proc_restriction(-1, 0) == -1);
    assertTest(errno == ESRCH);
}

static void set_proc_restriction_fail_procPrivNegative() {
    assertTest(set_proc_restriction(getpid(), -1) == -1);
    assertTest(errno == EINVAL);
}

static void set_proc_restriction_fail_procPrivGreaterThan2() {
    assertTest(set_proc_restriction(getpid(), 3) == -1);
    assertTest(errno == EINVAL);
}

static void set_proc_restriction_fail_returnESRCHBeforeEINVAL() {
    assertTest(set_proc_restriction(-1, -1) == -1);
    assertTest(errno == ESRCH);
}

static void set_proc_restriction_success() {
    scr scrl[1];
    assertTest(set_proc_restriction(getpid(), 0) == 0);
    assertTest(sc_restrict(getpid(), 1, scrl, 0) == 0);
    assertTest(set_proc_restriction(getpid(), 2) == 0);
}

static void get_process_log_fail_pidNegative() {
    fai fail[1];
    assertTest(get_process_log(-1, 0, fail) == -1);
    assertTest(errno == ESRCH);
}

static void get_process_log_fail_sizeMoreThanLogHas_hasNoneRequest1() {
    fai fail[1];
    assertTest(get_process_log(getpid(), 1, fail) == -1);
    assertTest(errno == EINVAL);
}

static void get_process_log_fail_sizeMoreThanLogHas_has1Request2() {
    fai fail[2];
    scr scrl[] = {{13, 1}}; /* 13 = time. */

    assertTest(sc_restrict(getpid(), 0, scrl, 1) == 0);
    assertTest(time(NULL) == -1);

    assertTest(get_process_log(getpid(), 2, fail) == -1);
    assertTest(errno == EINVAL);
}

static void get_process_log_fail_sizeMoreThan100() {
    fai fail[101];
    assertTest(get_process_log(getpid(), 101, fail) == -1);
    assertTest(errno == EINVAL);
}

static void get_process_log_fail_sizeNegative() {
    fai fail[1];
    assertTest(get_process_log(getpid(), -1, fail) == -1);
    assertTest(errno == EINVAL);
}

static void get_process_log_fail_returnESRCHBeforeEINVAL() {
    fai fail[1];
    assertTest(get_process_log(-1, -1, fail) == -1);
    assertTest(errno == ESRCH);
}

static void get_process_log_success_simple() {
    fai fail[5];
    pid_t pid = getpid();
    scr scrl[] = {{13, 0},  /* 13 = time. */
                  {20, 1},  /* 20 = getpid. */
                  {64, 2}}; /* 64 = getppid. */

    assertTest(sc_restrict(pid, 1, scrl, 3) == 0);
    assertTest(get_process_log(pid, 0, fail) == 0);

    assertTest(time(NULL) != -1);
    assertTest(getpid() != -1);
    assertTest(getppid() == -1);
    assertTest(errno == ENOSYS);
    assertTest(getppid() == -1);
    assertTest(errno == ENOSYS);
    assertTest(get_process_log(pid, 2, fail) == 0);

    assertFail(0, 64, 2, 1);
    assertFail(1, 64, 2, 1);

    assertTest(set_proc_restriction(pid, 0) == 0);
    assertTest(time(NULL) != -1);
    assertTest(getpid() == -1);
    assertTest(errno == ENOSYS);
    assertTest(getppid() == -1);
    assertTest(errno == ENOSYS);
    assertTest(getpid() == -1);
    assertTest(errno == ENOSYS);

    assertTest(get_process_log(pid, 2, fail) == 0);
    assertFail(0, 64, 2, 0);
    assertFail(1, 20, 1, 0);

    assertTest(get_process_log(pid, 5, fail) == 0);
    assertFail(0, 64, 2, 1);
    assertFail(1, 64, 2, 1);
    assertFail(2, 20, 1, 0);
    assertFail(3, 64, 2, 0);
    assertFail(4, 20, 1, 0);
}

static void get_process_log_success_moreThan100() {
    int i;
    pid_t pid = getpid();
    fai fail[100];

    scr scrl[] = {{13, 2},  /* 13 = time. */
                  {20, 1},  /* 20 = getpid. */
                  {64, 0}}; /* 64 = getppid. */

    assertTest(sc_restrict(pid, 0, scrl, 3) == 0);
    for (i = 0; i < 60; i++) {
        assertTest(time(NULL) == -1);
    }
    for (i = 0; i < 60; i++) {
        assertTest(getpid() == -1);
    }

    /* 40 of time and 60 of getpid. */
    assertTest(get_process_log(pid, 100, fail) == 0);
    for (i = 0; i < 100; i++) {
        if (i < 40) {
            assertFail(i, 13, 2, 0);
        } else {
            assertFail(i, 20, 1, 0);
        }
    }
}

static void get_process_log_success_failSorted() {
    pid_t pid = getpid();
    fai fail[5];

    scr scrl[] = {{13, 2},  /* 13 = time. */
                  {20, 2},  /* 20 = getpid. */
                  {64, 2}}; /* 64 = getppid. */

    assertTest(sc_restrict(pid, 0, scrl, 3) == 0);

    assertTest(getpid() == -1);
    assertTest(time(NULL) == -1);
    assertTest(getppid() == -1);
    assertTest(getpid() == -1);

    assertTest(get_process_log(pid, 3, fail) == 0);
    assertFail(0, 13, 2, 0);
    assertFail(1, 64, 2, 0);
    assertFail(2, 20, 2, 0);

    assertTest(fail[0].time <= fail[1].time);
    assertTest(fail[0].time <= fail[2].time);
    assertTest(fail[1].time <= fail[2].time);
}

static void test_zombieRetainInfo() {
    pid_t sonPid;
    sonPid = fork();
    if (sonPid == 0) {

        scr scrl[] = {{13, 2}}; /* 13 = time. */
        assertTest(sc_restrict(getpid(), 1, scrl, 1) == 0);
        assertTest(time(NULL) == -1);
        assertTest(time(NULL) == -1);
        exit(0);

    } else {
        fai fail[2];
        sleep(1);
        assertTest(get_process_log(sonPid, 2, fail) == 0);
        assertFail(0, 13, 2, 1);
        assertFail(1, 13, 2, 1);
    }
}

static void stress_restrict_override() {
    int i;
    int memError;
    pid_t pid;
    scr scrl[10000];

    for (i = 0; i < 10000; i++) {
        scrl[i].syscall_num = 13; /* time = 13. */
        scrl[i].restriction_threshold = 0;
    }

    memError = 0;
    pid = getpid();
    for (i = 0; i < 10000; i++) {
        sc_restrict(pid, 2, scrl, 10000);
        if (errno == ENOMEM) {
            memError = 1;
            break;
        }
    }
    assertTest(memError == 0);
}

static void stress_restrictions_list() {
    int i;
    int memError;
    scr scrl[10000];

    for (i = 0; i < 10000; i++) {
        scrl[i].syscall_num = 13; /* time = 13. */
        scrl[i].restriction_threshold = 0;
    }

    memError = 0;
    for (i = 0; i < 10000; i++) {
        sc_restrict(getpid(), 2, scrl, 10000);
        if (errno == ENOMEM) {
            memError = 1;
            break;
        }
        if (fork() != 0) {
            exit(0);
        }
    }
    assertTest(memError == 0);
}

/* Stuck on fail. */
static void stress_user_mem() {
    int i;
    scr scrl[10000];

    for (i = 0; i < 10000; i++) {
        scrl[i].syscall_num = 13; /* time = 13. */
        scrl[i].restriction_threshold = 2;
    }

    for (i = 0; i < 150000; i++) {
        sc_restrict(getpid(), 0, scrl, 1);
        time(NULL);
        if (fork() != 0) {
            exit(0);
        }
    }
}
