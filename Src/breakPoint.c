/* Fonctionnement breakPoint
 * Remplacer l'adresse du breakpoint par une instruction qui crash
 * Une fois crash il faut verifier si le signal envoyé est un SIGTRAP et que
 * l'adresse est bien celle du breakpoint.
 * Si c'est le cas, on supprime le breakpoint pour remettre l'instruction original.
 * On remet le rip à l'adresse du breakpoint pour que la prochaine instruction
 * executé soit celle ci.
 * On execute une et une seul instruction (PTRACE_SINGLESTEP) pour executer l'instruction
 * et s'arreter après. Une fois executé on remet le breakpoint (dans le cas d'une
 * boucle, on veut break plusieur fois, d'où l'interet de remettre le break point) et on continue.
 */

#include "../Include/breakPoint.h"

int waitchild(pid_t pid)
{
    int status;
    waitpid(pid, &status, 0);
    if (WIFSTOPPED(status))
    {
        return 0;
    }
    else if (WIFSIGNALED(status))
    {
        // gestion des signaux
        siginfo_t siginfo = ptrace_getsiginfo(pid);
        ptrace_signal(siginfo);
        return 2;
    }
    else if (WIFEXITED(status))
    {
        return 1;
    }
    else
    {
        printf("%d raised an unexpected status %d", pid, status);
        return 1;
    }
}

/**
 * @brief Ajout un breakpoint à l'adresse donnée
 *
 * @param pid pid du processus
 * @param addr adresses du breakpoint
 * @return int instruction remplacée par le breakpoint
 */
unsigned long addBp(pid_t pid, unsigned long address)
{
    const unsigned long original = ptrace(PTRACE_PEEKTEXT, pid, address, NULL);
    const unsigned long int3 = (original & 0xFFFFFFFFFFFFFF00) | 0x00000000000000CC;
    ptrace(PTRACE_POKETEXT, pid, address, int3);
    return original;
}

/**
 * @brief Enleve un break point à l'adresse donnée
 *
 * @param pid pid du processus
 * @param address Adresse du breakpoint
 * @param original instruction precédement suprimer à remettre
 */
void removebp(pid_t pid, unsigned long address, unsigned long original)
{
    ptrace(PTRACE_POKETEXT, pid, address, original);
}

/**
 * @brief Affiche le rip du processus
 *
 * @param pid pid du processus
 */
void dumpRIP(const pid_t pid)
{
    const unsigned long rip = ptrace(PTRACE_PEEKUSER, pid, 8 * RIP, NULL);
    printf("RIP = %lx\n", rip);
}

/**
 * @brief Change le rip du processus
 *
 * @param pid pid du processus
 * @param address nouvelle adresse pointé par le rip
 */
void replaceRip(pid_t pid, unsigned long address)
{
    ptrace(PTRACE_POKEUSER, pid, 8 * RIP, address);
}

/**
 * @brief Permet de retrouver l'adresse physique à partir
 * du fichier mappé
 *
 * @param pid pid du processus
 */

unsigned long getTracedRAMAddress(pid_t pid)
{
    size_t size = 50;
    char buffer[size];

    int size_path = snprintf(buffer, 50, "/proc/%d/maps", pid);

    FILE *input = NULL;

    input = fopen(buffer, "r"); // read mode

    if (input == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    getline((char **)buffer, (size_t *)size, input);
    fclose(input);

    int idx_line = 0;
    char hyphen;
    do
    {
        hyphen = buffer[idx_line++];
        if (hyphen == '-')
            buffer[idx_line - 1] = '\0';
    } while (hyphen != '-');

    return strtoul(buffer, NULL, 0);
}
