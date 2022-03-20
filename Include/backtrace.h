#ifndef _BACKTRACE_H_
#define _BACKTRACE_H_

#include <sys/ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <libunwind.h>
#include <libunwind-x86_64.h>
#include <libunwind-ptrace.h>

void my_backtrace(pid_t pid);

#endif