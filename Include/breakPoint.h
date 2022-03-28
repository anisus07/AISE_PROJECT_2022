#ifndef _BREAKPOINT_H_
#define _BREAKPOINT_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <elf.h>

#include "signaux.h"

int waitchild(pid_t pid);
unsigned long addBp(pid_t pid, unsigned long address);
void removebp(pid_t pid, unsigned long address, unsigned long original);
void dumpRIP(const pid_t pid);
void replaceRip(pid_t pid, unsigned long address);
unsigned long getTracedRAMAddress(pid_t pid);

#endif
