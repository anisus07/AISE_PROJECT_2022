

#define _GNU_SOURCE
#include <sys/user.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <sys/types.h>
#include <ucontext.h>
#include <execinfo.h>

#include "./Include/memory.h"
#include "./Include/breakPoint.h"
#include "./Include/backtrace.h"
#include "./Include/signaux.h"

void showInfo()
{

    //Fonction pour Afficher le pid, ppid, gid et le chemin de notre fichier
    char path[400];
    getcwd(path, 400);
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("\033[1m|%10s |%10s |%10s |%30s \033[0m\n", "PID", "PPID", "GID", "PATH");
    fflush(stdout);
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("|%10d |%10.3d |%10.3d |%30s\n",
           getpid(),
           getppid(),
           getgid(), path);

    fflush(stdout);
    printf("------------------------------------------------------------------------------------------------------------\n");
}
void showMemory()
{
    //fonction affich() qui fais appelle à la librarie memory
    affic();
}
static int shared_lib(struct dl_phdr_info *info, size_t size, void *data)
{
    //fonction qui permet d'afficher la liste des bibliothèques chargées
    int j;

    printf("name=%s (%d segments)\n", info->dlpi_name,
           info->dlpi_phnum);

    for (j = 0; j < info->dlpi_phnum; j++)
        printf("\t\t header %2d: address=%10p\n", j,
               (void *)(info->dlpi_addr + info->dlpi_phdr[j].p_vaddr));
    return 0;
}
void showVariablesFunctions()
{
    char *const eargv[] = {"readelf", "-s", "./main_prog", NULL};
    execvp(eargv[0], eargv);
}

void showMenu()
{
    //fonction pour afficher le menu
    printf("\n\n");
    printf("  #####################################################\n");
    printf("     1.  Informations générales\n");
    printf("     2.  Etat de la mémoire\n");
    printf("     3.  La liste des bibliothèques chargées\n");
    printf("     4.  La liste des variables globales et fonctions disponibles\n");
    printf("     0.  Pour lancer l'éxecution du programme\n");
    printf("  #####################################################\n");
    printf("\t Votre choix :   ");
}
void Affichage()
{
    int c = 0;
    printf("		                 Bienvenue sur notre Debuggeur GDB\n");
    printf("------------------------------------------------------------------------------------------------------------\n");
    time_t curtime;
    time(&curtime);
    printf("                                     %s", ctime(&curtime));
    printf("------------------------------------------------------------------------------------------------------------\n");

    do
    {
        showMenu();

        scanf("%d", &c);

        switch (c)
        {
        case 1:
            showInfo(); //Pour le processus pere
            break;
        case 2:
            showMemory();
            break;
        case 3:
            dl_iterate_phdr(shared_lib, NULL);

            break;

        case 4:
            showVariablesFunctions();
            break;
        }

    } while (c != 0);
}

int main(int argc, char **argv)
{

    if (argc != 2)
    {
        printf("Forgot an argument\n");
    }

    Affichage();

    pid_t child = fork();
    if (child == 0)
    {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], argv);
    }
    else
    {

        char adress_size[100];

        // gestion des signaux
        struct sigaction sa;

        sa.sa_sigaction = (void *)bt_sighandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART | SA_SIGINFO;

        sigaction(SIGSEGV, &sa, NULL);
        sigaction(SIGFPE, &sa, NULL);
        sigaction(SIGXCPU, &sa, NULL);
        sigaction(SIGPIPE, &sa, NULL);
        sigaction(SIGXFSZ, &sa, NULL);


        waitchild(child);
        ptrace(PTRACE_CONT, child, NULL, NULL);

        // entrer l'adresse du breakpoint
        scanf("%s", adress_size);

        //unsigned long physical_adress = getTracedRAMAddress(child);

        const unsigned long bpAddress = strtol(adress_size, NULL, 16);

        printf("Begin breakpoint 0x%016lX\n", bpAddress);

        // Ajout du breakpoint
        unsigned long originalInstruction = addBp(child, bpAddress);

        // Attente du breakpoint
        while (waitchild(child) < 1)
        {
            printf("Breakpoint hit !\n");

            dumpRIP(child);

            getchar();

            removebp(child, bpAddress, originalInstruction);

            ptrace(PTRACE_POKEUSER, child, 8 * RIP, bpAddress);
            my_backtrace(child);

            ptrace(PTRACE_SINGLESTEP, child, NULL, NULL);
            waitchild(child);

            addBp(child, bpAddress);

            ptrace(PTRACE_CONT, child, NULL, NULL);
        }
    }

    return 0;
}