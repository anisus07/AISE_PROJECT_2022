#ifndef _SIGNAUX_H_
#define _SIGNAUX_H_

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <elf.h>
#include <execinfo.h>

siginfo_t ptrace_getsiginfo(pid_t target);
void ptrace_signal(siginfo_t targetsig);
void bt_sighandler(int sig, siginfo_t *info, void *secret);

#endif