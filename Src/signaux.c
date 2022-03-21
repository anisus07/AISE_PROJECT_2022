/* Gestion des signaux
 * deux maniere pour gestionner un signal
 * une premiere à l'aide de ptrace 
 * et une autre à l'aide d'une structure sigaction
 */

#include "../Include/signaux.h"

/**
 * @brief Récupération d'une structure siginfo_t à partir
 * d'un appel à ptrace
 *
 * @param target pid du processus
 * @return struct siginfo_t
 */
siginfo_t ptrace_getsiginfo(pid_t target)
{
    siginfo_t targetsig;
    if (ptrace(PTRACE_GETSIGINFO, target, NULL, &targetsig) == -1)
    {
        fprintf(stderr, "ptrace(PTRACE_GETSIGINFO) failed\n");
        exit(1);
    }
    return targetsig;
}

/**
 * @brief Gestion du signal à partir de la struct siginfo_t renvoyée 
 * par la fonction ptrace_getsiginfo()
 * 
 *
 * @param targetsig struct siginfo_t
 */
void ptrace_signal(siginfo_t targetsig)
{
    switch (targetsig.si_signo)
    {
    case SIGSEGV:

        if (targetsig.si_code == SEGV_MAPERR)
        {
            printf("Got signal %d : Address isn't mapped to an object\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }

        if (targetsig.si_code == SEGV_ACCERR)
        {
            printf("Got signal %d : The mapping doesn't allow the attempted access\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }
        break;
    case SIGFPE:
        if (targetsig.si_code == FPE_INTDIV)
        {
            printf("Got signal %d : Address isn't mapped to an object\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }

        if (targetsig.si_code == FPE_INTOVF)
        {
            printf("Got signal %d : The mapping doesn't allow the attempted access\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }
        if (targetsig.si_code == FPE_FLTDIV)
        {
            printf("Got signal %d : Address isn't mapped to an object\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }

        if (targetsig.si_code == FPE_FLTOVF)
        {
            printf("Got signal %d : The mapping doesn't allow the attempted access\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }
        if (targetsig.si_code == FPE_FLTUND)
        {
            printf("Got signal %d : Address isn't mapped to an object\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }

        if (targetsig.si_code == FPE_FLTRES)
        {
            printf("Got signal %d : The mapping doesn't allow the attempted access\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }
        if (targetsig.si_code == FPE_FLTINV)
        {
            printf("Got signal %d : Address isn't mapped to an object\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }

        if (targetsig.si_code == FPE_FLTSUB)
        {
            printf("Got signal %d : The mapping doesn't allow the attempted access\n", targetsig.si_signo);
            printf("Faulty address is %p\n", targetsig.si_addr);
        }
        break;
    case SIGXCPU:

        printf("Got signal %d : exceed of limit processor time consumed\n", targetsig.si_signo);
        printf("Faulty address is %p \n", targetsig.si_addr);
        break;
    case SIGPIPE:

        printf("Got signal %d : the pipe has no reader\n", targetsig.si_signo);
        printf("Faulty address is %p \n", targetsig.si_addr);
        break;
    case SIGXFSZ:

        printf("Got signal %d : exceeded file size\n", targetsig.si_signo);
        printf("Faulty address is %p \n", targetsig.si_addr);
        break;
    }
}

/**
 * @brief Gestionnaire de signaux à l'aide d'une fonction sigaction
 * pour utiliser cette fonction mettre une structure de type struct sigaction
 * le backtrace permet de remonter la pile à partir de l'adresse du signal
 *
 * @param sig numéro du signal
 * @param info récupération de la structure siginfo_t
 * @param secret context pour utiliser une structure ucontext_t
 */
void bt_sighandler(int sig, siginfo_t *info, void *secret)
{

    void *trace[16];
    char **messages = (char **)NULL;
    int i, trace_size = 0;
    ucontext_t *uc = (ucontext_t *)secret;

    switch (sig)
    {
    case SIGSEGV:
        if (info->si_code == SEGV_MAPERR)
        {
            printf("Got signal %d : Address isn't mapped to an object\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == SEGV_ACCERR)
        {
            printf("Got signal %d : The mapping doesn't allow the attempted access\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        break;
    case SIGFPE:
        if (info->si_code == FPE_INTDIV)
        {
            printf("Got signal %d : Integer division by zero\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == FPE_INTOVF)
        {
            printf("Got signal %d : Integer overflow\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == FPE_FLTDIV)
        {
            printf("Got signal %d : Floating-point divide by zero\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == FPE_FLTOVF)
        {
            printf("Got signal %d : Floating-point overflow\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == FPE_FLTUND)
        {
            printf("Got signal %d : Floating-point underflow\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == FPE_FLTRES)
        {
            printf("Got signal %d : Floating-point inexact result\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == FPE_FLTINV)
        {
            printf("Got signal %d : Invalid floating-point operation\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        if (info->si_code == FPE_FLTSUB)
        {
            printf("Got signal %d : Subscript out of range (not currently used)\n", info->si_signo);
            printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        }
        break;
    case SIGXCPU:

        printf("Got signal %d : exceed of limit processor time consumed\n", sig);
        printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        break;
    case SIGPIPE:
        printf("Got signal %d : the pipe has no reader\n", sig);
        printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        break;
    case SIGXFSZ:
        printf("Got signal %d : exceeded file size\n", sig);
        printf("Faulty address is %p, from %lld\n", info->si_addr, uc->uc_mcontext.gregs[REG_RIP]);
        break;
    }

    trace_size = backtrace(trace, 16);

    /*écraser sigaction avec l'adresse de l'appelant */
    trace[1] = (void *)uc->uc_mcontext.gregs[REG_RIP];

    messages = backtrace_symbols(trace, trace_size);
    /*sauter le premier cadre de pile (points ici) */
    printf("[bt] Execution path\n");

    for (i = 1; i < trace_size; ++i)
        printf("[bt] %s\n", messages[i]);
}